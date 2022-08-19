#include "configuration.hpp"
#include "engine.hpp"
#include "receipt.hpp"

namespace po = boost::program_options;
namespace pt = boost::property_tree;

int main(int argc, char *argv[])
{
    po::options_description od("Options");
    od.add_options()("config,c", po::value<std::string>(), "set configuration file")("image,i", po::value<std::string>(), "set input image")("json,j", "set output as JSON");
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
    auto detections = r.extract(ocr::engine::name::easyocr);
    auto articles = r.process(detections);
    r.overlay(detections);

    if (!vm.count("json"))
    {
        std::cout << "Shop: " << r.get_shop() << std::endl;
        for (const auto &d : detections)
            std::cout << d;
        for (const auto &a : articles)
            std::cout << a;
    }
    else
    {
        pt::ptree root, arr;
        std::stringstream results;
        for (const auto &a : articles)
        {
            pt::ptree item;
            item.put("Name", boost::format("%s") % a.name);
            item.put("Price", boost::format("%.2f") % a.price);
            arr.push_back(std::make_pair("", item));
        }
        root.put_child("Articles", arr);
        pt::write_json(results, root);
        std::cout << results.str() << std::endl;
    }

    return 0;
}
