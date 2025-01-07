#pragma once



namespace SM2K
{

	typedef _Entity		StreamID;

	// Tags //
	TAG(StreamScheduler);
	
	
	// Command Tags //
	COMPONENT(CloseStream) { string name; };
	COMPONENT(RebootStream) { string name; };
	COMPONENT(OpenStream) { string name; };
	COMPONENT(NewStream) { string name; u16 size; };
	COMPONENT(SkipBackStream) { string name; u32 skipBackCount; };
	COMPONENT(SkipForwardStream) { string name; u32 skipForwardCount; };
	COMPONENT(ReplaySourceStream) { string name; u32 replayCount; };

	// Components //
	COMPONENT(StreamInstance) 
	{
		string name; string instancePath; string streamLogPath; 
		string trackFile; string* streamStatus = nullptr; 
	};


	// Process //

	PROCESS(Stream)
	{
	public:
		Stream(const StreamID & _id)
			: id{ _id }
		{}

		void init(_Registry& _registry) 
		{
			instanceRef = &ADD(StreamInstance, id);
			
		}
		void configure() override {};
		void configure(const string& _name, const string& _instancePath, const string& _streamLogPath,
						const string& _trackFile, const string* _statusObserver = nullptr)
		{
			auto& self = *instanceRef;
			self.name = _name;
			self.instancePath = _instancePath;
			self.streamLogPath = _streamLogPath;
			self.trackFile = _trackFile;
			self.streamStatus = const_cast<string*>(_statusObserver);

		}
		void failed() {}
		void aborted() {}
		void succeeded() {}

		void update(_Type, sm2k _registry)
		{


			//TODO: Add hls frame prossecing and time logic.


			//succeed();
		}

	private:
		StreamID id;
		StreamInstance* instanceRef = nullptr;
		GW::SYSTEM::GLog streamLog;
	};
};
