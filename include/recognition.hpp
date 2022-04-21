#ifndef RECOGNITION_HPP
#define RECOGNITION_HPP

#include <iostream>
#include <tesseract/baseapi.h>

namespace ocr
{
    class receipt
    {

    public:
        receipt();
        ~receipt();

        void extract();

    private:
    };

} // namespace ocr

#endif // RECOGNITION_HPP
