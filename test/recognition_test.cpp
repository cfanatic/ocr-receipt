#include <gtest/gtest.h>
#include "recognition.hpp"

class receipt_test : public ::testing::Test
{
public:
    receipt_test(){};
    virtual ~receipt_test(){};

    virtual void SetUp();
    virtual void TearDown();

    ocr::receipt *m_pt;
};

void receipt_test::SetUp()
{
    m_pt = new ocr::receipt("../misc/input/receipt_2.jpg");
}

void receipt_test::TearDown()
{
    delete m_pt;
}

TEST_F(receipt_test, extract)
{
    m_pt->init();
    auto detections = m_pt->extract();
    auto &text = detections[1].text;
    EXPECT_EQ(text, "MILCHREIS KIRSCH 0,29 B");
}
