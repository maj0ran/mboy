#pragma once

#include <common.hpp>

namespace mboy {

class Memory {
public:
	Memory();
	~Memory() = default;

	u8 read(u16 addr) const;
	void write(u16 addr, u8 val);

	u8 &operator[](u16 addr);


private:
	u8 mem[64 kB];
};


} /* namespace */



