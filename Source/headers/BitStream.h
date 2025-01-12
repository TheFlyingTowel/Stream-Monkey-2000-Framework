#pragma once
/*
* Stream Monkey 2000: BitStream
*
* SM2K bit stream class. This class is resposible for reading
* and writting bit data I/O files.
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
	class BitOfStream
	{


	public:
		/// <summary>
		/// This will open or create a file, and write the data by bit.
		/// </summary>
		/// <param name="_fileName">: Output file path.</param>
		/// <param name="_fileHeader">: Output pointer for header data.</param>
		/// <param name="_headerSize">: Header size..</param>
		/// <param name="_headerReadPosition">: The read position for the header. (Note: -1 will set the read poition to the end of the file.)</param>
		/// <param name="_beginOffSet">: Offsets the file start position.</param>
		BitOfStream(const string& _fileName, const char* _fileHeader, u64 _headerSize = 0, s64 _headerWritePosition = 0, u64 _beginOffSet = 0);
		

		/// <summary>
		/// This will open or create a file, and write the data by bit.
		/// </summary>
		BitOfStream();

		/// <summary>
		/// Clean up.
		/// </summary>
		~BitOfStream();

		/// <summary>
		/// Writes data to the file by a size.
		/// </summary>
		/// <param name="_data">: Pointer to data.</param>
		/// <param name="_size">: Data size.</param>
		/// <returns></returns>
		inline void Write(char* _data, u64 _size)
		{
			m_stream.write(_data, _size);
		};

		/// <summary>
		/// Get current bit index.
		/// </summary>
		/// <returns></returns>
		inline u64 GetIndex() const { return m_index; }

		/// <summary>
		/// Sets the current bit index.
		/// </summary>
		/// <param name="_index">: Index position.</param>
		/// <returns></returns>
		inline void SetIndex(u64 _index) { m_index = _index; }

		/// <summary>
		/// Write bits to the file.
		/// </summary>
		/// <param name="_bits"></param>
		/// <returns></returns>
		BitOfStream& operator<<(vector(bool)& _bits);

		/// <summary>
		/// Write one bit to the file.
		/// </summary>
		/// <param name="_bits"></param>
		/// <returns></returns>
		BitOfStream& operator<<(bool& _bit);

		/// <summary>
		/// Write bits to the file.
		/// </summary>
		/// <param name="_bits"></param>
		/// <returns></returns>
		BitOfStream& operator<<(char& _bit);

		/// <summary>
		/// Write bits to the file.
		/// </summary>
		/// <param name="_bits"></param>
		/// <returns></returns>
		BitOfStream& operator<<(u8& _bit);

		/// <summary>
		/// Close the file stream.
		/// </summary>
		/// <returns></returns>
		void Close();

		/// <summary>
		/// Writes the remaining bits into the file.
		/// </summary>
		/// <returns></returns>
		void Flush();

		/// <summary>
		/// Opens the given file.
		/// </summary>
		void Open(const string& _file);

		/// <summary>
		/// Set the write postion starting from the front of the file.
		/// </summary>
		void SetWritep(u64 pos);

		/// <summary>
		/// Set the write position starting from the back of the  file.
		/// </summary>
		void rSetWritep(u64 pos);

		/// <summary>
		/// Gets the current write positon.
		/// </summary>
		inline u64 GetWritePos() { return m_stream.tellp(); }

	private:
		std::ofstream m_stream;
		char m_buffer;
		int m_index;



	};


	class BitIfStream
	{


	public:
		/// <summary>
		/// This will open a file stream, and read the data by bit.
		/// </summary>
		/// <param name="_fileName">: Input file path.</param>
		/// <param name="_fileHeader">: Output pointer for header data.</param>
		/// <param name="_headerSize">: Header size..</param>
		/// <param name="_headerReadPosition">: The read position for the header. (Note: -1 will set the read poition to the end of the file.)</param>
		/// <param name="_beginOffSet">: Offsets file start position.</param>
		BitIfStream(const string& _fileName, const char* _fileHeader, u64 _headerSize = 0, s64 _headerReadPosition = 0, u64 _beginOffSet = 0);

		/// <summary>
		/// This will open a file stream, and read the data by bit.
		/// </summary>
		/// <returns></returns>
		BitIfStream();

		/// <summary>
		/// Clean up.
		/// </summary>
		~BitIfStream();

		/// <summary>
		/// Reads data from a file by size.
		/// </summary>
		/// <param name="_data">: Pointer to output data buffer.</param>
		/// <param name="_size">: Amount of data to read.</param>
		inline void Read(char* _data, u64 _size)
		{
			m_stream.read(_data, _size);
		}

		/// <summary>
		/// Reads the next data in the file.
		/// </summary>
		inline void Loadp()
		{
			m_stream.read(&m_buffer, 1);
			m_index = 7;
		}

		/// <summary>
		/// Gets the current bit index.
		/// </summary>
		inline u64 GetIndex() { return m_index; }

		/// <summary>
		/// Sets the current bit index.
		/// </summary>
		inline void SetIndex(int _index) { m_index = _index; }

		/// <summary>
		/// Read in file data by bit.
		/// </summary>
		BitIfStream& operator>>(bool& _bits);

		/// <summary>
		/// Close the file.
		/// </summary>
		void Close();

		/// <summary>
		/// Open a file to read.
		/// </summary>
		void Open(const string& _file);


		/// <summary>
		/// Sets the read position.
		/// </summary>
		void SetReadp(u64 pos);

		/// <summary>
		/// Clears the buffer and resets the index to -1.
		/// </summary>
		void Reset();

		/// <summary>
		/// Set the read pointer.
		/// </summary>
		void rSetReadp(u64 pos);

		/// <summary>
		/// Get read position.
		/// </summary>
		inline u64 GetReadPos() { return m_stream.tellg(); }


	private:
		std::ifstream m_stream;
		char m_buffer;
		int m_index;


	};

};