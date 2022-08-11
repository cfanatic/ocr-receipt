#ifndef ENGINE_EASYOCR_HPP
#define ENGINE_EASYOCR_HPP

#include <Python.h>
#include <vector>
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
        std::string text(const std::vector<int> &bounding_box) override;
        int conf() override;
        void overlay(const std::vector<std::vector<int>> &bounding_boxes) override;

        void print();

    private:
    };

} // namespace ocr

#endif // ENGINE_EASYOCR_HPP
