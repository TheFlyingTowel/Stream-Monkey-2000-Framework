#pragma once


#define tos(_number) std::to_string(_number) // To string
#define tos_cs(_cstring) string(_cstring) // To string

namespace SM2K
{
	namespace Core_layer
	{

		// Tags //

		TAG(CoreStart);
		TAG(CoreStop);
		TAG(Control);
		TAG(Web);

		// Components //

		COMPONENT(_EnableConsoleLogging)
		{
			bool enableConsoleLog;
		};

		COMPONENT(Core)
		{
			cv cv;
			list(ID) freeRequests;
			Shared(thread) control = nullptr;
			Shared(thread)webListener = nullptr;
			mutable mutex coreMutex;
			mutable mutex requestMutex;
			mutable mutex responceMutex;
			bool isRunning;
		};

		COMPONENT(smConfig) 
		{
			Shared(_Config) config = make(_Config);
		};

		COMPONENT(smSystemPaths)
		{
			string tmpPath;
			string dataRootPath;
			string logPath;
			string showPath;
			string hlsBaseDumpPath;
			string streamInstanceDumpPath;
			string urlBase;
		};

		COMPONENT(smUnformattedShow) 
		{
			string name;
			string path;
		};

		COMPONENT(smShow) 
		{
			u64 sourceCount;
			u64 totalTimeLength;
			string name;
			string path;
		};
		COMPONENT(smShowLoader) 
		{
			list(smUnformattedShow) unformattedShows;
			list(smShow) formattedShows;
		};

		COMPONENT(smUnformattedShows)
		{
			list(smUnformattedShow) shows;
		};
		
		COMPONENT(smFormattedShows)
		{
			list(smShow) shows;
		};

		COMPONENT(_Log)
		{
			GW::SYSTEM::GLog log;
			GW::SYSTEM::GLog logFile;
		};

		COMPONENT(_Message)
		{
			string contex;
			string msg;
		};

	}
}
	/*	void SendRequest(ID& _id, cstring _data, RequestType _communicationType = CMD_REQUEST, cstring _metaData = nullptr);
		void SendAndWait(ID& _id, cstring _data, RequestType _communicationType = CMD_REQUEST, cstring _metaData = nullptr);
		void WaitForResponce(const ID& _requestId);
		void ReadInResponce(const ID& _requestId, Responce& _responceRef);
		void SetAppReadFrequency(const App::milli& _freq);
		void _control(Core* _core);
		void _webListener(Core* _core);

		const RequestPtr& readRequest(const ulong& _id) const;
		bool deleteComBufferData(const ulong& _id, ComBufferType _commuincationBufferType);
		void sendResponce(const ResponceType& _result, const ulong& _id, cstring = nullptr);
		ID sendRequest(cstring _data, RequestType _communicationType, ulong _size = 2048, cstring _metaData = nullptr, ulong _metaSize = 1024);

		void signalApp(ID _id);
		void startApp();
		void ready();*/