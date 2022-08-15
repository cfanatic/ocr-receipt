#include "configuration.hpp"

namespace ocr
{
    configuration &config = ocr::configuration::get_instance();

    configuration &configuration::get_instance()
    {
        static configuration *c = new configuration();
        return *c;
    }

    void configuration::load(const std::string &path)
    {
        set_path(path);
        boost::property_tree::read_json(get_path(), m_config);
        for (const boost::property_tree::ptree::value_type &item : m_config.get_child("shops"))
        {
            static int i;
            m_shops.push_back(item.second.data());
            m_shops_enum[item.second.data()] = static_cast<ocr::receipt::shop>(i);
            i++;
        }
        for (const boost::property_tree::ptree::value_type &filter : m_config.get_child("filters"))
        {
            m_filters.push_back(filter.second.data());
        }
        for (const boost::property_tree::ptree::value_type &item : m_config.get_child("engines.easyocr"))
        {
            if (item.first == "directory")
                m_easyocr.directory = item.second.get_value<std::string>();
            if (item.first == "file")
                m_easyocr.file = item.second.get_value<std::string>();
            if (item.first == "offset")
                m_easyocr.offset = item.second.get_value<int>();
        }
        for (const boost::property_tree::ptree::value_type &item : m_config.get_child("engines.tesseract"))
        {
            const std::string &key = item.first;
            const boost::property_tree::ptree &tree = item.second;
            for (auto &item : tree)
            {
                std::string shop = item.first;
                int value = item.second.get_value<int>();
                m_paddings[enum_conversion(shop)] = value;
            }
            if (item.first == "conf_thres")
                m_threshold = item.second.get_value<int>();
        }
    }

    receipt::shop configuration::enum_conversion(std::string shop)
    {
        return m_shops_enum[shop];
    }

    std::vector<std::string> configuration::get_shops() const
    {
        return m_shops;
    }

    std::map<receipt::shop, int> configuration::get_paddings() const
    {
        return m_paddings;
    }

    std::vector<std::string> configuration::get_filters() const
    {
        return m_filters;
    }

    configuration::easyocr configuration::get_easyocr() const
    {
        return m_easyocr;
    }

    int configuration::get_threshold() const
    {
        return m_threshold;
    }
}
