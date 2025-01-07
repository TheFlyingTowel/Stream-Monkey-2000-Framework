#pragma once

namespace SM2K 
{
	class _Config : public ini::IniFile
	{
	public:
		_Config(_REGENT _reg = {}, bool _localConfig = false);
		virtual ~_Config();

		void Load(const string& _file);
		void Save(const string& _file);
	private:

		void generateDefaultFile(const string& _file);
	};
}
