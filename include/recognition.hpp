#ifndef RECOGNITION_HPP
#define RECOGNITION_HPP

#include <iostream>
#include <boost/filesystem.hpp>
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

    private:
        std::string m_path;
        Pix *m_image;
        tesseract::TessBaseAPI *m_api;
    };

} // namespace ocr

#endif // RECOGNITION_HPP
