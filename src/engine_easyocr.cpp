#include "configuration.hpp"
#include "engine_easyocr.hpp"

namespace ocr
{

    engine_easyocr::engine_easyocr(const std::string &path)
    {
        set_engine(name::easyocr);
        set_path(path);
        std::ofstream wrapper;
        boost::filesystem::path dir(ocr::config.get_easyocr().directory);
        boost::filesystem::path file(ocr::config.get_easyocr().file);
        boost::filesystem::path wrapper_path = dir / file;
        wrapper.open(wrapper_path.string(), std::ios_base::trunc);
        wrapper << engine_easyocr::s_wrapper_code;
        wrapper.close();
        Py_Initialize();
    }

    engine_easyocr::~engine_easyocr()
    {
        Py_Finalize();
    }

    void engine_easyocr::init()
    {
        boost::format sys_path = boost::format("sys.path.append('%s')") % ocr::config.get_easyocr().directory;
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.dont_write_bytecode = True");
        PyRun_SimpleString(sys_path.str().c_str());
    }

    std::string engine_easyocr::text()
    {
        PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;
        std::string wrapper = ocr::config.get_easyocr().file;
        wrapper.erase(wrapper.find(".py"), 3);
        pName = PyUnicode_FromString(wrapper.c_str());
        pModule = PyImport_Import(pName);
        pFunc = PyObject_GetAttrString(pModule, "ocr_short");
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
        std::stringstream box;
        int left = m_bounding_box[0], top = m_bounding_box[1], width = m_bounding_box[2], height = m_bounding_box[3];
        box << left << "," << top << "," << width << "," << height;
        return box.str();
    }

    const std::string engine_easyocr::s_wrapper_code =
R"(
import sys
sys.dont_write_bytecode = True
import easyocr
import cv2

def ocr_short(path, bounding_box) -> str:
    reader = easyocr.Reader(["de", "en"], verbose=False)
    img = cv2.imread(path)
    box = [int(c) for c in bounding_box.split(",")]
    x, y, w, h = box[0], box[1], box[2], box[3]
    img = img[y:y+h, x:x+w]
    article = []
    detections = reader.readtext(img, detail=1)
    for d in detections:
        article.append(d[-2])
    return " ".join(article)
)";

} // namespace ocr
