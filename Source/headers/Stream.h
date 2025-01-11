#pragma once



namespace SM2K
{

	

	
	enum class StreamType : u32
	{
		NONE = 0, LIVE, VOD
	};

	

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
		string trackFile; string hlsPath; string* streamStatus = nullptr;
		StreamType type = StreamType::NONE;
	};


	// Process //
	PROCESS(Stream)
	{
	public:


		Stream(const ProcessID & _id)
			: id{ _id }
		{}
		~Stream() {};

		void init(_Registry& _registry, const string& _name = "<Unnamed>")
		{
			registryRef = &_registry;
			ADD(StreamInstance, id).name = _name;
			isConfigured = false;
			
		}

		void configure() override {};
		void configure(const string& _instancePath, const string& _streamLogPath,
						const string& _trackFile, const StreamType& _type, string* _statusObserver = nullptr, bool _store = true)
		{
			auto& self = registryRef->get<StreamInstance>(id);
			auto core = registryRef->ctx().get<_Entity>();
			auto& sys = registryRef->get<Core_layer::smSystemPaths>(core);

			self.instancePath = _instancePath + self.name + "/";
			self.streamLogPath = self.instancePath + _streamLogPath;
			self.hlsPath = sys.hlsBaseDumpPath + self.name + "/";
			self.streamStatus = _statusObserver;
			self.trackFile = _trackFile;
			self.type =  _type;

			

			isConfigured = true;

			Print({ registryRef, id }, "Stream \"" + self.name + "\" configured", GetContex("Stream", this));
			if (_store) storeConfig();
		}
		void failed() {}
		void aborted() {}
		void succeeded() {}

		void storeConfig()
		{
			auto& self = registryRef->get<StreamInstance>(id);
			_Config config(_REGENT{ registryRef, id }, true);

			config[self.name + "_Config"]["name"] = self.name;
			config[self.name + "_Config"]["instancePath"] = self.instancePath;
			config[self.name + "_Config"]["hlsPath"] = self.hlsPath;
			config[self.name + "_Config"]["logPath"] = self.streamLogPath;
			config[self.name + "_Config"]["trackFile"] = self.trackFile;
			config[self.name + "_Config"]["streamType"] = (self.type == StreamType::LIVE ? "LIVE" : "VOD"); // A propper function will be needed if more types are added.

			std::filesystem::create_directory(self.instancePath);
			std::filesystem::create_directory(self.hlsPath);
			std::filesystem::create_directory(self.streamLogPath);



			config.save(self.instancePath + self.name + ".ini");
			registerWithCore();
			
			
		}

		void registerWithCore() 
		{
			auto& self = registryRef->get<StreamInstance>(id);
			auto& coreConfig = *(registryRef->get<Core_layer::smConfig>(registryRef->ctx().get<_Entity>()).config); 

			coreConfig["RegisteredStreams"][self.name] = self.instancePath + self.name + ".ini";
			coreConfig.save("./core.ini");
			Print({ registryRef, id }, "Registered \"" + self.name + "\" in core", GetContex("Core", this));
		}

		void update(_Registry*, sm2k)
		{


			//TODO: Add hls frame prossecing and time logic.


			//succeed();
		}

		bool IsConfigured() const { return isConfigured; }

	private:
		bool isConfigured = false;
		ProcessID id;
		_Registry* registryRef = nullptr;
		GW::SYSTEM::GLog streamLog;


		LINK(StreamRegistry);

	};

	// Registry //

	PROCESS_REGISTRY(StreamRegistry, Stream) 
	{
	public:
		StreamRegistry(_Registry& _registry, u32 _poolSize = 16) 
			:BaseProcessRegistry(_registry, _poolSize)
		{}



		void Clear() override
		{
			for (auto& [name, stream] : processes)
				registry.destroy(((Stream*)&*stream)->id);

			((BaseProcessRegistry<Stream>*)this)->Clear();
		}


	};



	using stream_type = StreamType;

};
