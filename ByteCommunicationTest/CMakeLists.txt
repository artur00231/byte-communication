include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(tests 
	ByteChannelIOTest.cpp
	ByteChannelTest.cpp
	DataPacketTest.cpp
	HammingEncoderTest.cpp
	ByteEncoderTest.cpp
	pch.h
	pch.cpp
)

target_link_libraries(tests gtest_main ByteCommunication)

add_test(NAME AllTests COMMAND tests)

