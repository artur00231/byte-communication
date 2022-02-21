#pragma once

template<typename IS_AVAIABLE_FUN_t,
	typename READ_FUN_t,
	typename WRITE_FUN_t>
class Communicator
{
public:
	Communicator(IS_AVAIABLE_FUN_t&& is_avaiable, READ_FUN_t&& read, WRITE_FUN_t&& write);

private:
	IS_AVAIABLE_FUN_t is_avaiable;
	READ_FUN_t read;
	WRITE_FUN_t write;
};
