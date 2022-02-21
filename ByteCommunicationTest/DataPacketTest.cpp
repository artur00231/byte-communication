#include "pch.h"
#include "../ByteCommunication/DataPacket.h"

#include <array>

//TODO Write more test!
TEST(DataPacket, TEST1)
{
	DataPacket<10> data{};

	//ID
	std::uint8_t ID = 15;
	//LENGTH
	std::uint8_t length = 3;
	//DATA
	std::array<std::uint8_t, 3> arr{ 1, 2, 3 };

	EXPECT_TRUE(data);
	EXPECT_FALSE(data.isValid());
	EXPECT_FALSE(data.isFull());

	auto view1 = data.getView();
	EXPECT_FALSE(view1);

	EXPECT_TRUE(data.push_back(ID));
	EXPECT_TRUE(data);
	EXPECT_FALSE(data.isValid());
	EXPECT_FALSE(data.isFull());
	EXPECT_TRUE(data.push_back(length));
	EXPECT_TRUE(data);
	EXPECT_FALSE(data.isValid());
	EXPECT_FALSE(data.isFull());
	EXPECT_TRUE(data.push_back(arr[0]));
	EXPECT_TRUE(data);
	EXPECT_FALSE(data.isValid());
	EXPECT_FALSE(data.isFull());
	EXPECT_TRUE(data.push_back(arr[1]));
	EXPECT_TRUE(data);
	EXPECT_FALSE(data.isValid());
	EXPECT_FALSE(data.isFull());

	auto view2 = data.getView();
	EXPECT_FALSE(view2);

	EXPECT_TRUE(data.push_back(arr[2]));
	EXPECT_TRUE(data);
	EXPECT_TRUE(data.isValid());
	EXPECT_FALSE(data.isFull());

	auto view3 = data.getView();

	EXPECT_TRUE(view3);
}

TEST(DataPacket, TOO_LONG)
{
	DataPacket<4> data{};

	//ID
	std::uint8_t ID = 15;
	//LENGTH
	std::uint8_t length = 10;
	//DATA
	std::array<std::uint8_t, 10> arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	EXPECT_TRUE(data);
	EXPECT_FALSE(data.isValid());
	EXPECT_FALSE(data.isFull());

	EXPECT_TRUE(data.push_back(ID));
	EXPECT_TRUE(data);
	EXPECT_FALSE(data.isValid());
	EXPECT_FALSE(data.isFull());

	EXPECT_FALSE(data.push_back(length));
	EXPECT_FALSE(data);
	EXPECT_FALSE(data.isValid());
	EXPECT_FALSE(data.isFull());

	for (auto&& x : arr)
	{
		EXPECT_FALSE(data.push_back(x));
	}

	EXPECT_FALSE(data);
	EXPECT_FALSE(data.isValid());
	EXPECT_TRUE(data.isFull());

	auto view3 = data.getView();
	EXPECT_FALSE(view3);
}