#include <gtest/gtest.h>
#include "configuration.hpp"
#include "recognition.hpp"

class receipt_test : public ::testing::Test
{
public:
    receipt_test() = default;
    virtual ~receipt_test() = default;

    static void SetUpTestSuite();
    static void TearDownTestSuite();

    virtual void SetUp(){};
    virtual void TearDown(){};

    static ocr::receipt *m_r1;
    static ocr::receipt *m_r2;
    static ocr::receipt *m_r3;
};

ocr::receipt *receipt_test::m_r1 = nullptr;
ocr::receipt *receipt_test::m_r2 = nullptr;
ocr::receipt *receipt_test::m_r3 = nullptr;

void receipt_test::SetUpTestSuite()
{
    ocr::config.load("../misc/config.json");
    m_r1 = new ocr::receipt("../misc/input/receipt_1.jpg");
    m_r2 = new ocr::receipt("../misc/input/receipt_2.jpg");
    m_r3 = new ocr::receipt("../misc/input/receipt_3.jpg");
    m_r1->init();
    m_r1->preprocess();
    m_r2->init();
    m_r2->preprocess();
    m_r3->init();
    m_r3->preprocess();
}

void receipt_test::TearDownTestSuite()
{
    delete m_r1;
    delete m_r2;
    delete m_r3;
}

TEST_F(receipt_test, receipt_1)
{
    std::vector<ocr::receipt::article> articles_gt = {
        ocr::receipt::article{"Milchreis Schoko", 0.29},
        ocr::receipt::article{"Milchreis Kirsch", 0.29},
    };
    m_r1->preprocess();
    auto detections = m_r1->extract();
    auto articles = m_r1->process(detections);
    EXPECT_EQ(m_r1->get_shop(), ocr::receipt::shop::unknown);
    EXPECT_EQ(articles, articles_gt);
}

TEST_F(receipt_test, receipt_2)
{
    std::vector<ocr::receipt::article> articles_gt = {
        ocr::receipt::article{"Konfitüre Extra", 1.29},
        ocr::receipt::article{"Feine Kleinkuchen", 1.79},
        ocr::receipt::article{"Erdbeeren 5009", 0.99},
        ocr::receipt::article{"Spargel Grün 4009", 2.99},
    };
    m_r2->preprocess();
    auto detections = m_r2->extract();
    auto articles = m_r2->process(detections);
    EXPECT_EQ(m_r2->get_shop(), ocr::receipt::shop::unknown);
    EXPECT_EQ(articles, articles_gt);
}

TEST_F(receipt_test, receipt_3)
{
    std::vector<ocr::receipt::article> articles_gt = {
        ocr::receipt::article{"Wrigleys Extra I.", 2.25},
        ocr::receipt::article{"Allwetter-Kiebeb.", 3.99},
        ocr::receipt::article{"Grieche Pur 4x1509g", 1.29},
        ocr::receipt::article{"Heidelbeeren 3009", 2.99},
        ocr::receipt::article{"Torffr. Blumenerde", 1.99},
        ocr::receipt::article{"Lacchwiben", 0.69},
        ocr::receipt::article{"Mandarinen Kg", 1.59},
        ocr::receipt::article{"Kerryg Orig 309", 1.39},
        ocr::receipt::article{"Kerryg Orig 5090", 1.39},
    };
    m_r3->preprocess();
    auto detections = m_r3->extract();
    auto articles = m_r3->process(detections);
    EXPECT_EQ(m_r3->get_shop(), ocr::receipt::shop::aldi);
    EXPECT_EQ(articles, articles_gt);
}
