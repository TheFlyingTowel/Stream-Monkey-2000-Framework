#pragma once

namespace SM2K 
{
	u64 GetEarilestSetBitIndex64(const u64& _buffer);
	vector(string) Split(string _string, const u64& _count);
	vector(string) Split(string _string, const string delimiter);
	string PtrToString(void* _ptr);

	string GetContex(const string& _name, void* _ptr);
	void Print(_REGENT _reg, string _msg, const string& _contex = "");
	void EnableConsoleLogging(_Registry& _registry, bool _state);



	COMPONENT(FilePath) 
	{
		string path;
	};

	COMPONENT(ShowCompressConfig) 
	{
		string name;
	};

	/// <summary>
	/// A container for the read line file pointer position.
	/// </summary>
	COMPONENT(PosData)
	{
		u64 _current_postion;
		u64 _max_line_index = 0;
		s8 _bit_index = -1;
		u8 _bit_buffer = 0x0;
		PosData() : _current_postion{ sizeof(PosData) }, _max_line_index{ 0 }{}
		PosData(u64 _pos) : _current_postion{ _pos }, _max_line_index{ 0 } {}
		//PosData(u64 _pos, s8 _ndx) : _current_postion{ _pos }, _max_line_index{ 0 }, _current_index{ _ndx } {}
	};

	struct _Compression 
	{
	private:
		struct Node
		{
			Node* left;
			Node* right;
			Node* parent;

			s16 value;
			u16 frequency;

			Node(s16 _value, u16 _frequency, Node* _left = nullptr, Node* _right = nullptr, Node* _parent = nullptr)
				:value{_value}, frequency{_frequency}, left{_left}, right{_right}, parent{_parent}
			{}

		};

		struct NodeCompare 
		{
			bool operator()(const Node* a, const Node* b)
			{
				return (a->frequency > b->frequency);
			}
		};

	public:

		_Compression(_Registry& _registry, _Entity e)
			: registry{ _registry }, entity{ e }, path{ GET(FilePath, e).path }
		{
			isOpen = false;
			DELETE(FilePath, e);
		};

		virtual ~_Compression();
		
		std::fstream& flush_out(std::fstream& _out);
		std::fstream& bv_out(std::fstream& _out, std::vector<bool>& _bits);
		std::ifstream& bv_in(std::ifstream& _in, bool& _bit);


		oss& flush_out_oss(oss& _out);
		oss& bv_out_oss(oss& _out, std::vector<bool>& _bits);
		iss& bv_in_str(iss& _in, bool& _bit);

		void CompressByLine(const vector(string)& _lines, const u8 _endLine = '\n');
		void ReadByLine(string& _line, const char& _endLine = '\n');
		void ReadByIndex(string& _line, const u64& _index, const char _endLine = '\n');

		void CompressByLineConfig(const vector(string)& _lines, const u8 _endLine = '\n');
		void ReadByLineConfig(string& _line, const char& _endLine = '\n');
		void ReadByIndexConfig(string& _line, const u64& _index, const char _endLine = '\n');
		
		void End(); // Removes this componetent from the entity.
	private:
		bool isOpen = false;
		_Registry& registry;
		_Entity entity;
		string path;
		u32 frequencyTable[256]{0};
		vector(Node*) leafList;
		vector(bool) encodingTable[256];
		PosData positionData;
		
		Node* tree = nullptr;
		
		u8 bit_buffer = 0x0;
		char bit_index = 7;



		void _clear();
		void _clear(Node* _node);
		void generateFrequencyTable(vector(string) _data);
		void generateLeafList();
		void generateTree();
		void generateEncodingTable();

		char decompressChar(iss& ifs, Node* _node);
		char decompressChar(std::ifstream& ifs, Node* _node);

		void saveHeader();

		void saveLinePosData();
		void loadLinePosData();

		void saveMetaData(ini::IniFile& out, oss& data);
	};

	
	COMPONENT(smCompression)
	{
		Shared(_Compression) com;
		smCompression(_Registry& _reg, _Entity e)
			:com{make_a(_Compression, _reg, e) }
		{}
	};
	
	struct FileDirectory // Extracts a file's directory path.
	{
		string path;
		FileDirectory(const string& _file, bool format = false);

	};



	void Trim(string& _str, const string& _trim);
	bool Contains(const string& _str, const string& _content);

	string App_SysCmd(const vector(string)& _cmd);
	s32 EXE_SysCmd(const string& _cmd);
	s32 EXE_SysCmd(const vector(string)& _cmd);
	void EXE_SysCmdReadOut(const vector(string)& _cmd, string& _out);
	void EXE_SysCmdReadOut(const string& _cmd, string& _out);


	string GetAppDataFolder();

};