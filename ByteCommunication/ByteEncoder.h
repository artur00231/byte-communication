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

class ByteDecoder
{
public:
	struct Result
	{
		std::uint8_t data{};
		bool isEmpty{ true };
	};

	Result pushByte(std::uint8_t byte) noexcept;

	/// This will reset Decoder!
	/// Returns true if decoder was closed in valid state
	bool flush() noexcept;

	/// Returns true if decoder can be closed in valid state
	bool isEndValid() const noexcept {
		return counter == 0 && data == 0;
	}

	bool isEmpty() const noexcept {
		return data_size == 0;
	}

protected:

private:
	std::uint8_t data{};
	std::uint8_t data_size : 4{};
	std::uint8_t counter : 4{};
};

/// ByteEncoder size constrain
static_assert(sizeof(ByteEncoder) == 2);