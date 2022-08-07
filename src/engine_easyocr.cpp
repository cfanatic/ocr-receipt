#include "engine_easyocr.hpp"

namespace ocr
{

    engine_easyocr::engine_easyocr()
    {
        m_engine = name::easyocr;
        Py_Initialize();
    }

    engine_easyocr::~engine_easyocr()
    {
        Py_Finalize();
    }

    void engine_easyocr::print()
    {
        std::cout << "Engine: " << get_engine() << std::endl;
    }

    void engine_easyocr::python()
    {
        PyRun_SimpleString("print(\"Hello World from Python!!\")");
    }

} // namespace ocr
