#include <boost/filesystem.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include "engine_easyocr.hpp"

class engine_easyocr_test : public ::testing::Test
{
public:
    engine_easyocr_test() = default;
    virtual ~engine_easyocr_test() = default;

    static void SetUpTestSuite();
    static void TearDownTestSuite();

    virtual void SetUp(){};
    virtual void TearDown(){};

    static ocr::engine_easyocr *m_engine_1;
    static ocr::engine_easyocr *m_engine_2;
};

ocr::engine_easyocr *engine_easyocr_test::m_engine_1 = nullptr;
ocr::engine_easyocr *engine_easyocr_test::m_engine_2 = nullptr;

void engine_easyocr_test::SetUpTestSuite()
{
    boost::filesystem::path path_rel_1("../misc/input/receipt_1.jpg"), path_rel_2("../misc/input/receipt_2.jpg");
    std::string path_abs;
    path_abs = boost::filesystem::canonical(path_rel_1).string();
    m_engine_1 = new ocr::engine_easyocr(path_abs);
    m_engine_1->init();
    path_abs = boost::filesystem::canonical(path_rel_2).string();
    m_engine_2 = new ocr::engine_easyocr(path_abs);
    m_engine_2->init();
}

void engine_easyocr_test::TearDownTestSuite()
{
    delete m_engine_1;
    delete m_engine_2;
}

TEST_F(engine_easyocr_test, receipt_1)
{
    std::vector<std::string> articles_gt = {
        "MILCHREIS SCHOKO 0, 29 B",
        "MILCHREIS KIRSCH 0, 29 B",
    };
    m_engine_1->set_bounding_box(100, 116, 2408, 160);
    EXPECT_EQ(m_engine_1->text(), articles_gt[0]);
    m_engine_1->set_bounding_box(98, 237, 2410, 159);
    EXPECT_EQ(m_engine_1->text(), articles_gt[1]);
}

TEST_F(engine_easyocr_test, receipt_2)
{
    std::vector<std::string> articles_gt = {
        "44718 Feine Kleinkuchen 1,79 A",
        "60819 Erdbeeren 500g 0,99 A",
    };
    m_engine_2->set_bounding_box(120, 173, 1227, 104);
    EXPECT_EQ(m_engine_2->text(), articles_gt[0]);
    m_engine_2->set_bounding_box(116, 239, 1237, 106);
    EXPECT_EQ(m_engine_2->text(), articles_gt[1]);
}
