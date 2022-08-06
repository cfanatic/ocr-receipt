#include "configuration.hpp"
#include "receipt.hpp"

namespace ocr
{

    receipt::receipt(const std::string &path)
    {
        set_path(path);
        m_api = new tesseract::TessBaseAPI();
    }

    receipt::~receipt()
    {
        pixDestroy(&m_img_pix);
        m_img_cv.release();
        m_api->End();
        delete m_api;
    }

    void receipt::init()
    {
        int orient_deg;
        float orient_conf;
        if (m_api->Init(nullptr, "deu", tesseract::OEM_LSTM_ONLY))
        {
            std::cerr << "Error: Could not initialize Tesseract" << std::endl;
        }
        else
        {
            m_img_pix = pixRead(m_path.c_str());
            m_img_cv = cv::imread(m_path.c_str());
            m_api->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
            m_api->SetImage(m_img_pix);
        }
        if (!m_api->DetectOrientationScript(&orient_deg, &orient_conf, nullptr, nullptr))
        {
            std::cerr << "Error: Could not detect the orientation of the input image" << std::endl;
        }
        else
        {
            std::cout << boost::format("orient_deg=%d, orient_conf=%.2f") % orient_deg % orient_conf << std::endl;
        }
        boost::locale::generator gen;
        std::locale loc = gen("de_DE.UTF-8");
        std::locale::global(loc);
    }

    void receipt::preprocess()
    {
        std::string text = m_api->GetUTF8Text();
        std::vector<std::string> shop_list = ocr::config.get_shops();
        auto shop_it = std::find_if(shop_list.begin(), shop_list.end(),
                                    [&](const auto &s)
                                    {
                                        text = boost::locale::to_lower(text);
                                        return text.find(s) != std::string::npos;
                                    });
        if (shop_it != shop_list.end())
        {
            set_shop(ocr::config.enum_conversion(*shop_it));
        }
        else
        {
            set_shop(receipt::shop::unknown);
        }
        std::cout << "Shop: " << get_shop() << std::endl;
    }

    std::vector<receipt::detection> receipt::extract(receipt::iterator level, receipt::extraction engine)
    {
        std::vector<receipt::detection> detections;
        switch (level)
        {
        case receipt::iterator::block:
        {
            receipt::detection detection = {0, 0, 0, 0, 0, m_api->MeanTextConf(), m_api->GetUTF8Text()};
            detections.push_back(detection);
            break;
        }
        case receipt::iterator::line:
        {
            std::map<receipt::shop, int> paddings = ocr::config.get_paddings();
            Boxa *boxes = m_api->GetComponentImages(tesseract::RIL_TEXTLINE, true, true, paddings[get_shop()], nullptr, nullptr, nullptr);
            for (int i = 0; i < boxes->n; i++)
            {
                auto box = boxaGetBox(boxes, i, L_CLONE);
                int x = box->x, y = box->y, w = box->w, h = box->h;
                m_api->SetRectangle(x, y, w, h);
                int conf = m_api->MeanTextConf();
                if (conf > ocr::config.get_threshold() && engine == receipt::extraction::tesseract)
                {
                    std::string text = m_api->GetUTF8Text();
                    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
                    receipt::detection detection = {static_cast<int>(detections.size()), x, y, w, h, conf, text};
                    detections.push_back(detection);
                }
                else if (conf > ocr::config.get_threshold() && engine == receipt::extraction::easyocr)
                {
                    // todo: call wrapper
                }
                boxDestroy(&box);
            }
            break;
        }
        case receipt::iterator::word:
            break;
        }
        return detections;
    }

    void receipt::overlay(std::vector<receipt::detection> detections)
    {
        std::string path_out = m_path;
        for (const auto &d : detections)
        {
            cv::Rect rec = cv::Rect(d.x, d.y, d.w, d.h);
            cv::rectangle(m_img_cv, rec, cv::Scalar(0, 0, 255), 2, 8, 0);
        }
        path_out = std::regex_replace(path_out, std::regex("input"), "output");
        path_out = std::regex_replace(path_out, std::regex(".jpg"), "_overlay.jpg");
        cv::imwrite(path_out, m_img_cv);
    }

    std::vector<receipt::article> receipt::process(std::vector<receipt::detection> detections)
    {
        std::vector<receipt::article> articles;
        auto filter_article = [&](std::string n)
        {
            bool drop;
            std::vector<std::string> filter_list = ocr::config.get_filters();
            drop = drop | std::any_of(filter_list.begin(), filter_list.end(),
                                      [&](const auto &f)
                                      {
                                          std::string name;
                                          name = boost::locale::to_lower(n);
                                          return name.find(f) != std::string::npos;
                                      });
            drop = drop | n.length() < 3;
            return drop;
        };
        auto convert_name = [&](std::string n)
        {
            std::string name;
            n = std::regex_replace(n, std::regex(" ,"), ".");
            name = boost::locale::to_title(n);
            return name;
        };
        auto convert_price = [&](std::string p)
        {
            float price;
            p = std::regex_replace(p, std::regex(", "), ".");
            p = std::regex_replace(p, std::regex(" ,"), ".");
            p = std::regex_replace(p, std::regex(","), ".");
            price = std::stof(p);
            return price;
        };
        for (const auto &d : detections)
        {
            std::string name, price;
            std::regex ex_name("([a-zA-Z]+)");                             // matches the first word
            std::regex ex_price("[0-9]{1,3}(\\s?)[,.](\\s?)([0-9]{1,2})"); // matches the price
            std::smatch match;
            if (std::regex_search(d.text.begin(), d.text.end(), match, ex_price))
            {
                price = match[0];
                if (std::regex_search(d.text.begin(), d.text.end(), match, ex_name))
                {
                    std::size_t begin = d.text.find(match[0]);
                    std::size_t end = d.text.find(price);
                    name = d.text.substr(begin, end - begin - 1);
                }
                if (filter_article(name))
                {
                    std::cout << "Drop: " << d.text << std::endl;
                    continue;
                }
                receipt::article article = {convert_name(name), convert_price(price)};
                articles.push_back(article);
            }
        }
        return articles;
    }

} // namespace ocr
