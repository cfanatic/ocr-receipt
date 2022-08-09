#include "engine_easyocr.hpp"

namespace ocr
{

    engine_easyocr::engine_easyocr()
    {
        set_engine(name::easyocr);
        Py_Initialize();
    }

    engine_easyocr::~engine_easyocr()
    {
        Py_Finalize();
    }

    void engine_easyocr::init(const std::string &path)
    {
        set_path(path);
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\"..\")");
    }

    std::string engine_easyocr::text()
    {
        PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;
        pName = PyUnicode_FromString(std::string("main").c_str());
        pModule = PyImport_Import(pName);
        pFunc = PyObject_GetAttrString(pModule, std::string("ocr").c_str());
        pArgs = PyTuple_Pack(2, PyUnicode_FromString(std::string("/src/ocr/misc/input/receipt_2.jpg").c_str()), PyUnicode_FromString(std::string("77,304,1280,111").c_str()));
        pValue = PyObject_CallObject(pFunc, pArgs);
        auto result = _PyUnicode_AsString(pValue);
        std::cout << result << std::endl;
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
