#include <iostream>

#include <bios.hpp>
#include <cpu.hpp>
#include <memory.hpp>

#include <cstring>

using namespace mboy;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
	CPU *cpu = new CPU();
	Memory *mem = new Memory();

	cpu->mem = mem;
	memcpy(cpu->mem, bios, 256);
	std::cout << "VAL: " << std::hex << (int)cpu->mem->read(0x100) << std::endl;
	return 0;
}

