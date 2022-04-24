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
            m_shops[shop.second.data()] = static_cast<ocr::receipt::shop>(i);
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
    }

    receipt::shop configuration::enum_conversion(std::string shop)
    {
        return m_shops[shop];
    }

    std::vector<std::string> configuration::get_shops()
    {
        std::vector<std::string> shops;
        std::transform(m_shops.begin(), m_shops.end(), std::back_inserter(shops), [](std::pair<std::string, receipt::shop> s)
                       { return s.first; });
        return shops;
    }

    std::map<receipt::shop, int> configuration::get_paddings()
    {
        return m_paddings;
    }

    std::vector<std::string> configuration::get_filters()
    {
        return m_filters;
    }
}
