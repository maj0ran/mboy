#include <curses.h>
#include <unistd.h>
#include <iostream>

#include <debugger.hpp>


namespace mboy {

Debugger::Debugger(CPU &cpu, Memory &mem) : cpu_(cpu), mem_(mem)
{
	initscr();
//	cbreak();
	noecho();
	curs_set(false);

	init_ui();
	sleep(3);
}

Debugger::~Debugger()
{
	endwin();
}

void Debugger::step()
{
	cpu_.exec();
}

void Debugger::init_ui()
{
	int root_x, root_y;

	getmaxyx(stdscr, root_y, root_x);
	WINDOW *data = derwin(stdscr, root_y, root_x * 1/3, 0, 0);
	WINDOW *pc = derwin(stdscr, root_y, root_x * 1/3, 0, data->_maxx);
	WINDOW *reg = derwin(data, root_y / 2, data->_maxx + 1, 0, 0);
	WINDOW *mem = derwin(data, root_y / 2, data->_maxx + 1, reg->_maxy + 1, 0);
	draw_borders(stdscr, "");
	draw_borders(pc, "Instructions");
	draw_borders(data, "");
	draw_borders(mem, "Memory");
	draw_borders(reg, "Registers");
//	draw_title(reg, "Registers");

	refresh();

}
void Debugger::draw_title(WINDOW *win, std::string title)
{
	mvwprintw(win, 0, 2, title.c_str());
}
void Debugger::draw_borders(WINDOW *win, std::string title)
{
	int x, y, i;
	getmaxyx(win, y, x);
	// 4 corners
	mvwprintw(win, 0, 0, "+");
	mvwprintw(win, y - 1, 0, "+");
	mvwprintw(win, 0, x - 1, "+");
	mvwprintw(win, y - 1, x - 1, "+");

	// sides
	for (i = 1; i < (y - 1); i++) {
		mvwprintw(win, i, 0, "|");
		mvwprintw(win, i, x - 1, "|");
	}

	// top and bottom
	for (i = 1; i < (x - 1); i++) {
		mvwprintw(win, 0, i, "-");
		mvwprintw(win, y - 1, i, "-");
	}
	if (!title.empty())
		draw_title(win, title);
}

}

