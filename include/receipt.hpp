#ifndef RECEIPT_HPP
#define RECEIPT_HPP

#include <algorithm>
#include <iostream>
#include <regex>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/locale.hpp>
#include "engine_easyocr.hpp"
#include "engine_tesseract.hpp"

namespace ocr
{
    class receipt
    {

    public:
        enum class shop
        {
            edeka,
            rewe,
            aldi,
            unknown
        };

        enum class iterator
        {
            block,
            line,
            word
        };

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

        struct article
        {
            inline bool operator==(const article &ra) const
            {
                return this->name == ra.name && this->price == ra.price;
            }

            std::string name;
            float price;
        };

        receipt(const std::string &path);
        ~receipt();

        void init();
        void preprocess();
        std::vector<detection> extract(engine::name engine = engine::name::tesseract, iterator level = iterator::line);
        void overlay(std::vector<detection> detections);
        std::vector<article> process(std::vector<detection> detections);

        void set_path(const std::string &path)
        {
            boost::filesystem::path path_rel(path);
            std::string path_abs(boost::filesystem::canonical(path_rel).string());
            m_path = std::move(path_abs);
        }
        std::string get_path() const { return m_path; }
        void set_shop(const shop &shop) { m_shop = std::move(shop); }
        shop get_shop() const { return m_shop; }

        friend std::ostream &operator<<(std::ostream &os, const shop &s)
        {
            std::map<shop, std::string> shops{
                {shop::edeka, "edeka"},
                {shop::rewe, "rewe"},
                {shop::aldi, "aldi"},
                {shop::unknown, "unknown"},
            };
            return os << shops[s];
        }

        friend std::ostream &operator<<(std::ostream &os, const detection &d)
        {
            return os << boost::format("Box[%d]: x=%d, y=%d, w=%d, h=%d, conf: %d, text: %s") % d.id % d.x % d.y % d.w % d.h % d.conf % d.text << std::endl;
        }

        friend std::ostream &operator<<(std::ostream &os, const article &a)
        {
            return os << boost::format("Article=%s, Price=%.2f") % a.name % a.price << std::endl;
        }

    private:
        std::string m_path;
        shop m_shop;
        engine_tesseract *m_engine;
        engine_easyocr *m_engine_e;
    };

} // namespace ocr

#endif // RECEIPT_HPP
