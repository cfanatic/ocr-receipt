#include "engine_easyocr.hpp"

namespace ocr
{

    engine_easyocr::engine_easyocr()
    {
        Py_Initialize();
        set_engine(name::easyocr);
    }

    engine_easyocr::~engine_easyocr()
    {
        Py_Finalize();
    }

    void engine_easyocr::init(const std::string &path)
    {
    }

    std::string engine_easyocr::text()
    {
        return std::string("-");
    }

    int engine_easyocr::conf()
    {
        return 0;
    }

    void engine_easyocr::overlay(const std::vector<std::vector<int>> &bounding_boxes)
    {
    }

    void engine_easyocr::print()
    {
        PyRun_SimpleString("print(\"Hello World from Python!!\")");
    }

} // namespace ocr
