#include "pch.h"


//#define GATEWARE_DISABLE_GVULKANSURFACE

namespace SM2K 
{


		
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
		auto& _registry = *_reg.registry;
		auto _entity = _reg.entity, core = _registry.ctx().get<_Entity>();
		_registry.emplace<Core_layer::_Message>(_entity, Core_layer::_Message{ _contex, _msg });
		_registry.patch<Core_layer::_Log>(core);

	}

	void EnableConsoleLogging(_REGENT _reg, bool _state)
	{
		auto& screenLog = _reg.registry->get<Core_layer::_Log>(_reg.entity).log;

		screenLog.EnableConsoleLogging(_state);
	}


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

}