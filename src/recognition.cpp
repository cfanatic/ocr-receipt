#include "recognition.hpp"

namespace ocr {

    receipt::receipt()
    {
    }

    receipt::~receipt()
    {
    }

    void receipt::extract()
    {
        Pix *image = pixRead("/src/ocr/misc/input/receipt_1.jpg");
        tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

        if (api->Init(NULL, "deu"))
        {
            std::cerr << "Could not initialize Tesseract" << std::endl;
            return;
        }

        api->SetImage(image);
        char *text = api->GetUTF8Text();
        std::cout << text << std::endl;

        api->End();
        pixDestroy(&image);
        delete api;
        delete[] text;
    }

} // namespace ocr
