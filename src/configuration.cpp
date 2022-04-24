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
    }
}
