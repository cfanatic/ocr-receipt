#include <gtest/gtest.h>
#include "recognition.hpp"

class receipt_test : public ::testing::Test
{
public:
    receipt_test(){};
    virtual ~receipt_test(){};

    static void SetUpTestSuite();
    static void TearDownTestSuite();

    virtual void SetUp(void){};
    virtual void TearDown(void){};

    static ocr::receipt *m_r1;
    static ocr::receipt *m_r2;
};

ocr::receipt *receipt_test::m_r1 = nullptr;
ocr::receipt *receipt_test::m_r2 = nullptr;

void receipt_test::SetUpTestSuite()
{
    m_r1 = new ocr::receipt("../misc/input/receipt_1.jpg");
    m_r2 = new ocr::receipt("../misc/input/receipt_2.jpg");
    m_r1->init();
    m_r2->init();
}

void receipt_test::TearDownTestSuite()
{
    delete m_r1;
    delete m_r2;
}

TEST_F(receipt_test, extract_1)
{
    auto detections = m_r1->extract();
    auto &text = detections[3].text;
    EXPECT_EQ(text, "Geg. EC-Cash EUR 0, 58");
}

TEST_F(receipt_test, extract_2)
{
    auto detections = m_r2->extract();
    auto &text = detections[1].text;
    EXPECT_EQ(text, "MILCHREIS KIRSCH 0,29 B");
}
