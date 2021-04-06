#pragma once

#include <common.hpp>

namespace mboy {

class Memory {
public:
	Memory() = default;
	~Memory() = default;

	template<u16 addr>
	u8 read() { return mem[addr]; };

	template<u16 addr>
	void write(u8 val) { mem[addr] = val; }

private:
	u8 mem[64 kB];

}; /* namespace */



