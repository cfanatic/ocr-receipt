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
        if (m_api->Init(NULL, "deu", tesseract::OEM_LSTM_ONLY))
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

    void receipt::extract()
    {
        Boxa *boxes = m_api->GetComponentImages(tesseract::RIL_TEXTLINE, true, true, 20, NULL, NULL, NULL);
        for (int i = 0; i < boxes->n; i++)
        {
            auto box = boxaGetBox(boxes, i, L_CLONE);
            auto &x = box->x, y = box->y, w = box->w, h = box->h;
            m_api->SetRectangle(x, y, w, h);
            std::cout << boost::format("Box[%d]: x=%d, y=%d, w=%d, h=%d, conf: %d, text: %s") % i % x % y % w % h % m_api->MeanTextConf() % m_api->GetUTF8Text();
            boxDestroy(&box);
        }
        init();
    }

    void receipt::extract_all()
    {
        std::cout << m_api->GetUTF8Text() << std::endl;
    }

} // namespace ocr
