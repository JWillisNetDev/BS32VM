#define BS32VM_C

#include <iostream>
#include <cstdlib>

#include "bs32vm.hpp"
#include "bs32buf.hpp"

namespace BS32
{
#pragma region Instruction
	/* DEFAULTS: 0, 0, 0 */
	Instruction::Instruction(OP::opcode_t op,
		byte a, byte b)
	{
		this->opcode = op;
		this->argA = a;
		this->argB = b;
	}
	Instruction::~Instruction()
	{
	}
#pragma region Methods
	OP::opcode_t Instruction::OpCode() const
	{
		return this->opcode;
	}
	byte Instruction::ArgA() const
	{
		return this->argA;
	}
	byte Instruction::ArgB() const
	{
		return this->argB;
	}
	short Instruction::ArgAx() const
	{
		return (short)((argA << 0x8) + argB);
	}
	word Instruction::ArguAx() const
	{
		return (word)((argA << 0x8) + argB);
	}
#pragma endregion
#pragma endregion
#pragma region Interpreter
	/* The interpreter is responsible for parsing
		assembler code into bytecode */
#pragma region Static Functions
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
	static bool isLetter(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}
	static bool isDigit(char c)
	{
		return c >= '0' && c <= '9';
	}
	static bool isLetterOrDigit(char c)
	{
		return isLetter(c) || isDigit(c);
	}
	static char toUpper(char c)
	{
		if (c >= 'A' && c <= 'Z')
			return c;
		return c + 'A' - 'a';
	}
	static byte byteParse(std::string str,
		bool isHex = false)
	{
		/* TODO: IMPLEMENT MAX */
		byte val = 0;
		for (uint i = 0; i < str.length(); i++)
		{
			char c = str[i];
			if (isHex)
				val = val * 16 + ((c >= '0' && val <= '9')
					? (c - '0')
					: (10 + (c - 'A')));
			else
				val = val * 10 + (c - '0');
		}
		return val;
	}
	static word wordParse(std::string str,
		bool isHex = false)
	{
		/* TODO: IMPLEMENT MAX */
		word val = 0;
		for (uint i = 0; i < str.length(); i++)
		{
			char c = str[i];
			if (isHex)
				val = val * 16 + ((c <= '0' && c >= '9')
					? (c - '0')
					: (10 + (c - 'A')));
			else
				val = val * 10 + (c - '0');
		}
		return val;
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