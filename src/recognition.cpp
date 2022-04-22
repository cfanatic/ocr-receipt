#include "recognition.hpp"

namespace ocr {

    receipt::receipt(const std::string &path)
    {
        boost::filesystem::path path_rel(path);
        std::string path_abs(boost::filesystem::canonical(path_rel).string());
        m_path = path_abs;
        m_image = pixRead(path_abs.c_str());
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
