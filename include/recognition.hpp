#ifndef RECOGNITION_HPP
#define RECOGNITION_HPP

#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include "boost/format.hpp"
#include <leptonica/allheaders.h>
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
        std::vector<detection> extract();
        detection extract_all();

        friend std::ostream &operator<<(std::ostream &os, const detection &d)
        {
            return os << boost::format("Box[%d]: x=%d, y=%d, w=%d, h=%d, conf: %d, text: %s") % d.id % d.x % d.y % d.w % d.h % d.conf % d.text;
        }

    private:
        std::string m_path;
        Pix *m_image;
        tesseract::TessBaseAPI *m_api;
    };

} // namespace ocr

#endif // RECOGNITION_HPP
