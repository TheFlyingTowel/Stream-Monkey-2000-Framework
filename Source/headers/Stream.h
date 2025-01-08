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

		void init(_Registry& _registry, const string& _name = "<Unnamed>")
		{
			registryRef = &_registry;
			ADD(StreamInstance, id).name = _name;
			isConfigured = false;
			
		}

		void configure() override {};
		void configure(const string& _instancePath, const string& _streamLogPath,
						const string& _trackFile, string* _statusObserver = nullptr)
		{
			_Config config(_REGENT{registryRef, id}, true);
			auto& self = registryRef->get<StreamInstance>(id);
			
			self.instancePath = _instancePath + self.name + "/";
			self.streamLogPath = self.instancePath + _streamLogPath;
			self.trackFile = _trackFile;
			self.streamStatus = _statusObserver;
			
			config["StreamConfig"]["name"] = self.name;
			config["StreamConfig"]["instancePath"] = self.instancePath;
			config["StreamConfig"]["trackFile"] = self.trackFile;
			config["StreamConfig"]["isLive"] = true;

			config.save(_instancePath);

			isConfigured = true;

			Print({ registryRef, id }, "Stream \"" + self.name + "\" configured.", GetContex("Stream", this));

		}
		void failed() {}
		void aborted() {}
		void succeeded() {}

		void update(_Type, sm2k _registry)
		{


			//TODO: Add hls frame prossecing and time logic.


			//succeed();
		}

		bool IsConfigured() const { return isConfigured; }

	private:
		bool isConfigured = false;
		StreamID id;
		_Registry* registryRef = nullptr;
		GW::SYSTEM::GLog streamLog;
	};

	// Registry //

	PROCESS_REGISTRY(StreamRegistry, Stream) {};

};
