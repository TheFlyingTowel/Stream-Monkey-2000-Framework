#pragma once



namespace SM2K
{


	INN_PROCESS(BaseProcess, u8)
	{
	public:
		virtual ~BaseProcess() = default;
		virtual void configure() = 0;
		virtual void update(_Type, sm2k _registry) = 0;
	};

#define PROCESS(_name) class _name : public BaseProcess
	template<typename T = BaseProcess>
	struct BaseProcessRegistry
	{
		template<typename... Args>
		inline T& Add(const string& _name, Args&&... args)
		{
			auto process = std::make_unique<T>(std::forward<Args>(args)...);
			auto& ref = processes[_name] = std::move(process);
			return *static_cast<T*>(&*ref);
		}

		inline void Clear()
		{
			scheduler.clear();
			processes.clear();
		}

		inline void Remove(const string& _name)
		{

			if (processes[_name]->alive())
			{
				processes[_name]->abort(true);
			}
			processes.erase(_name);
		}

		inline void Update(_Registry& _registry)
		{
			scheduler.update(0, &_registry);
		}
		
		inline void StartProcess(const string& _name)
		{
			if (!Has(_name))
			{
				std::cout << "Couldn't find a process named \"" << _name << "\". Please add it with the 'Add' function";
				return;
			}

			if (processes[_name]->alive())
			{
				std::cout << "Couldn't start process \"" << _name << "\", because it's still running.";
				return;
			}

			scheduler.attach<T>(*processes[_name]);


		}

		inline bool Has(const string& _name)
		{
			return processes.find(_name) != processes.end();
		}

		inline const T* GetProcess(const string& _name)
		{
			if (!Has(_name)) return nullptr;
			return const_cast<T*>(&(*processes.find(_name)->second));
		}

		inline bool IsRegistryEmpty()
		{
			return !scheduler.empty();
		}

		inline u64 GetProcessCount()
		{
			return processes.size();
		}
		inline u64 GetRunningProcessCount()
		{
			return scheduler.size();
		}

		~BaseProcessRegistry() 
		{
			Clear();
		}
	private:

		std::unordered_map<string, std::unique_ptr<T>> processes;
		entt::basic_scheduler<u8> scheduler;

	};

#define PROCESS_REGISTRY(_name, _processType) struct _name : public BaseProcessRegistry<_processType>

};