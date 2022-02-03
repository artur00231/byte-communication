#include "pch.h"

#include "../ByteCommunication/ByteChannelIO.h"


TEST(ByteChannelIO, BasicCommunication1)
{
	ByteChannel channel{};
	ByteChannelIO bIO1{ channel, ByteChannel::IOPoint::P1 };
	ByteChannelIO bIO2{ channel, ByteChannel::IOPoint::P2 };

	EXPECT_FALSE(bIO1.isAvaiable());
	EXPECT_FALSE(bIO2.isAvaiable());

	bIO1.write('\10');

	EXPECT_FALSE(bIO1.isAvaiable());

	bIO2.write('\10');

	EXPECT_TRUE(bIO1.isAvaiable());
	EXPECT_EQ('\10', bIO1.read());
	EXPECT_FALSE(bIO1.isAvaiable());
	EXPECT_TRUE(bIO2.isAvaiable());

}

TEST(ByteChannelIO, BasicCommunication2)
{
	ByteChannel channel{};
	ByteChannelIO bIO1{ channel, ByteChannel::IOPoint::P1 };
	ByteChannelIO bIO2{ channel, ByteChannel::IOPoint::P2 };

	EXPECT_FALSE(bIO1.isAvaiable());
	EXPECT_FALSE(bIO2.isAvaiable());

	bIO2.write('\10');
	bIO2.write('\11');
	bIO2.write('\12');
	bIO2.write('\13');

	EXPECT_TRUE(bIO1.isAvaiable());
	EXPECT_FALSE(bIO2.isAvaiable());
	EXPECT_EQ('\10', bIO1.read());
	EXPECT_EQ('\11', bIO1.read());
	EXPECT_EQ('\12', bIO1.read());
	EXPECT_EQ('\13', bIO1.read());
	EXPECT_FALSE(bIO1.isAvaiable());
	EXPECT_FALSE(bIO2.isAvaiable());
}