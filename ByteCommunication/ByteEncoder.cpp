#include "ByteEncoder.h"

#include <cassert>

ByteEncoder::Result ByteEncoder::pushByte(std::uint8_t byte) noexcept
{
	Result result{};

	result.data = data;
	data = 0;
	std::uint8_t shift = data_size;
	data_size = 0;

	for (std::uint8_t i = 0; i < 8; i++)
	{
		std::uint8_t value = (byte & 0x80) >> 7;
		byte <<= 1;

		if (value == 0)
		{
			shitInto(result, shift, value);
			counter = 0;
		}
		else
		{
			counter++;
			shitInto(result, shift, value);
		}

		if (counter == 3)
		{
			counter = 0;
			shitInto(result, shift, 0);
		}
	}

	if (data_size == 8)
	{
		result.additional_data = data;
		result.additional = true;
		data = 0;
		data_size = 0;
	}

	return result;
}

ByteEncoder::Result ByteEncoder::flush() noexcept
{
	counter = 0;

	if (isEmpty())
	{
		return {};
	}

	Result result{};

	result.additional_data = data;
	result.additional = true;
	data = 0;
	data_size = 0;

	return result;
}

void ByteEncoder::shitInto(ByteEncoder::Result& result, std::uint8_t& shift, std::uint8_t value)
{
	if (shift < 8)
	{
		result.data |= (value << (7 - shift));
		shift++;
	}
	else
	{
		if (data_size == 8)
		{
			//Force data to result

			result.additional = true;
			result.additional_data = data;

			data_size = 0;
			data = 0;
		}

		data |= (value << (7 - data_size));
		data_size++;
	}
}

ByteDecoder::Result ByteDecoder::pushByte(std::uint8_t byte) noexcept
{
	Result result{};

	for (std::uint8_t i = 0; i < 8; i++)
	{
		std::uint8_t value = (byte & 0x80) >> 7;
		byte <<= 1;

		if (counter == 3)
		{
			counter = 0;
			continue;
		}

		if (value == 0)
		{
			counter = 0;
		}
		else
		{
			counter++;
		}

		data |= (value << (7 - data_size));
		data_size++;
		if (data_size == 8)
		{
			result.data = data;
			result.isEmpty = false;
			data = 0;
			data_size = 0;
		}
	}

	return result;
}

bool ByteDecoder::flush() noexcept
{
	auto to_return = isEndValid();

	data = 0;
	data_size = 0;
	counter = 0;

	return to_return;
}
