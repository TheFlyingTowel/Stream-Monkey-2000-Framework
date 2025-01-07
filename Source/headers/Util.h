#pragma once

namespace SM2K 
{
	u64 GetEarilestSetBitIndex64(const u64& _buffer);
	vector(string) Split(string _string, const u64& _count);
	vector(string) Split(string _string, const string delimiter);
	string PtrToString(void* _ptr);

	string GetContex(const string& _name, void* _ptr);
	void Print(_REGENT _reg, string _msg, const string& _contex = "");
	void EnableConsoleLogging(_REGENT _reg, bool _state);

	struct FileDirectory // Extracts a file's directory path.
	{
		string path;
		FileDirectory(const string& _file, bool format = false);

	};

};