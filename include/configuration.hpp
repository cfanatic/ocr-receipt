#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace ocr
{
    class configuration
    {

    public:
        static configuration &get_instance();

        void load(const std::string &path);

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
    };

    extern configuration &config;

} // namespace ocr

#endif // CONFIGURATION_HPP
