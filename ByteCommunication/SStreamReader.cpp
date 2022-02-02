#include "SStreamReader.h"

SStreamReader::SStreamReader()
{
	stream = std::make_shared<std::stringstream>(std::stringstream{});
}

SStreamReader::SStreamReader(const SStreamReader& sstream) : stream{ sstream.stream }
{
}

SStreamReader::SStreamReader(SStreamReader&& sstream) noexcept : stream{ std::move(sstream.stream) }
{
}

SStreamReader& SStreamReader::operator=(const SStreamReader& sstream)
{
	stream = sstream.stream;
	return *this;
}

SStreamReader& SStreamReader::operator=(SStreamReader&& sstream) noexcept
{
	stream = std::move(sstream.stream);
	return *this;
}

std::uint8_t SStreamReader::getByte()
{
	std::uint8_t byte;
	stream->read(reinterpret_cast<char*>(&byte), 1);
	return byte;
}

bool SStreamReader::isAvaiable() const noexcept
{
	return stream->rdbuf()->in_avail();
}

void SStreamReader::putByte(std::uint8_t byte)
{
	stream->write(reinterpret_cast<char*>(&byte), 1);
}
