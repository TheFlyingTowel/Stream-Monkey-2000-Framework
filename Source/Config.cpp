#include "pch.h"


namespace SM2K
{

	_Config::_Config(_REGENT _reg, bool _localConfig)
	{
		if (!_reg.registry || _localConfig) return; // If we are returning here there is a problem.(It shouldn't get this far)

		const auto& core = _reg.registry->ctx().get<_Entity>();
		auto& sysPaths = _reg.registry->get<smSystemPaths>(core);
		string defaults = (sysPaths.dataRootPath + "default.ini");
		string saved = ("./config.ini");
		namespace fs = std::filesystem;

		if (bool s = fs::exists(saved) &&  fs::exists(defaults))
		{
			auto dtime = fs::last_write_time(defaults);
			auto stime = fs::last_write_time(saved);

			if (dtime > stime)
				this->load(defaults);
			else this->load(saved);

		}
		else if (s)
		{
			this->load(saved);
			
			sysPaths.dataRootPath = (*this)["Core"]["dataRootPath"].as<string>();
			
			defaults = (sysPaths.dataRootPath + "default.ini");

			if(!fs::exists(defaults)) 
				generateDefaultFile(defaults);
			
		}
		else generateDefaultFile(defaults);
		

	}


	_Config::~_Config()
	{
		this->save("./config.ini");
	}

	void _Config::Load(const string& _file)
	{
		this->load(_file);
	}

	void _Config::Save(const string& _file)
	{
		this->save(_file);
	}

	void _Config::generateDefaultFile(const string& _file)
	{
		auto path = FileDirectory(_file).path;
		std::filesystem::create_directory(path);
		(*this)["Core"]["versionName"] = "Stream_Monkey_2000_Framework _Alpha";
		(*this)["Core"]["version"] = "0.0.1";
		(*this)["Core"]["dataRootPath"] = "./data/";
		(*this)["Core"]["tmpPath"] = "./.sm2k/";
		(*this)["Core"]["logPath"] = "./data/logs/";
		(*this)["Core"]["maxRegistry"] = 6000;
		(*this)["Stream"]["maxStreamCount"] = 500;
		(*this)["Stream"]["instancePath"] = "./data/stream_instances/";
		(*this)["Stream"]["hlsBaseDumpPath"] = "./data/dump/";
		(*this)["Stream"]["baseUrl"] = "";
		(*this)["Stream"]["port"] = 888;
		this->save(_file);
	}


}
