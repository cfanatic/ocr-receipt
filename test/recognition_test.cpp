#include <gtest/gtest.h>
#include "recognition.hpp"

class receipt_test : public ::testing::Test
{
public:
    receipt_test(){};
    virtual ~receipt_test(){};

    static void SetUpTestSuite();
    static void TearDownTestSuite();

    virtual void SetUp(void);
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
}

void receipt_test::TearDownTestSuite()
{
    delete m_r1;
    delete m_r2;
}

void receipt_test::SetUp(void)
{
    m_r1->init();
    m_r1->preprocess();
    m_r2->init();
    m_r2->preprocess();
}

TEST_F(receipt_test, extract_1)
{
    auto detections = m_r1->extract();
    EXPECT_EQ(detections[0].text, "EUR");
    EXPECT_EQ(detections[1].text, "MILCHREIS SCHOKO 0,29 B");
    EXPECT_EQ(detections[2].text, "MILCHREIS KIRSCH 0,29 B");
    EXPECT_EQ(detections[3].text, "Geg. EC-Cash EUR 0, 58");
}

TEST_F(receipt_test, extract_2)
{
    auto detections = m_r2->extract();
    EXPECT_EQ(detections[0].text, "EUR");
    EXPECT_EQ(detections[1].text, "814373 Konfitüre Extra 1,29 A");
    EXPECT_EQ(detections[2].text, "44718 Feine Kleinkuchen 1,79 A");
    EXPECT_EQ(detections[3].text, "60819 Erdbeeren 5009 0,99 A");
    EXPECT_EQ(detections[4].text, "814989 Spargel grün 4009 2,99 A");
}
