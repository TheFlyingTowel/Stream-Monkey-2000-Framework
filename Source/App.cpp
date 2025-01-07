#define SM2K_USING_CMD_INIT

#include "pch.h"

namespace SM2K 
{

#define INIT_TOKEN(_token, _type) Interpreter::s_cmdTable[_token] = Token(_type, _token)




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



	//namespace Core_layer
	//{
	//	
	//	using namespace std::chrono_literals;
	//	


	//	void _appThread()
	//		{
	//			INIT_TOKEN("'", OP);
	//			INIT_TOKEN("\"", OP);
	//			INIT_TOKEN("\0", EOT);
	//			// TODO: Place non cmd tokens here. ^^^
	//			INIT_CMDS; // <- Inits command tokens

	//			//TODO: Init all tokens here. ^^^
	//			


	//			//Core& core = *Core::Get();

	//			//lockUnique lock(m_mutex);
	//			//core.m_cv.wait(lock, [&core]() {return core.IsReady; });
	//			//lock.unlock();
	//			//while (core.IsRunning)
	//			//{
	//			//	if (!empty())
	//			//	{
	//			//		Core& core = *Core::Get();
	//			//		for (ID id : *this)
	//			//		{
	//			//			// process request

	//			//			const Request_t& request = *core.readRequest(id);


	//			//			switch (request.type)
	//			//			{
	//			//			case CMD_REQUEST:
	//			//			{
	//			//				RequestPtr request = core.readRequest(id);
	//			//				osstream status;
	//			//				status << "Command status: PENDING, Command String: " << request->data;
	//			//				core.sendResponce(PENDING, id, status.str().c_str());

	//			//				core.m_cv.notify_all();
	//			//				Interpreter interp(request->data, CMD_REQUEST);
	//			//				core.sendResponce(OK, id, status.str().c_str());
	//			//				
	//			//			}
	//			//			break;

	//			//			

	//			//			case NET_REQUEST:
	//			//			{
	//			//				core.sendResponce(OK, id, "net request..");
	//			//				
	//			//			}
	//			//			break;
	//			//			}
	//			//			core.m_cv.notify_all();
	//			//			core.deleteComBufferData(id, REQUEST);
	//			//		}
	//			//		clear(); // Clear out signal buffer when done.
	//			//	}
	//			//	
	//			//	std::this_thread::sleep_for(m_freq);
	//			//}
	//		}
	//	
	//
	//	void Construct_App(_Registry& _registry, _Entity e)
	//	{
	//		auto& self = GET(smApp, e);
	//		self.p_app = make(thread);

	//	}

	//	void Signal_App(_Registry& _registry, _Entity e)
	//	{

	//	}

	//	void Destroy_App(_Registry& _registry, _Entity e)
	//	{
	//		auto& self = GET(smApp, e);
	//		self.running = false;
	//		self.p_app->join();

	//	}


	//	CONNECT_COMPONENT() 
	//	{
	//		CONNECT_ON_CONSTRUCT(smApp, Construct_App);
	//		CONNECT_ON_UPDATE(smApp, Signal_App);
	//		CONNECT_ON_DESTROY(smApp, Destroy_App);
	//	}
	//
	//}
};