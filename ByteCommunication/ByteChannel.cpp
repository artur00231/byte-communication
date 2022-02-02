#include "ByteChannel.h"

std::uint8_t ByteChannel::getByte(IOPoint target)
{
	if (isAvaiable(target))
	{
		auto byte = data.at(static_cast<std::size_t>(target)).front();
		data.at(static_cast<std::size_t>(target)).pop_front();

		return byte;
	}

	//UB
	return 0;
}

bool ByteChannel::isAvaiable(IOPoint target) const noexcept
{
	return !data.at(static_cast<std::size_t>(target)).empty();
}

void ByteChannel::putByte(std::uint8_t byte, IOPoint source)
{
	source = (source == IOPoint::P1) ? IOPoint::P2 : IOPoint::P1;

	data.at(static_cast<std::size_t>(source)).push_back(byte);
}
