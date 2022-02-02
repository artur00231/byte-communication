#include "HammingEncoder.h"

HammingEncoder::DecodeResult HammingEncoder::decodeByte(std::uint8_t high, std::uint8_t low) noexcept
{
	DecodeResult result{};
	result.result = DecodeResult::Result::CORRECT;

	std::uint8_t decoded = decode(low);
	if ((decoded >> 4) == 1)
	{
		result.result = DecodeResult::Result::CORRECTED;
	}
	else if ((decoded >> 4) == 2)
	{
		result.result = DecodeResult::Result::UNCORRECTABLE;
		return result;
	}

	result.byte = decoded & 0x0F;

	decoded = decode(high);
	if ((decoded >> 4) == 1)
	{
		result.result = DecodeResult::Result::CORRECTED;
	}
	else if ((decoded >> 4) == 2)
	{
		result.result = DecodeResult::Result::UNCORRECTABLE;
		return result;
	}

	result.byte |= (decoded & 0x0F) << 4;

	return result;
}

HammingEncoder::EncodeResult HammingEncoder::encodeByte(std::uint8_t byte) noexcept
{
	return EncodeResult{ .high = encode(byte >> 4), .low = encode(byte & 0x0F) };
}

std::uint8_t HammingEncoder::encode(std::uint8_t value) noexcept
{
	std::uint8_t encoded{};
	for (std::uint8_t i = 0; i < 8; i++)
	{
		encoded <<= 1;
		encoded |= ((generator_matrix[i] >> 3) & (value >> 3)) ^ (((generator_matrix[i] >> 2) & 1) & ((value >> 2) & 1)) ^
			(((generator_matrix[i] >> 1) & 1) & ((value >> 1) & 1)) ^ ((generator_matrix[i] & 1) & (value & 1));
	}

	return encoded;
}

std::uint8_t HammingEncoder::decode(std::uint8_t value) noexcept
{
	bool parity = (value & 1) == (((value >> 7) & 1) ^ ((value >> 6) & 1) ^ ((value >> 5) & 1) ^ 
		((value >> 4) & 1) ^ ((value >> 3) & 1) ^ ((value >> 2) & 1) ^ ((value >> 1) & 1));

	//Remove parity bite
	value >>= 1;

	std::uint8_t syndrome{};
	for (std::uint8_t i = 0; i < 3; i++)
	{
		syndrome <<= 1;
		syndrome |= ((check_matrix[i] >> 6) & (value >> 6)) ^ (((check_matrix[i] >> 5) & 1) & ((value >> 5) & 1)) ^
			(((check_matrix[i] >> 4) & 1) & ((value >> 4) & 1)) ^ (((check_matrix[i] >> 3) & 1) & ((value >> 3) & 1)) ^
			(((check_matrix[i] >> 2) & 1) & ((value >> 2) & 1)) ^ (((check_matrix[i] >> 1) & 1) & ((value >> 1) & 1)) ^ ((check_matrix[i] & 1) & ((value >> 0) & 1));
	}

	//CORRECT
	std::uint8_t flag{};

	if (syndrome == 0 && parity == true)
	{
		//OK, do nothing
	}
	else if (syndrome == 0 && parity == false)
	{
		//OK, but set falg
		flag = 0x10;
	}
	else if (syndrome != 0 && parity == false)
	{
		//CORRECTED
		flag = 0x10;

		auto move = check_matrix_T[syndrome];

		//Correct the error
		value = (value & (~(1 << check_matrix_T[syndrome]))) | ((~value) & (1 << check_matrix_T[syndrome]));
	}
	else
	{
		//UNCORRECTABLE
		flag = 0x20;
		return flag;
	}

	std::uint8_t decoded{ static_cast<std::uint8_t>(
		((value >> 4) & 1) << 3 | ((value >> 2) & 1) << 2 | ((value >> 1) & 1) << 1 | value & 1
		)
	};

	return decoded | flag;
}
