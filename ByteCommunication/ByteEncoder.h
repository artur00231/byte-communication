#pragma once

#include <cstdint>

class ByteEncoder
{
public:
	struct Result
	{
		std::uint8_t data{};
		bool additional{ false };
		std::uint8_t additional_data{};
	};

	Result pushByte(std::uint8_t byte) noexcept;

	/// Only additional_data will be returned (if additional == true)!
	/// This will reset Encoder!
	Result flush() noexcept;

	bool isEmpty() const noexcept {
		return data_size == 0;
	}

protected:
	void shitInto(Result& result, std::uint8_t& shift, std::uint8_t value);

private:
	std::uint8_t data{};
	std::uint8_t data_size : 4{};
	std::uint8_t counter : 4{};
};

/// ByteEncoder size constrain
static_assert(sizeof(ByteEncoder) == 2);
