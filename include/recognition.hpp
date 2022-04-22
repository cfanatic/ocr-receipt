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

        friend std::ostream &operator<<(std::ostream &os, const detection &d)
        {
            return os << boost::format("Box[%d]: x=%d, y=%d, w=%d, h=%d, conf: %d, text: %s") % d.id % d.x % d.y % d.w % d.h % d.conf % d.text;
        }

    public:
        receipt(const std::string &path);
        ~receipt();

        void init();
        std::vector<detection> extract();
        detection extract_all();

    private:
        std::string m_path;
        Pix *m_image;
        tesseract::TessBaseAPI *m_api;
    };

} // namespace ocr

#endif // RECOGNITION_HPP
