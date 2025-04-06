#pragma once

#define SKP_FW 0x8000000000000000
#define SKP_BK 0x4000000000000000
#define REPLAY 0x2000000000000000
#define PAUSE_ 0x1000000000000000
#define RESET_ 0x800000000000000
#define STOP__ 0x400000000000000
#define PLAY__ 0x200000000000000

namespace SM2K
{




	enum class StreamType : u32
	{
		NONE = 0, LIVE, VOD
	};



	// Tags //
	TAG(StreamScheduler);


	// Command Tags //
	COMPONENT(Source) { string source; };
	COMPONENT(SourceCollection) { vector(_ID) collection; };

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
			:BaseProcess(_id)
		{
		}
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

		void update(_Registry* registry, sm2k) override
		{

			_Registry& _registry = *registry;

			//TODO: Find a way to implement direct stream controls with the "libavformat" lib.

			if (cmdBuffer)
			{
				u64 state = (cmdBuffer & (1 << GetEarilestSetBitIndex64(cmdBuffer)));

				switch (state)
				{
				case SKP_FW:
					break;

				case SKP_BK:
					break;
					
				case REPLAY:
					break;
					
				case PAUSE_:
					break;
					
				case PLAY__:
					break;
					
				case STOP__:
					break;
					
				case RESET_:
					break;
					
				default:
					break;
				}


			}


			////TODO: Add hls frame prossecing and time logic.
			//ADD(FilePath, id).path = "./data/testCompression.txt";
			//ADD(ShowCompressConfig, id).name = "Test";
			//auto& com = ADD(_Compression, id, _registry, id);

			//string read = "~";
			////com.CompressByLine({ "Padding","Teast0","test2","test3","test11","test100" });
			//com.ReadByLine(read);
			//std::cout << read << std::endl;

			//com.End();

			succeed();
		}

		inline void send(Hash _request, size_t _size) 
		{
			switch (_request)
			{
				case TOKEN(SkipFW):
				{
					cmdBuffer |= SKP_FW;
					cmdBuffer |= ((_size << 8) & 0xFFFFFF00);
				}
				break;

				case TOKEN(SkipBW):
				{
					cmdBuffer |= SKP_BK;
					cmdBuffer |= ((_size << 8) & 0xFFFFFF00);
				}
				break;

				case TOKEN(Replay):
				{
					cmdBuffer |= REPLAY;
					cmdBuffer |= ((_size << 8) & 0xFFFFFF00);
				}
				break;

				case TOKEN(Pause):
				{
					cmdBuffer |= PAUSE_;
					cmdBuffer |= ((_size << 8) & 0xFFFFFF00);
				}
				break;

				case TOKEN(Play):
				{
					cmdBuffer |= PAUSE_;
					cmdBuffer |= ((_size << 8) & 0xFFFFFF00);
				}
				break;
				
				case TOKEN(Stop):
				{
					cmdBuffer |= STOP__;
					cmdBuffer |= ((_size << 8) & 0xFFFFFF00);
				}
				break;
				
				case TOKEN(Rest):
				{
					cmdBuffer |= RESET_;
					cmdBuffer |= ((_size << 8) & 0xFFFFFF00);
				}
				break;
				
				default:
				break;
			}
		};

		bool IsConfigured() const { return isConfigured; }

	private:
		bool isConfigured = false;
		_Registry* registryRef = nullptr;
		GW::SYSTEM::GLog streamLog;
		u64 cmdBuffer = 0x0;

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
