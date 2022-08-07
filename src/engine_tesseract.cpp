#include "engine_tesseract.hpp"

namespace ocr
{

    engine_tesseract::engine_tesseract()
    {
        m_engine = name::tesseract;
    }

    engine_tesseract::~engine_tesseract()
    {
    }

    void engine_tesseract::print()
    {
        std::cout << "Engine: " << get_engine() << std::endl;
    }

} // namespace ocr
