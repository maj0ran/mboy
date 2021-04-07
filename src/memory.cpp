#include <memory.hpp>

namespace mboy {

Memory::Memory()
{
}

u8 Memory::read(u16 addr) const
{
	return this->mem[addr];
}

void Memory::write(u16 addr, u8 val) {
	this->mem[addr] = val;
}

u8& Memory::operator[](u16 addr)
{
	return this->mem[addr];
}

} /* namespace */

