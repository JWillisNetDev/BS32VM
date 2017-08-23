#pragma once
#if !defined(BS32VM_BUF)
#define BS32VM_BUF

#include <vector>
#include <fstream>
#include "bs32vm.hpp"

namespace BS32
{
	namespace BUF
	{
		enum buferr_t : byte
		{
			BUFERR_OK = 0x00,				/* No error */
			BUFERR_INDEX_OUT_OF_RANGE,		/* When you attempt to index out-of-range memory */
			BUFERR_PUSH_OUT_OF_RANGE		/* When you attempt a push that would exceed memory */
		};
	}
	class BufferWriter
	{
	public:
		BufferWriter();
		~BufferWriter();

		std::vector<byte> Buffer();
		byte* Data();
		uint Length();

		void Empty();

		void Pad(uint);

		void WriteByte(byte);
		void WriteWord(word);
		void WriteInstruction(Instruction);
		void WriteInt(int);

		void WriteByteAt(uint, byte);
		void WriteWordAt(uint, word);
		void WriteInstructionAt(uint, Instruction);
		void WriteIntAt(uint, int);
		
		void WriteToFile(std::string);
	private:
		std::vector<byte> buffer;
	};
	class BufferReader
	{
	public:
		BufferReader();
		~BufferReader();

		byte* Buffer();
		uint Length();
		uint Location();

		void Empty();

		void Assign(byte*, uint);
		void Assign(BufferWriter);

		void Advance(uint);
		void Move(uint);

		bool Good();

		byte Peek(uint);

		byte ReadByte();
		word ReadWord();
		int ReadInt();
	protected:
		byte* buffer;
		uint length;
		uint ptr;
	};
	class ParseBuffer : protected BufferReader
	{
	public:
		ParseBuffer(std::string);
		~ParseBuffer();

		byte* Buffer();
		uint Length();

		std::string String();

		bool Good();

		char ReadChar();
		std::string ReadLine(char = '\n', bool = false);
	};
}

#endif