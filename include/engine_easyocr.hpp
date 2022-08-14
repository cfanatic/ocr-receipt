#ifndef ENGINE_EASYOCR_HPP
#define ENGINE_EASYOCR_HPP

#include <Python.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "engine.hpp"

namespace ocr
{

    class engine_easyocr : public engine
    {

    public:
        engine_easyocr(const std::string &path);
        ~engine_easyocr();

        void init() override;
        std::string text() override;
        int conf() override;
        void overlay(const std::vector<std::vector<int>> &bounding_boxes) override;

        void set_bounding_box(int left, int top, int width, int height);
        std::string get_bounding_box();

    private:
        std::vector<int> m_bounding_box;
        static const std::string s_wrapper_code;
    };

} // namespace ocr

#endif // ENGINE_EASYOCR_HPP
