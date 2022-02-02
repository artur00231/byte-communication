#include "pch.h"
#include "../ByteCommunication/SStreamReader.h"

TEST(SStreamReaderTest, TEST1) 
{
	SStreamReader reader{};

	EXPECT_FALSE(reader.isAvaiable());

	reader.putByte('\10');

	EXPECT_TRUE(reader.isAvaiable());
	EXPECT_EQ('\10', reader.getByte());
	EXPECT_FALSE(reader.isAvaiable());
}

TEST(SStreamReaderTest, TEST2) 
{
	SStreamReader reader{};

	EXPECT_FALSE(reader.isAvaiable());

	reader.putByte('\10');
	reader.putByte('\11');
	reader.putByte('\12');
	reader.putByte('\13');

	EXPECT_TRUE(reader.isAvaiable());
	EXPECT_EQ('\10', reader.getByte());
	EXPECT_EQ('\11', reader.getByte());
	EXPECT_EQ('\12', reader.getByte());
	EXPECT_EQ('\13', reader.getByte());
	EXPECT_FALSE(reader.isAvaiable());
}