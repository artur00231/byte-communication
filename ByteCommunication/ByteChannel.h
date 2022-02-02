#pragma once

#include <deque>
#include <array>
#include <memory>
#include <cinttypes>

class ByteChannel
{
public:
	enum class IOPoint { P1 = 0, P2 = 1 };

	ByteChannel() = default;
	ByteChannel(const ByteChannel&) = delete;
	ByteChannel(ByteChannel&&) noexcept = default;
	ByteChannel& operator=(const ByteChannel&) = delete;
	ByteChannel& operator=(ByteChannel&&) noexcept = default;
	~ByteChannel() = default;

	std::uint8_t getByte(IOPoint target);
	bool isAvaiable(IOPoint target) const noexcept;

	void putByte(std::uint8_t byte, IOPoint source);

private:
	std::array<std::deque<std::uint8_t>, 2> data;
};

