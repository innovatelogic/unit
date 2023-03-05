#include "core.h"
#include <gtest/gtest.h>

TEST(Math, Plus)
{
	int a = 3;
	int b = 2;
	EXPECT_EQ(a + b, 5);
}


TEST(Core, onnx)
{
	core::UCore::test_onnx("data/test_training_model_2.onnx");
}