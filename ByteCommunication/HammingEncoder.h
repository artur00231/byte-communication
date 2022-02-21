#pragma once

#include <cstdint>

class HammingEncoder
{
public:
	struct EncodeResult
	{
		std::uint8_t high;
		std::uint8_t low;
	};

	struct DecodeResult
	{
		enum class Result : std::uint8_t
		{
			CORRECT, CORRECTED, UNCORRECTABLE
		};

		std::uint8_t byte;
		Result result;
	};

	static constexpr std::uint8_t generator_matrix[] =
	{
		0b1101,
		0b1011,
		0b1000,
		0b0111,
		0b0100,
		0b0010,
		0b0001,
		0b1110
	};

	static constexpr std::uint8_t check_matrix[] =
	{
		0b1010101,
		0b0110011,
		0b0001111
	};

	static constexpr std::uint8_t check_matrix_T[] =
	{
		7, /*valid -->*/ 3, 5, 1, 6, 2, 4, 0
	};

	static DecodeResult decodeByte(std::uint8_t high, std::uint8_t low) noexcept;
	static EncodeResult encodeByte(std::uint8_t byte) noexcept;

private:
	//4 bit value!
	static std::uint8_t encode(std::uint8_t value) noexcept;

	//4 bit value
	//OUT: HIGH - status (0, 1, 2), LOW - value
	static std::uint8_t decode(std::uint8_t value) noexcept;
};

