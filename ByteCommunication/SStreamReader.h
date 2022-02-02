#pragma once

#include <sstream>
#include <memory>
#include <cinttypes>

class SStreamReader
{
public:
	SStreamReader();
	SStreamReader(const SStreamReader&);
	SStreamReader(SStreamReader&&) noexcept;
	SStreamReader& operator=(const SStreamReader&);
	SStreamReader& operator=(SStreamReader&&) noexcept;

	std::uint8_t getByte();
	bool isAvaiable() const noexcept;

	void putByte(std::uint8_t byte);

private:
	std::shared_ptr<std::stringstream> stream{};
};

