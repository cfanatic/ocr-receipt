#include "easyocr.hpp"

namespace ocr
{

    easyocr::easyocr()
    {
        Py_Initialize();
    }

    easyocr::~easyocr()
    {
        Py_Finalize();
    }

    void easyocr::print() const
    {
        PyRun_SimpleString("print(\"Hello World from Python!\")");
    }

} // namespace ocr
