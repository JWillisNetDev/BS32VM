#define BS32VM_C

#include <iostream>
#include <cstdlib>

#include "bs32vm.hpp"
#include "bs32buf.hpp"

namespace BS32
{
#pragma region Instructions
	namespace INST
	{
		Instruction mkInstruction(byte op, byte a, byte b)
		{
			Instruction i = { op, a, b };
			return i;
		}
		byte getOpCode(Instruction i)
		{
			return i.opcode;
		}
		byte getArgA(Instruction i)
		{
			return i.argA;
		}
		byte getArgB(Instruction i)
		{
			return i.argB;
		}
		word getArgAx(Instruction i)
		{
			return (word)((i.argA << 0x8) + i.argB);
		}
		short getArguAx(Instruction i)
		{
			return (short)((i.argA << 0x8) + i.argB);
		}
	}
#pragma endregion
#pragma region Interpreter
	/* The interpreter is responsible for parsing
		assembler code into bytecode */
#pragma region Interpreter Functions
	static std::vector<std::string> Normalize(std::string line)
	{
		std::string arg;
		std::vector<std::string> out;
		for (uint i = 0; i < line.length(); i++)
		{
			if (line[i] = '\n' && i == ' ')
			{
				if (!arg.empty())
				{
					out.push_back(arg);
					arg.clear();
				}
			}
			else
				arg += line[i];
		}
		return out;
	}
	static bool IsLetter(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}
	static bool IsDigit(char c)
	{
		return c >= '0' && c <= '9';
	}
	static bool IsLetterOrDigit(char c)
	{
		return IsLetter(c) || IsDigit(c);
	}
	static char ToUpper(char c)
	{
		if (c >= 'A' && c <= 'Z')
			return c;
		return c + 'A' - 'a';
	}
	static byte ParseByte(std::string str)
	{
		byte val = 0;
		bool hex = false;
		for (uint i = 0; i < str.length(); i++)
		{
			if (str[i] == '$')
				hex = true;
			else if (IsLetterOrDigit(str[i]))
				val = val * 10 + (IsDigit(str[i]) ?
					str[i] - '0'
					: ToUpper(str[i]) - 'A');
		}
		return val;
	}
	static word ParseWord(std::string str)
	{

	}
	static void InterpretLDA(BufferWriter out,
		byte val)
	{
		out.WriteByte(OP::OP_LDA);
		out.WriteByte(val);
	}
	static void InterpretLDX(BufferWriter out,
		word val)
	{
		out.WriteByte(OP::OP_LDX);
		out.WriteByte(val);
	}
	static void InterpretSTA(BufferWriter out,
		REG::register_t r)
	{
		out.WriteByte(OP::OP_STA);
		out.WriteByte(r);
	}
#pragma endregion

	Interpreter::Interpreter()
	{
	}
	Interpreter::Interpreter(std::string path)
	{
		std::ifstream ifs(path);
		this->source = "";
		while (!ifs.eof())
			this->source += ifs.get();
		ifs.close();
	}
	Interpreter::~Interpreter()
	{
	}

	std::string Interpreter::Source()
	{
		return this->source;
	}
	byte* Interpreter::Output()
	{
		return this->output;
	}

	void Interpreter::Parse()
	{
		BufferWriter writer = BufferWriter();
		writer.WriteByte('B');	/* Header */
		writer.WriteByte('3');	/* Header */
		writer.WriteByte('2');	/* Header */
		writer.WriteWord(0);	/* Starting address */
		writer.WriteWord(0);	/* Execution address */

		writer.WriteToFile("out.b32");
	}
#pragma endregion
#pragma region Machine
	Machine::Machine()
	{
		this->memory = new byte[this->maxmemory];
		regA = 0;
		regB = 0;
		regD = 0;
		regX = 0;
		regY = 0;
	}
	Machine::~Machine()
	{

	}
#pragma endregion
}