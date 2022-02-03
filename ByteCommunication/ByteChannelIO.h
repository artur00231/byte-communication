#pragma once

#include "ByteChannel.h"

class ByteChannelIO
{
public:
	ByteChannelIO(ByteChannel& channel, ByteChannel::IOPoint point) : channel{ channel }, point { point } {}
	ByteChannelIO(const ByteChannelIO&) = delete;
	ByteChannelIO(ByteChannelIO&&) = default;
	ByteChannelIO& operator=(ByteChannelIO&&) = default;
	ByteChannelIO& operator=(const ByteChannelIO&) = delete;

	void write(std::uint8_t byte) {
		channel.get().putByte(byte, point);
	}

	std::uint8_t read() {
		return channel.get().getByte(point);
	}

	bool isAvaiable() const noexcept {
		return channel.get().isAvaiable(point);
	}

private:
	std::reference_wrapper<ByteChannel> channel;
	ByteChannel::IOPoint point;
};

