#ifndef ENGINE_TESSERACT_HPP
#define ENGINE_TESSERACT_HPP

#include "engine.hpp"

namespace ocr
{

    class engine_tesseract : public engine
    {

    public:
        engine_tesseract();
        ~engine_tesseract();

        void print() override;

    private:
    };

} // namespace ocr

#endif // ENGINE_TESSERACT_HPP
