#include <gtest/gtest.h>
#include "configuration.hpp"

class configuration_test : public ::testing::Test
{
public:
    configuration_test() = default;
    virtual ~configuration_test() = default;

    static void SetUpTestSuite();
    static void TearDownTestSuite();

    virtual void SetUp(){};
    virtual void TearDown(){};
};

void configuration_test::SetUpTestSuite()
{
    ocr::config.load("../misc/config.json");
}

void configuration_test::TearDownTestSuite()
{
}

TEST_F(configuration_test, shops)
{
    std::vector<std::string> shops_gt = {
        "edeka",
        "rewe",
        "aldi",
        "unknown",
    };
    std::vector<std::string> shops = ocr::config.get_shops();
    EXPECT_EQ(shops, shops_gt);
}

TEST_F(configuration_test, filters)
{
    std::vector<std::string> filters_gt = {
        "summe",
        "pfand",
        "leergut",
        "einweg",
    };
    std::vector<std::string> filters = ocr::config.get_filters();
    filters.erase(filters.begin() + 4, filters.end());
    EXPECT_EQ(filters, filters_gt);
}

TEST_F(configuration_test, easyocr)
{
    ocr::configuration::easyocr easyocr_gt = {
        "/tmp",
        "wrapper.py",
        15,
    };
    ocr::configuration::easyocr easyocr = ocr::config.get_easyocr();
    EXPECT_EQ(easyocr, easyocr_gt);
}

TEST_F(configuration_test, tesseract_paddings)
{
    std::map<ocr::receipt::shop, int> paddings_gt = {
        {ocr::receipt::shop::edeka, 6},
        {ocr::receipt::shop::rewe, 20},
        {ocr::receipt::shop::aldi, 18},
        {ocr::receipt::shop::unknown, 20},
    };
    std::map<ocr::receipt::shop, int> paddings = ocr::config.get_paddings();
    EXPECT_EQ(paddings, paddings_gt);
}

TEST_F(configuration_test, tesseract_threshold)
{
    int threshold_gt = 30;
    int threshold = ocr::config.get_threshold();
    EXPECT_EQ(threshold, threshold_gt);
}
