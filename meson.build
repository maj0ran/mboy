# SPDX-License-Identifier: GPL-2.0-only

project	('myboy', 'cpp',
	version : '0.1.0',
	default_options : ['cpp_std=c++20',
			   'warning_level=1',
			   'werror=true',
			  ]
	)

C = 'clang'

add_global_arguments('-Wno-pedantic',
		     language : 'cpp')

ncurses_dep = dependency('curses')

src = ['src/main.cpp',
       'src/cpu.cpp',
	   'src/cpu_opcode_init.cpp',
	   'src/debugger.cpp',
       'src/memory.cpp',
      ]

incdir = include_directories('include')

executable('myboy',
	   sources: src,
	   include_directories : incdir,
	   dependencies : ncurses_dep,
	   )

