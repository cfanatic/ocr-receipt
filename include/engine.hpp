#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <map>

namespace ocr
{

    class engine
    {

    public:
        enum class name
        {
            easyocr,
            tesseract
        };

        engine() = default;
        virtual ~engine() = default;

        virtual void print() = 0;

    protected:
        name get_engine() { return m_engine; };

        friend std::ostream &operator<<(std::ostream &os, const name &n)
        {
            std::map<name, std::string> names{
                {name::easyocr, "easyocr"},
                {name::tesseract, "tesseract"},
            };
            return os << names[n];
        }

        name m_engine;
    };

} // namespace ocr

#endif // ENGINE_HPP
