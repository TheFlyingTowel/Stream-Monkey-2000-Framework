#include "pch.h"


//#define GATEWARE_DISABLE_GVULKANSURFACE




namespace SM2K
{

#define US (u8) 0x1F
#define S_US string((char) 0x1F)
#define EXT (u8) 0x03
#define S_EXT string((char) 0x03)
#define EOT (u8) 0x04
#define S_EOT string((char) 0x04)
#define NL (u8) 0x0A
#define S_NL string((char) 0x0A)

	mutex print_mutex;

	FileDirectory::FileDirectory(const string& _file, bool format)
	{
		size_t ndx = _file.rfind(format ? '\\' : '/');
		if (ndx != string::npos)
			path = _file.substr(0, ndx + 1);

	};



	u64 GetEarilestSetBitIndex64(const u64& buffer)
	{
		return 63 - __lzcnt64(buffer);
	}
    vector(string) Split(string str, const u64& ndx)
    {
		vector(string) result;
		if (ndx >= str.size()) { result.emplace_back(str); return result; }
		while (str.size() >= ndx)
		{
			result.emplace_back(str.substr(0, ndx));
			str.erase(0, ndx);
		}
		if (!str.empty()) result.emplace_back(str);
		return result;
    }
	vector(string) Split(string str, const string delimiter)
	{
		vector(string) result;
		size_t pos = str.find(delimiter);
		if (pos == string::npos) { result.emplace_back(str); return result; }

		for (; pos != string::npos; pos = str.find(delimiter))
		{
			result.emplace_back(str.substr(0, pos));
			str.erase(0, pos + delimiter.length());
		}

		if (!str.empty())
			result.emplace_back(str);

		return result;
	}


	string PtrToString(void* _ptr)
	{
		std::ostringstream oss;
		oss << "0x"
			<< std::setfill('0') << std::setw(sizeof(void*) * 2)
			<< std::hex << reinterpret_cast<std::uintptr_t>(_ptr);
		return oss.str();
	}

	string GetContex(const string& _name, void* _ptr)
	{
		return _name + " @ " + PtrToString(_ptr);
	}
	
	void Print(_REGENT _reg, string _msg, const string& _contex)
	{
		lockGaurd _lock(print_mutex);
		if (!_reg.registry) return;
		auto& _registry = *_reg.registry;
		auto _entity = _reg.entity, core = _registry.ctx().get<_Entity>();
		if (!_registry.valid(_entity))return;
		_registry.emplace<Core_layer::_Message>(_entity, Core_layer::_Message{ _contex, _msg });
		_registry.patch<Core_layer::_Log>(core);

	}

	void EnableConsoleLogging(_Registry& _registry, bool _state)
	{
		auto& screenLog = GET(Core_layer::_Log, GGET(_Entity)).log;

		screenLog.EnableConsoleLogging(_state);
	}


#pragma region Log

	void Construct_Log(_Registry& _registry, _Entity e)
	{
		auto& self = _registry.get<Core_layer::_Log>(e);
		auto& screenLog = self.log;
		auto& fileLog = self.logFile;
		const auto& sysPaths = GET(smSystemPaths, e);

		std::stringstream name;
		name << "StreamMonkey2K-" << std::time(nullptr) << ".log";

		screenLog.Create((sysPaths.tmpPath + "__" + name.str()).c_str());
		fileLog.Create((sysPaths.logPath + name.str()).c_str());

		fileLog.EnableConsoleLogging(false);
		fileLog.EnableVerboseLogging(true);

		screenLog.EnableVerboseLogging(false);


	}
	void Update_Log(_Registry& _registry, _Entity e)
	{
		auto entitiesWithMsg = _registry.view<Core_layer::_Message>();
		auto& logger = GET(Core_layer::_Log, e); // Get the logger we are going to print to.


		for (auto& entity : entitiesWithMsg)
		{

			auto& msg = GET(Core_layer::_Message, entity); // Get the message to print.


			if (msg.contex.empty())
			{
				logger.log.Log(msg.msg.c_str());
				logger.logFile.Log(msg.msg.c_str());
			}
			else
			{
				logger.log.LogCategorized(msg.contex.c_str(), msg.msg.c_str());
				logger.logFile.LogCategorized(msg.contex.c_str(), msg.msg.c_str());
			}

			_registry.remove<Core_layer::_Message>(entity); // Remove message component from the entity.
		}
	}
	void Destroy_Log(_Registry& _registry, _Entity e)
	{
		auto& logger = GET(Core_layer::_Log, e);

		logger.log.Flush();
		logger.logFile.Flush();

	}

	CONNECT_COMPONENT()
	{
		CONNECT_ON_CONSTRUCT(Core_layer::_Log, Construct_Log);
		CONNECT_ON_UPDATE(Core_layer::_Log, Update_Log);
		CONNECT_ON_DESTROY(Core_layer::_Log, Destroy_Log);
	}
#pragma endregion

#pragma region Compression
	void _Compression::generateFrequencyTable(vector(string) _data)
	{
		memset(frequencyTable, 0x0, sizeof(frequencyTable));
		
		while (!_data.empty())
		{
			string line = _data[0];
			while (!line.empty())
			{
				++frequencyTable[(u8)line[0]];
				line.erase(0, 1);
			}
			_data.erase(_data.begin());
		}

	}

	_Compression::~_Compression()
	{
		_clear();
	};

	std::fstream& _Compression::flush_out(std::fstream& _out)
	{
		if (bit_index < 7)
			_out.write((char*) &bit_buffer, 1);
		bit_index = 7;

		return _out;
	}
	std::fstream& _Compression::bv_out(std::fstream& _out, std::vector<bool>& _bits)
	{
		for (auto bit : _bits)
		{
			if (bit)
				bit_buffer |= 1 << bit_index;
			--bit_index;

			if (bit_index < 0)
			{
				_out.write((char*) &bit_buffer, 1);
				bit_buffer = 0x0;
				bit_index = 7;
			}

		}
		return _out;
	}
	std::ifstream& _Compression::bv_in(std::ifstream& _in, bool& _bit)
	{
		if(bit_index < 0)
		{
			bit_index = 7;
			_in.read((char*) &bit_buffer, 1);
		}

		_bit = !!(bit_buffer & (1 << bit_index));
		--bit_index;
		return _in;
	}

	oss& _Compression::flush_out_oss(oss& _out)
	{
		if (bit_index < 7)
			_out.write((char*)&bit_buffer, 1);
		bit_index = 7;

		return _out;
	}

	oss& _Compression::bv_out_oss(oss& _out, std::vector<bool>& _bits)
	{
		for (auto bit : _bits)
		{
			if (bit)
				bit_buffer |= 1 << bit_index;
			--bit_index;

			if (bit_index < 0)
			{
				_out.write((char*)&bit_buffer, 1);
				bit_buffer = 0x0;
				bit_index = 7;
			}

		}
		return _out;
	}

	iss& _Compression::bv_in_str(iss& _in, bool& _bit)
	{
		if (bit_index < 0)
		{
			bit_index = 7;
			_in.read((char*)&bit_buffer, 1);
		}

		_bit = !!(bit_buffer & (1 << bit_index));
		--bit_index;
		return _in;
	}

	void _Compression::CompressByLine(const vector(string)& _lines, const u8 _endLine)
	{
		if (!isOpen)
		{
			generateFrequencyTable(_lines);
			frequencyTable[_endLine] = _lines.size();
			++frequencyTable[EOT];
			generateLeafList();
			generateTree();
			generateEncodingTable();
			{
				std::ofstream(path, std::ios::binary).close(); // creates the file if it doesn't exixt already.
			}
			isOpen = true;
		}

		std::fstream ofs(path, std::ios::binary | std::ios::out | std::ios::in);
		
		ofs.seekp(0);
		PosData pos{ sizeof(PosData)};
		ofs.write((char*)&pos, sizeof(PosData));

		u32 maxIndex = -1;
		for(string line : _lines)
		{
			for (s32 i = 0; i < line.length(); ++i)
				bv_out(ofs, encodingTable[(u8)line[i]]);
			bv_out(ofs, encodingTable[(u8)_endLine]);
			++maxIndex;
		}
		bv_out(ofs, encodingTable[(u8)EOT]);
		flush_out(ofs);
		++maxIndex;

		ofs.write((char*)frequencyTable, sizeof(frequencyTable));
		ofs.close();
		
		pos._max_line_index = maxIndex;
		positionData = pos;
		saveLinePosData();
	}

	void _Compression::ReadByLine(string& _line, const char& _endLine)
	{
		oss result;
		std::ifstream ifs(path, std::ios::binary);
		if(!isOpen)
		{
			ifs.seekg(-1 * sizeof(frequencyTable), std::ios::end);
			ifs.read((char*)frequencyTable, sizeof(frequencyTable));
			generateLeafList();
			generateTree();
			isOpen = true;
		}

		loadLinePosData();
		ifs.seekg(positionData._current_postion);
		bit_index = positionData._bit_index;
		bit_buffer = positionData._bit_buffer;

		char tmp = 0x0;
		while(true)
		{
			tmp = decompressChar(ifs, tree);


			if (tmp == EOT || tmp == _endLine) break;
			positionData._current_postion = ifs.tellg();
			result << tmp;
		}

		if(tmp == EOT)
		{
			result.str("");
			positionData._current_postion = sizeof(PosData);
			positionData._bit_buffer = 0;
			positionData._bit_index = -1;
			ifs.close();
			
			saveLinePosData();
			_line = result.str();
			return;
		}
		else positionData._current_postion = ifs.tellg();
		ifs.close();
		
		positionData._bit_index = bit_index;
		positionData._bit_buffer = bit_buffer;
		saveLinePosData();
		_line = result.str();
	}

	void _Compression::ReadByIndex(string& _line, const u64& _index, char _endLine)
	{
		oss result;
		u64 index = 0;
		std::ifstream ifs(path, std::ios::binary);
		
		if(!isOpen)
		{
			isOpen = true;
			ifs.seekg(-1 * sizeof(frequencyTable), std::ios::end);
			ifs.read((char*)frequencyTable, sizeof(frequencyTable));
			generateLeafList();
			generateTree();
		}

		ifs.seekg(0);
		ifs.read((char*)&positionData, sizeof(PosData));

		if(_index >= positionData._max_line_index)
		{
			_line = "";
			ifs.close();
			return;
		}

		char tmp = 0x0;
		u32 currentIndex = 0;
		bit_index = -1;
		bit_buffer = 0x0;
		while(true)
		{
			tmp = decompressChar(ifs, tree);
			if(currentIndex < _index)
			{
				if (tmp == _endLine) ++currentIndex;
				continue;
			}
			if (tmp == _endLine || tmp == EOT) break;
			result << tmp;
		}
		if (tmp == EOT)
		{
			positionData._current_postion = sizeof(PosData);
			ifs.close();
			saveLinePosData();
			_line = result.str();
			return;
		}
		else positionData._current_postion = ifs.tellg();
		
		ifs.close();
		saveLinePosData();
		_line = result.str();
	}

	void _Compression::CompressByLineConfig(const vector(string)& _lines, const u8 _endLine)
	{
		oss data("", std::ios::binary);

		if (!isOpen)
		{
			generateFrequencyTable(_lines);
			frequencyTable[_endLine] = _lines.size();
			++frequencyTable[EOT];
			generateLeafList();
			generateTree();
			generateEncodingTable();
			isOpen = true;
		}
		data.seekp(0);
		PosData pos{ sizeof(PosData) };
		data.write((char*)&pos, sizeof(PosData));


		u32 maxIndex = -1;
		for (string line : _lines)
		{
			for (s32 i = 0; i < line.length(); ++i)
				bv_out_oss(data, encodingTable[(u8)line[i]]);
			bv_out_oss(data, encodingTable[(u8)_endLine]);
			++maxIndex;
		}
		bv_out_oss(data, encodingTable[(u8)EOT]);
		flush_out_oss(data);
		++maxIndex;

		data.write((char*)frequencyTable, sizeof(frequencyTable));
		
		pos._max_line_index = maxIndex;
		positionData = pos;
		data.seekp(0);
		data.write((char*)&pos, sizeof(PosData));

		if(registry.any_of<ShowCompressConfig>(entity))
		{
			ini::IniFile out;
			if(registry.all_of<ShowCompressConfig>(entity))
			{
				auto& name = registry.get<ShowCompressConfig>(entity).name;
				out["ShowConfig"]["name"] = name;
				out["ShowConfig"]["sourceCount"] = maxIndex;
				out["ShowConfig"]["totalTimeLength"] = -1;
				out["DataBuffer"]["buffer"] = data.str();
			}

			out.save(path);
		}

	}

	void _Compression::ReadByLineConfig(string& _line, const char& _endLine)
	{
		ini::IniFile out;
		out.load(path);
		oss result;
		oss data("", std::ios::binary);
		iss in(out.at("DataBuffer").at("buffer").as<string>(), std::ios::binary);
		if (!isOpen)
		{
			in.seekg(-1 * sizeof(frequencyTable), std::ios::end);
			in.read((char*)frequencyTable, sizeof(frequencyTable));
			generateLeafList();
			generateTree();
			isOpen = true;
		}

		in.seekg(0);
		in.read((char*)&positionData, sizeof(positionData));
		in.seekg(positionData._current_postion);
		bit_index = positionData._bit_index;
		bit_buffer = positionData._bit_buffer;

		char tmp = 0x0;
		while (true)
		{
			tmp = decompressChar(in, tree);


			if (tmp == EOT || tmp == _endLine) break;
			positionData._current_postion = in.tellg();
			result << tmp;
		}

		if (tmp == EOT)
		{
			result.str("");
			positionData._current_postion = sizeof(PosData);
			positionData._bit_buffer = 0;
			positionData._bit_index = -1;
			
			data.str(in.str());
			
			data.seekp(0);
			data.write((char*)&positionData, sizeof(positionData));

			out["DataBuffer"]["buffer"] = data.str();
			out.save(path);
			_line = result.str();
			return;
		}
		else positionData._current_postion = in.tellg();
		

		positionData._bit_index = bit_index;
		positionData._bit_buffer = bit_buffer;
		data.str(in.str());

		data.seekp(0);
		data.write((char*)&positionData, sizeof(positionData));

		out["DataBuffer"]["buffer"] = data.str();
		out.save(path);
		_line = result.str();
	}

	void _Compression::ReadByIndexConfig(string& _line, const u64& _index, const char _endLine)
	{
	}

	void _Compression::End()
	{
		isOpen = false;
		memset(frequencyTable, 0x0, sizeof(frequencyTable));
		memset(&positionData, 0x0, sizeof(positionData));
		leafList.clear();
		for (int i = 0; i < 256; ++i)
			encodingTable[i].clear();
		registry.remove<_Compression>(entity);
	}

	char _Compression::decompressChar(iss& _stream, Node* _node)
	{
		while (_node->value == -1)
		{
			bool tmp;
			bv_in_str(_stream, tmp);
			if (tmp) _node = _node->right;
			else _node = _node->left;
		}


		return _node->value;
	}

	char _Compression::decompressChar(std::ifstream& _stream, Node* _node)
	{

		while (_node->value == -1)
		{
			bool tmp;
			bv_in(_stream, tmp);
			if (tmp) _node = _node->right;
			else _node = _node->left;
		}


		return _node->value;
	}

	void _Compression::_clear()
	{
		_clear(tree);
		tree = nullptr;
	}

	void _Compression::_clear(Node* _node)
	{
		if (!_node) return;
		_clear(_node->left);
		_clear(_node->right);
		delete _node;
	}

	void _Compression::generateLeafList()
	{
		leafList.clear();
		for (u16 ndx = 0; ndx < 256; ++ndx)
			if (frequencyTable[ndx])
				leafList.push_back(new Node(ndx, frequencyTable[ndx]));
	}

	void _Compression::generateTree()
	{
		std::priority_queue<Node*, std::vector<Node*>, NodeCompare> queue;
		for (auto& node : leafList) queue.push(node);

		while(queue.size() > 1)
		{
			Node* tmp0 = queue.top();
			queue.pop();

			Node* tmp1 = queue.top();
			queue.pop();

			queue.push(tmp0->parent = tmp1->parent = new Node( -1, tmp0->frequency + tmp1->frequency, tmp0, tmp1));
		}
		if(!queue.empty())
		{
			tree = queue.top();
			queue.pop();
		}
	}

	void _Compression::generateEncodingTable()
	{
		for(auto& node : leafList)
		{
			vector(bool)& encodingValue = encodingTable[node->value];
			Node* current = node;
			while(current != tree)
			{
				Node* parent = current->parent;
				encodingValue.emplace_back(current == parent->right);
				current = parent;
			}
			std::reverse(encodingValue.begin(), encodingValue.end());
		}
	}

	void _Compression::saveHeader()
	{
		//data.write((char*)frequencyTable, 256 * sizeof(u32));// Stores the headers at the end of the buffer.
	}

	void _Compression::saveLinePosData()
	{
		std::fstream ofs(path, std::ios::binary | std::ios::out | std::ios::in);
		ofs.seekp(0);
		ofs.write((char*)&positionData, sizeof(PosData));
		ofs.close();

	}

	void _Compression::loadLinePosData()
	{
		std::ifstream ifs(path, std::ios::binary | std::ios::in);
		ifs.seekg(0);
		ifs.read((char*)&positionData, sizeof(PosData));
		ifs.close();
	}
#pragma endregion



	void Trim(string& _str, const string& _trim)
	{
		u64 pos;
		while ((pos = _str.find(_trim)) != string::npos)
		{
			_str.erase(pos, _trim.length());
		}
	}

	string App_SysCmd(const vector(string)& _cmd)
	{
		string result = "";
		
		for(auto& str : _cmd)
			result.append(" " + str);
		
		return result;
	}

	s32 EXE_SysCmd(const string& _cmd)
	{
		return system(_cmd.c_str());
	}

	s32 EXE_SysCmd(const vector(string)& _cmd) 
	{
		return system(App_SysCmd(_cmd).c_str());
	}

	void EXE_SysCmdReadOut(const vector(string)& _cmd, string& _out)
	{
		
		EXE_SysCmdReadOut(App_SysCmd(_cmd), _out);

	}

	void EXE_SysCmdReadOut(const string& _cmd, string& _out)
	{
		string data = "";
		FILE* fileStream;
		char buffer[128];

		fileStream = _popen_(_cmd.c_str(), "r");
		if(fileStream)
		{
			fflush(fileStream);
			while(!feof(fileStream))
			{
				if (fgets(buffer, sizeof(fileStream), fileStream) != nullptr)
					data.append(buffer);
			}
			_pclose_(fileStream);
		}

		if (data.empty())
		{
			oss str;
			std::cout.flush();
			std::cout.rdbuf(str.rdbuf());
			data = str.str();
		}

		_out = data;

	}

	string GetAppDataFolder()
	{
		string out = "";
		SM2K::EXE_SysCmdReadOut(SM2K_APPDATA_CMD, out);
		SM2K::Trim(out, "\n");
		return out;
	}

}
