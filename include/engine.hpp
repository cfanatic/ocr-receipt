#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <map>
#include <vector>

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

        virtual void init(const std::string &path) = 0;
        virtual std::string text() = 0;
        virtual int conf() = 0;
        virtual void overlay(const std::vector<std::vector<int>> &bounding_boxes) = 0;

    protected:
        void set_engine(const name &engine) { m_engine = std::move(engine); }
        name get_engine() const { return m_engine; };
        void set_path(const std::string &path)
        {
            m_path = std::move(path);
        }
        std::string get_path() const { return m_path; }

        friend std::ostream &operator<<(std::ostream &os, const name &n)
        {
            std::map<name, std::string> names{
                {name::easyocr, "easyocr"},
                {name::tesseract, "tesseract"},
            };
            return os << names[n];
        }

    private:
        name m_engine;
        std::string m_path;
    };

} // namespace ocr

#endif // ENGINE_HPP
