#pragma once

#include <ncurses.h>

#include <cpu.hpp>
#include <memory.hpp>

namespace mboy {
class Debugger {
public:
	Debugger(CPU &cpu, Memory &mem);
	~Debugger();

	void step();

	CPU cpu_;
	Memory mem_;

private:
	void init_ui();
	void draw_title(WINDOW *win, std::string title);
	void draw_borders(WINDOW *win, std::string title);

};

} /* namespace */
