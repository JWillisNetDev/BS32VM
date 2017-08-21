#pragma once
#define BS32VM_BUF_C

#include "bs32vm.hpp"
#include "bs32buf.hpp"

namespace BS32
{
#pragma region BufferWriter
	BufferWriter::BufferWriter()
	{
	}
	BufferWriter::~BufferWriter()
	{
	}

	std::vector<byte> BufferWriter::Buffer()
	{
		return this->buffer;
	}
	byte* BufferWriter::Data()
	{
		return &(this->buffer[0]);
	}
	uint BufferWriter::Length()
	{
		return this->buffer.size();
	}

	void BufferWriter::Empty()
	{
		this->buffer.empty();
	}

	void BufferWriter::Pad(uint c = 1)
	{
		for (uint i = 0; i < c; i++)
			this->buffer.push_back((byte)0);
	}

	void BufferWriter::WriteByte(byte val)
	{
		this->buffer.push_back(val);
	}
	void BufferWriter::WriteWord(word val)
	{
		this->buffer.push_back(val & 0x00FF);
		this->buffer.push_back((val & 0xFF00) >> 0x8);
	}
	void BufferWriter::WriteInt(int val)
	{
		this->buffer.push_back(val & 0x000000FF);
		this->buffer.push_back((val & 0x0000FF00) >> 0x8);
		this->buffer.push_back((val & 0x00FF0000) >> 0x10);
		this->buffer.push_back((val & 0xFF000000) >> 0x18);
	}

	void BufferWriter::WriteByteAt(uint loc, byte val)
	{
		this->buffer[loc] = val;
	}
	void BufferWriter::WriteWordAt(uint loc, word val)
	{
		this->buffer[loc] = val & 0x00FF;
		this->buffer[loc + 1] = (val & 0xFF00) >> 0x8;
	}
	void BufferWriter::WriteIntAt(uint loc, int val)
	{
		this->buffer[loc] = val & 0x000000FF;
		this->buffer[loc + 1] = (val & 0x0000FF00) >> 0x8;
		this->buffer[loc + 2] = (val & 0x00FF0000) >> 0x10;
		this->buffer[loc + 3] = (val & 0xFF000000) >> 0x18;
	}

	void BufferWriter::WriteToFile(std::string path)
	{
		std::ofstream ofs(path);
		for (uint i = 0; i < this->buffer.size(); i++)
			ofs.put(this->buffer[i]);
		ofs.close();
	}
#pragma endregion
#pragma region BufferReader
	BufferReader::BufferReader()
	{
		this->buffer = nullptr;
		this->length = 0;
		this->ptr = 0;
	}
	BufferReader::~BufferReader()
	{
		//delete[] this->buffer;
	}

	byte* BufferReader::Buffer()
	{
		return this->buffer;
	}
	uint BufferReader::Length()
	{
		return this->length;
	}
	uint BufferReader::Location()
	{
		return this->ptr;
	}

	void BufferReader::Empty()
	{
		delete[] this->buffer;
		this->length = 0;
		this->ptr = 0;
	}

	void BufferReader::Assign(byte* buf, uint len)
	{
		delete[] this->buffer;
		this->buffer = buf;
		this->length = len;
		for (uint i = 0; i < len; i++)
			this->buffer[i] = 0;
		this->ptr = 0;
	}
	void BufferReader::Assign(BufferWriter buf)
	{
		delete[] this->buffer;
		this->buffer = new byte[buf.Length()];
		this->length = buf.Length();
		for (uint i = 0; i < this->length; i++)
			this->buffer[i] = buf.Buffer()[i];
		this->ptr = 0;
	}

	void BufferReader::Advance(uint count = 1)
	{
		ptr += count;
	}
	void BufferReader::Move(uint loc)
	{
		ptr = loc;
	}

	bool BufferReader::Good()
	{
		return this->ptr < this->length;
	}

	byte BufferReader::Peek(uint offset = 1)
	{
		if (ptr + offset > length) return 0;
		return buffer[ptr + offset];
	}

	byte BufferReader::ReadByte()
	{
		if (ptr > length) return 0;
		return buffer[ptr++];
	}
	word BufferReader::ReadWord()
	{
		if (ptr + 1 > length) return 0;
		word val = (buffer[ptr]) + (buffer[ptr + 1] << 0x8);
		ptr += 2;
		return val;
	}
	int BufferReader::ReadInt()
	{
		if (ptr + 3 > length) return 0;
		int val = (buffer[ptr]) + (buffer[ptr + 1] << 0x8) + (buffer[ptr + 2] << 0x10) + (buffer[ptr + 3] << 0x18);
		ptr += 4;
		return val;
	}
#pragma endregion
#pragma region ParseBuffer
	ParseBuffer::ParseBuffer(std::string str)
	{
		this->buffer = new byte[str.length()];
		this->length = str.length();
		for (uint i = 0; i < str.length(); i++)
			this->buffer[i] = str[i];
		this->ptr = 0;
	}
	ParseBuffer::~ParseBuffer()
	{
	}

	byte* ParseBuffer::Buffer()
	{
		return buffer;
	}
	uint ParseBuffer::Length()
	{
		return this->length;
	}

	std::string ParseBuffer::String()
	{
		std::string str = "";
		return str;
	}
	
	bool ParseBuffer::Good()
	{
		return this->ptr < this->length;
	}

	char ParseBuffer::ReadChar()
	{
		return (char)buffer[ptr++];
	}
	std::string ParseBuffer::ReadLine(char delim, 
		bool inclusive)
	{
		std::string str = "";
		char c = ReadChar();
		while (c != delim && this->Good())
		{
			str += c;
			c = ReadChar();
		}
		if (inclusive)
			str += delim;
		return str;
	}
#pragma endregion
}