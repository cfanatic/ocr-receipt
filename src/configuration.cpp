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
            m_shops.push_back(shop.second.data());
        }
        for (boost::property_tree::ptree::value_type &padding : m_config.get_child("paddings"))
        {
            std::string shop = padding.first;
            int value = padding.second.get_value<int>();
            m_paddings[shop] = value;
        }
        for (boost::property_tree::ptree::value_type &filter : m_config.get_child("filters"))
        {
            m_filters.push_back(filter.second.data());
        }
    }

    std::vector<std::string> configuration::get_shops()
    {
        return m_shops;
    }

    std::map<std::string, int> configuration::get_paddings()
    {
        return m_paddings;
    }

    std::vector<std::string> configuration::get_filters()
    {
        return m_filters;
    }
}
