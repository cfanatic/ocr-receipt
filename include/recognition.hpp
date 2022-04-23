#ifndef RECOGNITION_HPP
#define RECOGNITION_HPP

#include <algorithm>
#include <iostream>
#include <regex>
#include <vector>
#include <boost/filesystem.hpp>
#include "boost/format.hpp"
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

namespace ocr
{
    class receipt
    {

    public:
        struct detection
        {
            int id;
            int x;
            int y;
            int w;
            int h;
            int conf;
            std::string text;
        };

        receipt(const std::string &path);
        ~receipt();

        void init();
        void preprocess();
        void overlay(std::vector<receipt::detection> detections);
        std::vector<detection> extract();
        detection extract_all();

        friend std::ostream &operator<<(std::ostream &os, const detection &d)
        {
            return os << boost::format("Box[%d]: x=%d, y=%d, w=%d, h=%d, conf: %d, text: %s") % d.id % d.x % d.y % d.w % d.h % d.conf % d.text << std::endl;
        }

    private:
        std::string m_path;
        Pix *m_img_pix;
        cv::Mat m_img_cv;
        tesseract::TessBaseAPI *m_api;
    };

} // namespace ocr

#endif // RECOGNITION_HPP
