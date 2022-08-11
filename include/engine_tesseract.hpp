#ifndef ENGINE_TESSERACT_HPP
#define ENGINE_TESSERACT_HPP

#include <regex>
#include <vector>
#include <boost/format.hpp>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include "engine.hpp"

namespace ocr
{

    class engine_tesseract : public engine
    {

    public:
        typedef BOXA boxx;
        typedef BOX box;

        engine_tesseract(const std::string &path);
        ~engine_tesseract();

        void init() override;
        std::string text() override;
        int conf() override;
        void overlay(const std::vector<std::vector<int>> &bounding_boxes) override;

        boxx *get_bounding_boxes(int padding);
        box *get_bounding_box(boxx *bounding_boxes, int index);
        void set_bounding_box(int left, int top, int width, int height);

    private:
        tesseract::TessBaseAPI *m_api;
        cv::Mat m_img_cv;
        Pix *m_img_pix;
        std::vector<engine_tesseract::box *> bounding_box_storage;
    };

} // namespace ocr

#endif // ENGINE_TESSERACT_HPP
