#include "configuration.hpp"
#include "receipt.hpp"

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
    po::options_description od("Options");
    od.add_options()("config,c", po::value<std::string>(), "set configuration file")("image,i", po::value<std::string>(), "set input image");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, od), vm);
    po::notify(vm);

    if (!vm.count("config"))
    {
        std::cerr << std::endl
                  << "Error: Provide path to configuration file" << std::endl
                  << std::endl;
        std::cerr << od << std::endl;
        return -1;
    }
    if (!vm.count("image"))
    {
        std::cerr << std::endl
                  << "Error: Provide path to input image" << std::endl
                  << std::endl;
        std::cerr << od << std::endl;
        return -1;
    }

    const std::string &path_c = vm["config"].as<std::string>();
    const std::string &path_i = vm["image"].as<std::string>();

    ocr::config.load(path_c);

    ocr::receipt r(path_i);
    r.init();
    r.preprocess();
    auto detections = r.extract();
    auto articles = r.process(detections);
    for (const auto &d : detections)
        std::cout << d;
    for (const auto &a : articles)
        std::cout << a;
    r.overlay(detections);

    return 0;
}
