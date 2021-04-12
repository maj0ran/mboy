#!/usr/bin/python3

import os
import re

class Instruction():
    def __init__(self, name : str, opcode : int):
        self.name = name
        self.opcode = opcode

    def gen_object(self) -> str:
        object_code = F"\topcode[0x{self.opcode}] = CPU::Instruction(\"{self.name}\", 0x{self.opcode}, &CPU::{self.name});"
        return object_code


dirname = os.path.dirname(__file__)
filename = os.path.join(dirname, '../include/cpu.hpp')

start: str = "/** CPU Instructions **/"
end: str = "private:"

pattern = "void .+\/\/\s*0x.{2}"
regex = re.compile(pattern)

f = open(filename)

for l in f:
    if start in l:
        break

instructions = []

for l in f:
    if l == '\n':
        continue

    if re.search(regex, l):
        # remove tabs and newline in the string.
        # But also remove all spaces, this makes the
        # extraction of the fields easier
        line = re.sub('[\t\n\s]', '', l)

        # name of the function
        name_begin = line.find("d", 0, len(line)) + 1
        name_end = line.find("(", name_begin, len(line))
        name = line[name_begin:name_end]

        opcode_begin = line.find("0x", 0, len(line))
        if opcode_begin != -1:
            opcode = line[opcode_begin + 2:]

        i = Instruction(name, opcode)
        instructions.append(i)
    if end in l:
        break

f.close()

dirname = os.path.dirname(__file__)
filename = os.path.join(dirname, '../src/cpu_opcode_init.cpp')

f = open(filename, "w")

cpu_opcode_init_file: str = ""
cpu_opcode_init_file += "#include <cpu.hpp>\n\n" \
                        "void mboy::CPU::init_opcodes()\n" \
                        "{\n"

for i in instructions:
    cpu_opcode_init_file += i.gen_object() + "\n"

cpu_opcode_init_file += "}"

f.write(cpu_opcode_init_file)
f.close()
