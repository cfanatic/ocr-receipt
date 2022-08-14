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
        for (boost::property_tree::ptree::value_type &shop : m_config.get_child("shops"))
        {
            static int i;
            m_shops.push_back(shop.second.data());
            m_shops_enum[shop.second.data()] = static_cast<ocr::receipt::shop>(i);
            i++;
        }
        for (boost::property_tree::ptree::value_type &padding : m_config.get_child("paddings"))
        {
            std::string shop = padding.first;
            int value = padding.second.get_value<int>();
            m_paddings[enum_conversion(shop)] = value;
        }
        for (boost::property_tree::ptree::value_type &filter : m_config.get_child("filters"))
        {
            m_filters.push_back(filter.second.data());
        }
        for (boost::property_tree::ptree::value_type &easyocr : m_config.get_child("easyocr"))
        {
            if (easyocr.first == "path")
                m_easyocr.path = easyocr.second.get_value<std::string>();
            if (easyocr.first == "offset")
                m_easyocr.offset = easyocr.second.get_value<int>();
        }
        m_threshold = m_config.get<int>("detection_conf_thres", 0);
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
