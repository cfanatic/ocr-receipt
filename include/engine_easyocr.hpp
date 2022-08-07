#ifndef ENGINE_EASYOCR_HPP
#define ENGINE_EASYOCR_HPP

#include <Python.h>
#include "engine.hpp"

namespace ocr
{

    class engine_easyocr : public engine
    {

    public:
        engine_easyocr();
        ~engine_easyocr();

        void print() override;
        void python();

    private:
    };

} // namespace ocr

#endif // ENGINE_EASYOCR_HPP
