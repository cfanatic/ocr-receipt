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
        pixDestroy(&m_image);
        delete m_api;
    }

    void receipt::init()
    {
        if (m_api->Init(nullptr, "deu", tesseract::OEM_LSTM_ONLY))
        {
            std::cerr << "Error: Could not initialize Tesseract" << std::endl;
        }
        else
        {
            m_image = pixRead(m_path.c_str());
            m_api->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
            m_api->SetImage(m_image);
        }
    }

    void receipt::preprocess()
    {
        int orient_deg;
        float orient_conf;
        if (!m_api->DetectOrientationScript(&orient_deg, &orient_conf, nullptr, nullptr))
        {
            std::cerr << "Error: Could not detect the orientation of the input image" << std::endl;
        }
        else
        {
            std::cout << boost::format("orient_deg=%d, orient_conf=%.2f") % orient_deg % orient_conf << std::endl;
        }
    }

    std::vector<receipt::detection> receipt::extract()
    {
        std::vector<receipt::detection> detections;
        Boxa *boxes = m_api->GetComponentImages(tesseract::RIL_TEXTLINE, true, true, 20, nullptr, nullptr, nullptr);
        for (int i = 0; i < boxes->n; i++)
        {
            auto box = boxaGetBox(boxes, i, L_CLONE);
            auto &x = box->x, y = box->y, w = box->w, h = box->h;
            m_api->SetRectangle(x, y, w, h);
            receipt::detection detection = {i, x, y, w, h, m_api->MeanTextConf(), m_api->GetUTF8Text()};
            std::string &text = detection.text;
            text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
            detections.push_back(detection);
            boxDestroy(&box);
        }
        return detections;
    }

    receipt::detection receipt::extract_all()
    {
        receipt::detection detection = {0, 0, 0, 0, 0, m_api->MeanTextConf(), m_api->GetUTF8Text()};
        return detection;
    }

} // namespace ocr
