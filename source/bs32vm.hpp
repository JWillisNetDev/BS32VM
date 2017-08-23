#pragma once
#if !defined(BS32VM)
#define BS32VM

#include <string>
#include <fstream>
#include <vector>

/* BS32VM is a virtual machine inspired by B32. */

/*	SPECIFICATIONS:
*	8-bit register: A // 8-bit register: B
*	16-bit register Ax = $A .. $B
*	16-bit register X // 16-bit register: Y
*	64K memory
*	ADDR $A000 - $AFA0 dedicated to screen output
*/

/*	INSTRUCTIONS:
*	8 bits dedicated to opcode
*	8 bits dedicated to ArgA
*	8 bits dedicated to ArgB
*	ArgAx = ArgA + ArgB
*	ArguAx = unsigned ArgA + ArgB
*/

namespace BS32
{
	typedef unsigned char byte;
	typedef unsigned short word;
	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef unsigned int uint;
	
	const int BS32_SHORT_MAX = (~(word)0);
	const int BS32_MAX_MEM = BS32_SHORT_MAX;
	const int BS32_INSTRUCTION_SIZE = 3;


	namespace OP
	{
		enum opcode_t : byte
		{
			/* OPCODE	ARGS		DESC			*/
			/*--------------------------------------*/
			OP_NULL,/*	NULL		NULL			*/
			OP_LDA,	/*	A			RK(a) := A		*/
			OP_LDX,	/*	Ax			RK(x) := Bx		*/
			OP_STA, /*				STORE a AT x	*/
			OP_END = 0xff
		};
	}
	namespace REG
	{
		enum register_t : byte
		{
			REG_NULL = 0x00,
			REG_A = 0x01, REG_B, REG_D,
			REG_X = 0x10, REG_Y
		};
	}
	struct Instruction
	{
		OP::opcode_t opcode;
		byte argA, argB;

		/* Constructors */
		Instruction(OP::opcode_t = OP::OP_NULL,
			byte = 0, byte = 0);
		~Instruction();
		/* Methods */
		OP::opcode_t OpCode() const;
		byte ArgA() const;
		byte ArgB() const;
		short ArgAx() const;
		word ArguAx() const;
	};
	class Interpreter
	{
	public:
		Interpreter();
		Interpreter(std::string);
		~Interpreter();

		std::string Source();
		byte* Output();

		void Parse();
	private:
		std::string source;
		byte* output;
	};
	class Machine
	{
	public:
		Machine();
		~Machine();


	private:
		byte* memory;
		const uint maxmemory = BS32_MAX_MEM;
		/* TODO: Label map */
		byte regA, regB;
		word regD, regX, regY;
	};
}

#endif