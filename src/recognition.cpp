#include "recognition.hpp"

namespace ocr {

    receipt::receipt()
    {
    }

    receipt::~receipt()
    {
    }

    void receipt::extract()
    {
        std::cout << "extract()" << std::endl;
        tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
        delete api;
    }

} // namespace ocr
