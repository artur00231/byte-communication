#include "pch.h"

#include "../ByteCommunication/HammingEncoder.h"

TEST(HammingEncoderTest, TEST1) 
{
	auto value = HammingEncoder::encodeByte('\5');

	auto de_value = HammingEncoder::decodeByte(value.high, value.low);

	EXPECT_EQ('\5', de_value.byte);
	EXPECT_EQ(HammingEncoder::DecodeResult::Result::CORRECT, de_value.result);
}

TEST(HammingEncoderTest, TEST2)
{
	for (int i = 0; i < 255; i++)
	{
		auto to_encode = static_cast<std::uint8_t>(i);
		auto value = HammingEncoder::encodeByte(to_encode);

		auto de_value = HammingEncoder::decodeByte(value.high, value.low);

		EXPECT_EQ(to_encode, de_value.byte);
		EXPECT_EQ(HammingEncoder::DecodeResult::Result::CORRECT, de_value.result);
	}
}

TEST(HammingEncoderTest, TEST3)
{
	for (int i = 0; i < 255; i++)
	{
		auto to_encode = static_cast<std::uint8_t>(i);
		auto value = HammingEncoder::encodeByte(to_encode);

		for (int j = 0; j < 16; j++)
		{
			std::uint16_t data = value.low | value.high << 8;
			data ^= 1 << j;
			auto de_value = HammingEncoder::decodeByte(data >> 8, data & 0xFF);

			EXPECT_EQ(to_encode, de_value.byte);
			EXPECT_EQ(HammingEncoder::DecodeResult::Result::CORRECTED, de_value.result);
		}
	}
}

TEST(HammingEncoderTest, TEST4)
{
	for (int i = 0; i < 255; i++)
	{
		auto to_encode = static_cast<std::uint8_t>(i);
		auto value = HammingEncoder::encodeByte(to_encode);

		for (int j = 0; j < 8; j++)
		{
			for (int k = 8; k < 16; k++)
			{
				std::uint16_t data = value.low | value.high << 8;
				data ^= 1 << j;
				data ^= 1 << k;
				auto de_value = HammingEncoder::decodeByte(data >> 8, data & 0xFF);

				EXPECT_EQ(to_encode, de_value.byte);
				EXPECT_EQ(HammingEncoder::DecodeResult::Result::CORRECTED, de_value.result);
			}
		}
	}
}

TEST(HammingEncoderTest, TEST5)
{
	for (int i = 0; i < 255; i++)
	{
		auto to_encode = static_cast<std::uint8_t>(i);
		auto value = HammingEncoder::encodeByte(to_encode);

		for (int j = 0; j < 8; j++)
		{
			for (int k = 8; k < 15; k++)
			{
				for (int l = k + 1; l < 16; l++)
				{
					std::uint16_t data = value.low | value.high << 8;
					data ^= 1 << j;
					data ^= 1 << k;
					data ^= 1 << l;
					auto de_value = HammingEncoder::decodeByte(data >> 8, data & 0xFF);

					EXPECT_EQ(HammingEncoder::DecodeResult::Result::UNCORRECTABLE, de_value.result);
				}
			}
		}
	}
}

TEST(HammingEncoderTest, TEST6)
{
	for (int i = 0; i < 255; i++)
	{
		auto to_encode = static_cast<std::uint8_t>(i);
		auto value = HammingEncoder::encodeByte(to_encode);

		for (int k = 0; k < 7; k++)
		{
			for (int l = k + 1; l < 8; l++)
			{
				std::uint16_t data = value.low | value.high << 8;
				data ^= 1 << k;
				data ^= 1 << l;
				auto de_value = HammingEncoder::decodeByte(data >> 8, data & 0xFF);

				EXPECT_EQ(HammingEncoder::DecodeResult::Result::UNCORRECTABLE, de_value.result);
			}
		}
	}
}