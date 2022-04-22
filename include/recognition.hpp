#ifndef RECOGNITION_HPP
#define RECOGNITION_HPP

#include <iostream>
#include <boost/filesystem.hpp>
#include "boost/format.hpp"
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

namespace ocr
{
    class receipt
    {

    public:
        receipt(const std::string &path);
        ~receipt();

        void init();
        void extract();
        void extract_all();

    private:
        std::string m_path;
        Pix *m_image;
        tesseract::TessBaseAPI *m_api;
    };

} // namespace ocr

#endif // RECOGNITION_HPP
