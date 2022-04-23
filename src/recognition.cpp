#include "recognition.hpp"

namespace ocr {

    receipt::receipt(const std::string &path)
    {
        boost::filesystem::path path_rel(path);
        std::string path_abs(boost::filesystem::canonical(path_rel).string());
        m_path = path_abs;
        m_api = new tesseract::TessBaseAPI();
    }

    receipt::~receipt()
    {
        m_api->End();
        pixDestroy(&m_img_pix);
        m_img_cv.release();
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
    }

    std::vector<receipt::detection> receipt::extract(receipt::iterator level)
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
            Boxa *boxes = m_api->GetComponentImages(tesseract::RIL_TEXTLINE, true, true, 20, nullptr, nullptr, nullptr);
            for (int i = 0; i < boxes->n; i++)
            {
                auto box = boxaGetBox(boxes, i, L_CLONE);
                int x = box->x, y = box->y, w = box->w, h = box->h;
                m_api->SetRectangle(x, y, w, h);
                int conf = m_api->MeanTextConf();
                if (conf > 30)
                {
                    std::string text = m_api->GetUTF8Text();
                    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
                    receipt::detection detection = {static_cast<int>(detections.size()), x, y, w, h, conf, text};
                    detections.push_back(detection);
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
        for (const auto &d : detections)
        {
            cv::Rect rec = cv::Rect(d.x, d.y, d.w, d.h);
            cv::rectangle(m_img_cv, rec, cv::Scalar(0, 0, 255), 2, 8, 0);
        }
        std::string path_out = std::regex_replace(m_path, std::regex(".jpg"), "_overlay.jpg");
        cv::imwrite(path_out, m_img_cv);
    }

    std::vector<receipt::article> receipt::process(std::vector<receipt::detection> detections)
    {
        std::vector<receipt::article> articles;
        for (const auto &d : detections)
        {
            std::regex ex_name("([a-zA-Z]+)");                       // matches the first word
            std::regex ex_price("[0-9]{1,3}[,.](\\s?)([0-9]{1,2})"); // matches either '0,00' or '0, 00'
            std::smatch match;
            if (std::regex_search(d.text.begin(), d.text.end(), match, ex_price))
            {
                std::string price = match[0];
                if (std::regex_search(d.text.begin(), d.text.end(), match, ex_name))
                {
                    std::size_t begin = d.text.find(match[0]);
                    std::size_t end = d.text.find(price);
                    std::string name = d.text.substr(begin, end - begin - 1);
                    receipt::article article = {name, price};
                    articles.push_back(article);
                }
            }
        }
        return articles;
    }

} // namespace ocr
