#ifndef RECOGNITION_HPP
#define RECOGNITION_HPP

#include <iostream>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

namespace ocr
{
    class receipt
    {

    public:
        receipt(const char *path);
        ~receipt();

        void init();
        void extract();

    private:
        const char *m_path;
        Pix *m_image;
        tesseract::TessBaseAPI *m_api;
    };

} // namespace ocr

#endif // RECOGNITION_HPP
