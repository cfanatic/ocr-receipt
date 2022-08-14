#include "configuration.hpp"
#include "engine_easyocr.hpp"

namespace ocr
{

    engine_easyocr::engine_easyocr(const std::string &path)
    {
        set_engine(name::easyocr);
        set_path(path);
        Py_Initialize();
    }

    engine_easyocr::~engine_easyocr()
    {
        Py_Finalize();
    }

    void engine_easyocr::init()
    {
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.dont_write_bytecode = True");
        PyRun_SimpleString("sys.path.append(\"..\")");
    }

    std::string engine_easyocr::text()
    {
        PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;
        pName = PyUnicode_FromString(std::string("main").c_str());
        pModule = PyImport_Import(pName);
        pFunc = PyObject_GetAttrString(pModule, std::string("ocr").c_str());
        pArgs = PyTuple_Pack(2, PyUnicode_FromString(get_path().c_str()), PyUnicode_FromString(get_bounding_box().c_str()));
        pValue = PyObject_CallObject(pFunc, pArgs);
        return _PyUnicode_AsString(pValue);
    }

    int engine_easyocr::conf()
    {
        return 0;
    }

    void engine_easyocr::overlay(const std::vector<std::vector<int>> &bounding_boxes)
    {
    }

    void engine_easyocr::set_bounding_box(int left, int top, int width, int height)
    {
        int offset = ocr::config.get_easyocr().offset;
        std::vector<int> box = {left + offset, top + offset, width - offset, height - offset};
        m_bounding_box = box;
    }

    std::string engine_easyocr::get_bounding_box()
    {
        std::ostringstream box;
        int left = m_bounding_box[0], top = m_bounding_box[1], width = m_bounding_box[2], height = m_bounding_box[3];
        box << left << "," << top << "," << width << "," << height;
        return box.str();
    }

} // namespace ocr
