#include "pch.h"
#include <vector>
#include <numeric>
#include <random>
#include <functional>

#include "../ByteCommunication/ByteEncoder.h"

namespace ByteEncoderTestHelper
{
	static constexpr bool USE_RANDOM{ true };
	static std::uint64_t SEED{ 0 };
	static constexpr std::size_t RANDOM_SIZE{ 4096 };

	[[nodiscard]] bool isValid(std::uint8_t value, int& counter)
	{
		for (int i = 0; i < 8; i++)
		{
			if (((value & 0x80) >> 7) == 0)
			{
				counter = 0;
			}
			else
			{
				counter++;

				if (counter == 4)
				{
					return false;
				}
			}

			value <<= 1;
		}

		return true;
	}

	[[nodiscard]] bool isValid(std::uint8_t value)
	{
		int counter{};
		return isValid(value, counter);
	}

	[[nodiscard]] bool isValid(const std::vector<std::uint8_t>& values)
	{
		int counter{};
		int pos{};
		for (auto&& value : values)
		{
			if (!isValid(value, counter))
			{
				return false;
			}

			pos++;
		}

		return true;
	}
}
TEST(ByteEncoderTest, Internal_isValid)
{
	EXPECT_EQ(true, ByteEncoderTestHelper::isValid(0b00000000));
	EXPECT_EQ(true, ByteEncoderTestHelper::isValid(0b10101010));
	EXPECT_EQ(true, ByteEncoderTestHelper::isValid(0b11101110));
	EXPECT_EQ(true, ByteEncoderTestHelper::isValid(0b01110101));
	EXPECT_EQ(true, ByteEncoderTestHelper::isValid(0b01010111));

	EXPECT_EQ(false, ByteEncoderTestHelper::isValid(0b0111101));
	EXPECT_EQ(false, ByteEncoderTestHelper::isValid(0b1111000));
	EXPECT_EQ(false, ByteEncoderTestHelper::isValid(0b0001111));
	EXPECT_EQ(false, ByteEncoderTestHelper::isValid(0b1111110));
	EXPECT_EQ(false, ByteEncoderTestHelper::isValid(0b1111111));
}

TEST(ByteEncoderTest, Internal_isValid_V)
{
	std::vector<std::uint8_t> data{ 0b00000000, 0b10101010, 0b11101110, 0b01110101, 0b01010111 };
	EXPECT_EQ(true, ByteEncoderTestHelper::isValid(data));

	data = { 0b0001111 };
	EXPECT_EQ(false, ByteEncoderTestHelper::isValid(data));

	data = { 0b0000111, 0b10001011 };
	EXPECT_EQ(false, ByteEncoderTestHelper::isValid(data));

	data = { 0b0000011, 0b11001011 };
	EXPECT_EQ(false, ByteEncoderTestHelper::isValid(data));

	data = { 0b0000001, 0b11100110 };
	EXPECT_EQ(false, ByteEncoderTestHelper::isValid(data));
}

TEST(ByteEncoderTest, ByteEncoderTest1)
{
	ByteEncoder encoder{};

	EXPECT_EQ(encoder.isEmpty(), true);
}

TEST(ByteEncoderTest, ByteEncoderTest2)
{
	ByteEncoder encoder{};
	auto result = encoder.pushByte(0b01010101);

	EXPECT_EQ(result.additional, false);
	EXPECT_EQ(result.data, 0b01010101);
}

TEST(ByteEncoderTest, ByteEncoderTest3)
{
	ByteEncoder encoder{};

	for (std::uint16_t i{}; i <= 0b11111111; i++)
	{
		EXPECT_EQ(encoder.flush().additional, false);

		if (!ByteEncoderTestHelper::isValid(static_cast<std::uint8_t>(i)))
		{
			continue;
		}

		auto result = encoder.pushByte(static_cast<std::uint8_t>(i));

		EXPECT_EQ(result.additional, false);
		EXPECT_EQ(result.data, i);
		ASSERT_EQ(encoder.isEmpty(), true);
	}
}

TEST(ByteEncoderTest, ByteEncoderTest4)
{
	ByteEncoder encoder{};

	encoder.pushByte(0b00001111);

	for (std::uint16_t i{}; i <= 0b11111111; i++)
	{
		if (ByteEncoderTestHelper::isValid(static_cast<std::uint8_t>(i)))
		{
			continue;
		}

		EXPECT_EQ(encoder.flush().additional, true);

		auto result = encoder.pushByte(static_cast<std::uint8_t>(i));

		EXPECT_EQ(result.additional, false);
		EXPECT_NE(result.data, static_cast<std::uint8_t>(i));
		ASSERT_EQ(encoder.isEmpty(), false);
	}
}
TEST(ByteEncoderTest, ByteEncoderTest5)
{
	std::vector<std::uint8_t> data{};

	std::generate_n(std::back_inserter(data), 256, []() {
		static std::uint8_t i{};
		return i++;
		});

	ByteEncoder encoder{};

	EXPECT_FALSE(ByteEncoderTestHelper::isValid(data));

	std::vector<std::uint8_t> data_enc{};

	for (auto&& x : data)
	{
		auto result = encoder.pushByte(x);
		data_enc.push_back(result.data);

		if (result.additional)
		{
			data_enc.push_back(result.additional_data);
		}
	}

	EXPECT_TRUE(ByteEncoderTestHelper::isValid(data_enc));
	EXPECT_GE(data_enc.size(), data.size());
}

TEST(ByteEncoderTest, ByteEncoderTest6)
{
	std::vector<std::uint8_t> data{};

	if constexpr (ByteEncoderTestHelper::USE_RANDOM)
	{
		std::random_device rd{};
		ByteEncoderTestHelper::SEED = rd();

		std::cout << "\tTESTING WITH RANDOM SEED " << ByteEncoderTestHelper::SEED << "\n";
	}
	else
	{
		std::cout << "\tTESTING WITH SEED " << ByteEncoderTestHelper::SEED << "\n";
	}


	std::mt19937_64 gen{ ByteEncoderTestHelper::SEED };
	std::uniform_int_distribution dist{ 0, 255 };
	auto rand = std::bind(dist, gen);

	std::generate_n(std::back_inserter(data), ByteEncoderTestHelper::RANDOM_SIZE, [&rand]() {
		return rand();
		});

	ByteEncoder encoder{};

	EXPECT_FALSE(ByteEncoderTestHelper::isValid(data)) << "WITH SEED = " << ByteEncoderTestHelper::SEED;

	std::vector<std::uint8_t> data_enc{};

	for (auto&& x : data)
	{
		auto result = encoder.pushByte(x);
		data_enc.push_back(result.data);

		if (result.additional)
		{
			data_enc.push_back(result.additional_data);
		}
	}

	EXPECT_TRUE(ByteEncoderTestHelper::isValid(data_enc)) << "WITH SEED = " << ByteEncoderTestHelper::SEED;;
	EXPECT_GE(data_enc.size(), data.size()) << "WITH SEED = " << ByteEncoderTestHelper::SEED;;
}