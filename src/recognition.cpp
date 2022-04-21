#include "recognition.hpp"

namespace ocr {

    receipt::receipt(const char *path) : m_path(path)
    {
        m_image = pixRead(m_path);
        m_api = new tesseract::TessBaseAPI();
    }

    receipt::~receipt()
    {
        pixDestroy(&m_image);
        delete m_api;
    }

    void receipt::init()
    {
        if (m_api->Init(NULL, "deu"))
        {
            std::cerr << "Error: Could not initialize Tesseract" << std::endl;
        }
        else
        {
            m_api->SetImage(m_image);
        }
    }

    void receipt::extract()
    {
        std::cout << m_api->GetUTF8Text() << std::endl;
        m_api->End();
    }

} // namespace ocr
