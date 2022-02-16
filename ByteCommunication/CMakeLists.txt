add_library(ByteCommunication 
	ByteChannel.cpp 
	ByteChannel.h 
	ByteChannelIO.h 
	DataPacket.h
	HammingEncoder.cpp
	HammingEncoder.h)

target_include_directories(ByteCommunication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})