#include "pch.h"
#include "../ByteCommunication/ByteChannel.h"

TEST(SStreamReaderTest, TEST1) 
{
	ByteChannel reader{};

	EXPECT_FALSE(reader.isAvaiable(ByteChannel::IOPoint::P1));

	reader.putByte('\10', ByteChannel::IOPoint::P1);

	EXPECT_FALSE(reader.isAvaiable(ByteChannel::IOPoint::P1));

	reader.putByte('\10', ByteChannel::IOPoint::P2);

	EXPECT_TRUE(reader.isAvaiable(ByteChannel::IOPoint::P1));
	EXPECT_EQ('\10', reader.getByte(ByteChannel::IOPoint::P1));
	EXPECT_FALSE(reader.isAvaiable(ByteChannel::IOPoint::P1));
	EXPECT_TRUE(reader.isAvaiable(ByteChannel::IOPoint::P2));
}

TEST(SStreamReaderTest, TEST2) 
{
	ByteChannel reader{};

	EXPECT_FALSE(reader.isAvaiable(ByteChannel::IOPoint::P1));

	reader.putByte('\10', ByteChannel::IOPoint::P2);
	reader.putByte('\11', ByteChannel::IOPoint::P2);
	reader.putByte('\12', ByteChannel::IOPoint::P2);
	reader.putByte('\13', ByteChannel::IOPoint::P2);

	EXPECT_TRUE(reader.isAvaiable(ByteChannel::IOPoint::P1));
	EXPECT_FALSE(reader.isAvaiable(ByteChannel::IOPoint::P2));
	EXPECT_EQ('\10', reader.getByte(ByteChannel::IOPoint::P1));
	EXPECT_EQ('\11', reader.getByte(ByteChannel::IOPoint::P1));
	EXPECT_EQ('\12', reader.getByte(ByteChannel::IOPoint::P1));
	EXPECT_EQ('\13', reader.getByte(ByteChannel::IOPoint::P1));
	EXPECT_FALSE(reader.isAvaiable(ByteChannel::IOPoint::P1));
	EXPECT_FALSE(reader.isAvaiable(ByteChannel::IOPoint::P2));
}