//#define AUTO_ID
#include "pch.h"


namespace SM2K
{

	namespace ECS
	{

		namespace {
			std::list<ComponentFunc> compLogic;
			std::list<ComponentFunc>& GetCompLogic()
			{
				return compLogic;
			};
		}

		sm2kComponentLogic::sm2kComponentLogic(ComponentFunc _logic)
		{
			GetCompLogic().push_back(_logic);
		}

		void InitializeComponents(_Registry& REGISTRY)
		{
			for (auto& func : GetCompLogic())
				func(REGISTRY);
		}
	}

	namespace Core_layer
	{

		//using namespace AutoID_Literals;



		void _control(_Registry* registry, _Entity e, Core* core)
		{
			auto& _registry = *registry;
			ADD(Control, e);

			while (core->isRunning)
			{

			}
		}

		void _webListener(_Registry* registry, _Entity e, Core* core)
		{
			auto& _registry = *registry;
			auto& _core = *core;

			ADD(Web, e);

			while (_core.isRunning)
			{

			}


		}

		void On_ConstructCore(_Registry& _registry, _Entity e)
		{
			auto& self = GET(Core, e);
			auto& sysPaths = ADD(smSystemPaths, e);
			auto& config = GET(smConfig, e).config; // Gets config file data.
			auto& log = ADD(_Log, e);

			log.log.EnableConsoleLogging(GET(_EnableConsoleLogging, e).enableConsoleLog);

			sysPaths.hlsBaseDumpPath = config->at("Stream").at("hlsBaseDumpPath").as<string>();
			sysPaths.streamInstanceDumpPath = config->at("Stream").at("instancePath").as<string>();
			sysPaths.urlBase = config->at("Stream").at("baseUrl").as<string>();

			if(!sysPaths.hlsBaseDumpPath.empty())
				std::filesystem::create_directories(sysPaths.hlsBaseDumpPath);

			if (!sysPaths.streamInstanceDumpPath.empty())
				std::filesystem::create_directories(sysPaths.streamInstanceDumpPath);


			
			Print(_REGENT{&_registry, e}, "Initalized stream scheduler.", GetContex("Core", &self));
			
			
			
		}


		void On_SignalCore(_Registry& _registry, _Entity e)
		{
			auto& self = GET(Core, e);
			lockGaurd lock(self.coreMutex);

			if(_registry.any_of<CoreStart, CoreStop>(e))
			{
				if (IS(e, CoreStart) && !self.isRunning)
				{
					if (self.control && self.webListener)
					{
						// Ensures control threads are completed.
						self.control->join();

						self.control.reset();
						self.webListener.reset();
					}
					self.isRunning = true;
					self.webListener = make_a(thread, &_webListener, &_registry, _registry.create(), &self);
					self.control = make_a(thread, &_control, &_registry, _registry.create(), &self);
					self.webListener->detach();
					DELETE(CoreStart, e);
				}
				else if (IS(e, CoreStop))
				{
					self.isRunning = false;
					auto control = GRAB(Control);
					auto web = GRAB(Web);
					_registry.destroy(control);
					_registry.destroy(web);
					DELETE(CoreStop, e);
				}
			}
		}

		void On_DestroyCore(_Registry& _registry, _Entity e)
		{
			auto& self = GET(Core, e);

			lockGaurd lock(self.coreMutex);
			self.isRunning = false;

			if(self.control) self.control->join();
		}


		void On_ConstructSystemPath(_Registry& _registry, _Entity e)
		{
			auto& sysPaths = GET(smSystemPaths, e); // Construct the defualt path for now.
			sysPaths.tmpPath = "./.sm2k/";
			sysPaths.dataRootPath = "./data/";
			sysPaths.logPath = "./data/logs/";
			sysPaths.showPath = "./data/shows/";

			auto& config = _registry.emplace<smConfig>(e, make_a(_Config, _REGENT{ &_registry, e })).config; // Loads config file.

			sysPaths.tmpPath = config->at("Core").at("tmpPath").as<string>();
			sysPaths.dataRootPath = config->at("Core").at("dataRootPath").as<string>();
			sysPaths.logPath = config->at("Core").at("logPath").as<string>();
			sysPaths.showPath = config->at("Core").at("showPath").as<string>();

			std::filesystem::create_directories(sysPaths.showPath);
			std::filesystem::create_directories(sysPaths.tmpPath);
			std::filesystem::create_directories(sysPaths.dataRootPath);
			std::filesystem::create_directories(sysPaths.logPath);
		}

		void On_DestroySystemPath(_Registry& _registry, _Entity e)
		{
			auto& sys = GET(smSystemPaths, e);
			std::filesystem::remove_all(sys.tmpPath);

		}

		CONNECT_COMPONENT()
		{
			CONNECT_ON_CONSTRUCT(Core, On_ConstructCore);
			CONNECT_ON_UPDATE(Core, On_SignalCore);
			CONNECT_ON_DESTROY(Core, On_DestroyCore);

			CONNECT_ON_CONSTRUCT(smSystemPaths, On_ConstructSystemPath);
			CONNECT_ON_DESTROY(smSystemPaths, On_DestroySystemPath);
		}

	}
}
		//const RequestPtr& readRequest(const ulong& _id) const
		//{
		//	return &p_buffer->request[_id];
		//}

		//bool deleteComBufferData(const ulong& _id, ComBufferType _commuincationBufferType)
		//{
		//	switch (_commuincationBufferType)
		//	{
		//	case REQUEST:
		//	{
		//		Request_t& req = p_buffer->request[_id];
		//		lockGaurd lock(m_requestMutex);
		//		if (req.type)
		//		{
		//			null_buffer(req.raw, REQUEST_BUFFER_SIZE); // Delete data at the request data loaction. Request instance is stll valid, but type and values will be null.
		//		}

		//		return !req.type;
		//	}
		//	break;

		//	case RESPONCE:
		//	{
		//		Responce_t& resp = p_buffer->responce[_id];
		//		lockGaurd lock(m_requestMutex);
		//		if (resp.results)
		//		{
		//			null_buffer(resp.raw, RESPONCE_BUFFER_SIZE); // Delete data at the request data loaction. Request instance is stll valid, but type and values will be null.
		//			m_freeRequests.emplace_back(_id);
		//
		//		}

		//		return !resp.results;
		//	}
		//	break;


		//	}
		//}

		//void sendResponce(const ResponceType& _result, const ulong& _id, cstring _meta)
		//{
		//	lockGaurd lock(m_responceMutex);
		//	p_buffer->responce[_id].id = _id;
		//	p_buffer->responce[_id].results = _result;
		//	if(_meta)
		//	{
		//		memcpy(p_buffer->responce[_id].metaData, _meta, std::min(1024ull, strlen(_meta)));
		//	}
		//}

		//

		//Core::Core()
		//	:m_isReady{false}, m_isRunning{false}
		//{
		//	if(p_instance) throw Error(LEVEL_2, "Invalid constuction of 'Core'... \nYou can NOT manualy constuct a Core object. Please use 'Core::Get()'");
		//	for(ID i = 0; i < 1024; ++i)
		//		m_freeRequests.push_back(i);
		//
		//	p_buffer = make(ComBuff_t);
		//	memset(p_buffer->request, 0x0, sizeof(Request_t) * 1024);
		//	memset(p_buffer->responce, 0x0, sizeof(Responce_t) * 1024);
		//	//p_instance = CoreInstance{ (Core*)((void*)0xffffffffffffffff), [](Core*) {} }; // Ser to a null value for safety.
		//
		//
		//}


		//const CoreInstance& Core::Get()
		//{
		//	if (!p_instance || p_instance.get() == (void*)0xffffffffffffffff)
		//	{
		//		return p_instance = make_singleton(Core);
		//
		//	}
		//	else return p_instance;
		//}
		//void Core::signalApp(ID _id)
		//{
		//	App::Get()->emplace_back(_id);
		//}
		//void Core::startApp()
		//{
		//	App::Get();
		//}
		//void Core::ready()
		//{
		//	m_isReady = true;
		//	m_cv.notify_all();
		//}
		//void Core::Start()
		//{
		//	if (m_isRunning) return;
		//	m_isRunning = true;
		//	startApp();
		//	p_control = make_a(thread, &Core::_control, this, this);
		//	p_webListener = make_a(thread, &Core::_webListener, this, this);
		//	p_webListener->detach();
		//
		//}
		//void Core::Stop()
		//{
		//	m_isRunning = false;
		//	if (!p_control) return;
		//	App::Get()->Join();
		//	p_control->join();
		//	Shared(App)& t = ((Shared(App)&)App::Get());
		//	t.reset();
		//
		//}

		//void Core::SendRequest(ID& _id, cstring data, RequestType communicationType, cstring metaData)
		//{
		//	_id = sendRequest(data, communicationType, std::min(2048ull, std::strlen(data)), metaData, std::min(1024ull, std::strlen(metaData))) ;
		//}

		//void Core::SendAndWait(ID& _id, cstring data, RequestType communicationType, cstring metaData)
		//{
		//	_id = sendRequest(data, communicationType, std::min(2048ull, std::strlen(data)), metaData, std::min(1024ull, std::strlen(metaData)));
		//	WaitForResponce(_id);
		//}

		//ID Core::sendRequest(cstring _data, RequestType _communicationType, ulong _size, cstring _meta, ulong _metaSize)
		//{
		//	if (!IsReady) ready();
		//	ID _id = 0_nullid;
		//	{
		//		lockUnique lok(m_requestMutex);
		//		m_cv.wait(lok, [this]() { return !m_freeRequests.empty(); });
		//
		//		_id = m_freeRequests.front();
		//		m_freeRequests.pop_front();
		//		lok.unlock();
		//	}
		//	p_buffer->responce[_id].results = UNHANDLED;
		//	p_buffer->request[_id].type = _communicationType;
		//	p_buffer->request[_id].id = _id;


		//	null_buffer(p_buffer->request[_id].data, 2048); // ensures the buffer is clean
		//	memcpy(p_buffer->request[_id].data, _data, ((_size <= 2047) ? _size : 2047));
		//	if (_meta)
		//	{
		//		null_buffer(p_buffer->request[_id].metaData, 1024); // ensures the buffer is clean
		//		memcpy(p_buffer->request[_id].metaData, _meta, ((_metaSize <= 1023) ? _metaSize : 1023));
		//	}
		//
		//	signalApp(_id);
		//	return _id;
		//}

		//void Core::WaitForResponce(const ID& _requestId)
		//{
		//	lockUnique lock(m_responceMutex);
		//	m_cv.wait(lock, [this, &_requestId]() {
		//		return !(p_buffer->responce[_requestId].results & (PENDING | UNHANDLED));
		//		});
		//}
		//void Core::ReadInResponce(const ID& _requestId, Responce& _responceRef)
		//{
		//	_responceRef = p_buffer->responce[_requestId];
		//	deleteComBufferData(_requestId, RESPONCE);
		//}
		//void Core::SetAppReadFrequency(const App::milli& _freq)
		//{
		//	App::Get()->m_freq = _freq;
		//}
