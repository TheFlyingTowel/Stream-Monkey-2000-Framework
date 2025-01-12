
#include "pch.h"
/*
* Stream Monkey 2000: BitStream
*
* SM2K bit stream class. This class is resposible for reading
* and writting bit data io of files.
*
* Author:						Tourel A Britton
* Author email:					britton.tourel@gmail.com
*
* Note:
* License: N/A
*
*/
namespace SM2K
{
	BitOfStream::BitOfStream(const string& _fileName, const char* _fileHeader, u64 _headerSize, s64 _headerWritePosition, u64 _beginOffSet)
	{
		m_buffer = 0x0;
		m_index = 7;


		m_stream.open(_fileName, std::ios::binary);

		if (!m_stream.is_open());
			//throw (sm2kError*) new sm2kFileReadError("File: \"" + sm2kString(_fileName) + "\" Couldn't be created.");

		if (_fileHeader)
		{
			if (_headerWritePosition == -1)
				m_stream.seekp(-1 * _headerSize, std::ios::end);
			else
			{
				m_stream.seekp(_headerWritePosition);
				if (!_beginOffSet) _beginOffSet = _headerSize;
			}
			m_stream.write(_fileHeader, _headerSize);
			m_stream.seekp(_beginOffSet);

		}
	}
	BitOfStream::BitOfStream()
	{
		m_buffer = 0x0;
		m_index = 7;
	}
	BitOfStream::~BitOfStream()
	{
		Close();
	}
	BitOfStream& BitOfStream::operator<<(vector(bool)& _bits)
	{
		for (u32 ndx = 0; ndx < _bits.size(); ++ndx)
		{
			bool b = _bits[ndx];
			*this << b;
		}
		return *this;
	}
	BitOfStream& BitOfStream::operator<<(bool& _bit)
	{
		if (_bit)
			m_buffer |= 1 << m_index;

		--m_index;

		if (m_index < 0)
		{
			m_stream.write(&m_buffer, 1);
			m_buffer = 0x0;
			m_index = 7;
		}

		return *this;
	}


	BitOfStream& BitOfStream::operator<<(u8& _bit)
	{
		if (_bit)
			m_buffer = _bit;


		m_stream.write(&m_buffer, 1);
		m_buffer = 0x0;
		m_index = 7;


		return *this;

	}

	BitOfStream& BitOfStream::operator<<(char& _bits)
	{
		vector(bool) _bitBuffer;
		for (u64 ndx = 0; ndx < 8; ++ndx) // Construct bits into a bool vector.
		{
			char tmp = (1 << (7 - ndx));
			_bitBuffer.push_back(!!(_bits & tmp));
		}
		return (*this << _bitBuffer);
	}
	void BitOfStream::Flush()
	{
		if (m_index < 7)
			m_stream.write(&m_buffer, 1);
		m_index = 7;
	}
	void BitOfStream::Close()
	{
		Flush();

		m_stream.close();

	}
	void BitOfStream::Open(const string& _file)
	{
		m_stream.open(_file, std::ios::binary);
	}
	BitIfStream::BitIfStream(const string& _fileName, const char* _fileHeader, u64 _headerSize, s64 _headerReadPosition, u64 _beginOffSet)
	{
		m_stream.open(_fileName, std::ios::binary);

		if (!m_stream.is_open());
			//throw new sm2kFileReadError("File: \"" + sm2kString(_fileName) + "\" couldn't be opened!");

		if (_fileHeader)
		{
			memset((void*)_fileHeader, 0x0, _headerSize);

			if (_headerReadPosition == -1)
				m_stream.seekg(-1 * _headerSize, std::ios::end);
			else
			{
				if (!_beginOffSet) _beginOffSet = _headerSize;
				m_stream.seekg(_headerReadPosition);
			}


			m_stream.read((char*)_fileHeader, _headerSize);
			m_stream.seekg(_beginOffSet);
		}

		m_index = 7;
		m_stream.read(&m_buffer, 1);

	}
	BitIfStream::BitIfStream()
	{
		m_buffer = 0x0;
		m_index = 7;
	}
	BitIfStream::~BitIfStream()
	{
		Close();
	}
	BitIfStream& BitIfStream::operator>>(bool& _bit)
	{
		if (m_index < 0)
		{
			m_index = 7;
			m_stream.read(&m_buffer, 1);

		}

		_bit = !!(m_buffer & (1 << m_index));
		--m_index;
		return *this;
	}
	void BitIfStream::Close()
	{
		m_stream.close();
	}
	void BitIfStream::Open(const string& _file)
	{
		m_stream.open(_file, std::ios::binary);
		//m_stream.read(&m_buffer, 1);
	}

	void BitIfStream::SetReadp(u64 pos)
	{
		m_stream.seekg(pos);
	}

	void BitIfStream::Reset()
	{
		m_index = -1;
		m_buffer = 0x0;
		//m_stream.read(&m_buffer, 1);
	}

	void BitIfStream::rSetReadp(u64 pos)
	{
		m_stream.seekg(pos, std::ios::end);
	}
	void BitOfStream::SetWritep(u64 pos)
	{
		m_stream.seekp(pos);
	}
	void BitOfStream::rSetWritep(u64 pos)
	{
		m_stream.seekp(pos, std::ios::end);
	}
};
