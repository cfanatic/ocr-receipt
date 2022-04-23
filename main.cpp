#include "recognition.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Error: Provide absolute or relative path to image" << std::endl;
        return -1;
    }
    std::string path(argv[1]);
    ocr::receipt r(path);
    r.init();
    auto detections = r.extract();
    for (const auto &d : detections)
        std::cout << d;
    return 0;
}
