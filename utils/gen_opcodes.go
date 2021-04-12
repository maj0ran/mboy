package main

import (
	"bufio"
	"fmt"
//	"io"
//	"io/ioutil"
	"os"
	"path/filepath"
	"regexp"
	"strconv"
	"strings"
)

func check(e error) {
	if e!= nil {
		panic(e)
	}
}

func main() {
	filename, _ := filepath.Abs("../src/cpu.cpp")
	file, err := os.Open(filename)

	check(err)

	var func_head = regexp.MustCompile(`^void CPU::.+()$`)
	var func_end = regexp.MustCompile(`^} // 0x.+`)
	var func_one_arg = regexp.MustCompile(`.*read_pc().*`)
	var func_two_arg = regexp.MustCompile(`.*read16_pc().*`)

	var instructions []string
	instructions = append(instructions, "#include <cpu.hpp>\n")
	instructions = append(instructions, "\n")
	instructions = append(instructions, "void mboy::CPU::init_opcodes()\n")
	instructions = append(instructions, "{\n")
	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanLines)

	for scanner.Text() != "********** Opcodes of the Gameboy CPU **********" {
		scanner.Scan()
	}
	var func_name string
	var num_args int = 0
	var opcode uint64

	var instr_cotr string

	for scanner.Scan() {
		var line string = scanner.Text()

		// function name
		if func_head.MatchString(line) {
			var a int = strings.Index(line, ":") + 2
			var e int = strings.Index(line, "(")
			func_name = line[a:e]
			for !func_end.MatchString(line) {
				scanner.Scan()
				line = scanner.Text()
				if func_one_arg.MatchString(line) {
					num_args = 1
					break
				}
				if func_two_arg.MatchString(line) {
					num_args = 2
					break
				}
				num_args = 0
			}
			for !func_end.MatchString(line) {
				scanner.Scan()
				line = scanner.Text()
			}
			opcode, _ = strconv.ParseUint(strings.ReplaceAll(line[7:], " ", ""), 16, 16)
			instr_cotr = fmt.Sprintf(
				"\topcode[0x%X] = CPU::Instruction(\"%s\", 0x%X, &CPU::%s, %d);\n", opcode, func_name, opcode, func_name, num_args)
			instructions = append(instructions, instr_cotr)
		}
	}
	instructions = append(instructions, "}")
	file.Close()

	/* write file */
	filename, _ = filepath.Abs("../src/cpu_opcode_init.cpp")
	file, err = os.Create(filename)
	check(err)

	for _, s := range instructions {
		file.WriteString(s)
}
	file.Sync()
	file.Close()
}
