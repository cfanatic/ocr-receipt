#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "receipt.hpp"

namespace ocr
{
    class configuration
    {

    public:
        struct easyocr
        {
            inline bool operator==(const easyocr &e) const
            {
                return this->directory == e.directory && this->file == e.file && this->offset == e.offset;
            }

            std::string directory;
            std::string file;
            int offset;
        };

        static configuration &get_instance();

        void load(const std::string &path);
        receipt::shop enum_conversion(std::string shop);

        std::vector<std::string> get_shops() const;
        std::map<receipt::shop, int> get_paddings() const;
        std::vector<std::string> get_filters() const;
        easyocr get_easyocr() const;
        int get_threshold() const;

        void set_path(const std::string &path)
        {
            boost::filesystem::path path_rel(path);
            std::string path_abs(boost::filesystem::canonical(path_rel).string());
            m_path = std::move(path_abs);
        }
        std::string get_path() const { return m_path; }

    private:
        configuration() = default;
        configuration(const configuration &) = delete;
        configuration &operator=(const configuration &) = delete;
        configuration(configuration &&) = delete;
        configuration &operator=(configuration &&) = delete;

        std::string m_path;
        boost::property_tree::ptree m_config;
        std::vector<std::string> m_shops;
        std::map<std::string, receipt::shop> m_shops_enum;
        std::map<receipt::shop, int> m_paddings;
        std::vector<std::string> m_filters;
        easyocr m_easyocr;
        int m_threshold;
    };

    extern configuration &config;

} // namespace ocr

#endif // CONFIGURATION_HPP
