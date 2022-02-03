#pragma once

#include <cstdint>

/***
* data package: (every field 1 byte)
* -------------------
* |ID               |
* -------------------
* |LENGTH           |
* -------------------
* |DATA + 0         |
* |DATA + 1         |
* |DATA + 2         |
*       *
*       *
*       *
* |DATA + LENGTH - 1|
* -------------------
*/

template<std::uint8_t N>
class DataPacketView;

template<std::uint8_t N>
class DataPacket
{
public:
	static_assert(N >= 2, "Buffer cannot be smaller than 2 bytes!");

	enum class FLAGS : std::uint8_t { OVERFILL = 1, INVALID = 2, PROCESSING = 4 };

	bool good() const noexcept;
	operator bool() const noexcept { return good(); }

	bool isValid() const noexcept;
	bool isFull() const noexcept;

	bool push_back(std::uint8_t byte) noexcept;

	DataPacketView<N> getView() const;

protected:
	std::uint8_t getLength() const noexcept {
		return data[1];
	}

private:
	std::uint8_t data[N];
	std::uint8_t size{};
	std::uint8_t flags{ static_cast<std::uint8_t>(FLAGS::PROCESSING) };
};

template<std::uint8_t N>
class DataPacketView
{
public:
	DataPacketView(const DataPacketView&) = default;
	DataPacketView(DataPacketView&&) = default;

	std::uint8_t getID() const noexcept;
	std::uint8_t getLength() const noexcept;

	bool good() const noexcept;
	operator bool() const noexcept { return good(); }

	const std::uint8_t* getData() const noexcept;
	std::uint8_t operator[](std::uint8_t) const noexcept;

protected:
	DataPacketView(const DataPacket<N>& data) : data{ data } {};

private:
	const DataPacket<N>& data;

	template<std::uint8_t R>
	friend class DataPacket;
};

template<std::uint8_t N>
inline bool DataPacket<N>::isFull() const noexcept
{
	return (size == N);
}

template<std::uint8_t N>
inline bool DataPacket<N>::good() const noexcept
{
	if ((flags & ~(static_cast<std::uint8_t>(FLAGS::PROCESSING))) != 0)
	{
		return false;
	}

	if (size < 2)
	{
		return true;
	}

	return (getLength() <= N - 2);
}

template<std::uint8_t N>
inline bool DataPacket<N>::isValid() const noexcept
{
	return ((flags & static_cast<std::uint8_t>(FLAGS::PROCESSING)) == 0 && good());
}

template<std::uint8_t N>
inline bool DataPacket<N>::push_back(std::uint8_t byte) noexcept
{
	if (isFull())
	{
		flags |= static_cast<std::uint8_t>(FLAGS::OVERFILL);
		return false;
	}

	data[size] = byte;
	size++;
	
	if (size >= 2 && !good())
	{
		return false;
	}

	if (size >= 2 && size == getLength() + 2)
	{
		flags &= ~(static_cast<std::uint8_t>(FLAGS::PROCESSING));
	}

	return true;
}

template<std::uint8_t N>
inline DataPacketView<N> DataPacket<N>::getView() const
{
	return DataPacketView(*this);
}




template<std::uint8_t N>
inline std::uint8_t DataPacketView<N>::getID() const noexcept
{
	return data.data[0];
}

template<std::uint8_t N>
inline std::uint8_t DataPacketView<N>::getLength() const noexcept
{
	return data.data[1];
}

template<std::uint8_t N>
inline bool DataPacketView<N>::good() const noexcept
{
	return data.isValid();
}

template<std::uint8_t N>
inline const std::uint8_t* DataPacketView<N>::getData() const noexcept
{
	return data.data + 2;
}

template<std::uint8_t N>
inline std::uint8_t DataPacketView<N>::operator[](std::uint8_t index) const noexcept
{
	if (index >= getLength())
	{
		return data.data[index + 2];
	}

	return 255;
}
