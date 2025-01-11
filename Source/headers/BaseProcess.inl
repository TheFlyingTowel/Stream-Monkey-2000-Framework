#pragma once



namespace SM2K
{


	INN_PROCESS(BaseProcess, _Registry*)
	{
	public:
		virtual ~BaseProcess() = default;
		virtual void configure() = 0;
		virtual void update(_Type, sm2k _optional = nullptr) = 0;
	};

#define PROCESS(_name) class _name : public BaseProcess
	



	template<class _Type = BaseProcess>
	class BaseProcessPool
	{
	public:

	struct Func
	{
		Shared(_Type) process = nullptr;
		sm2k data = nullptr;
	};

	BaseProcessPool(_Registry& _registry, u32 _poolSize)
	{

		workers.reserve(_poolSize);

		for (u64 i = 0; i < _poolSize; ++i)
		{
			workers.emplace_back([&] {
				Func task;
				while (true)
				{
					if (!task.process)
					{
						std::unique_lock<mutex>  _lock(queue_mutex);
						cv.wait(_lock, [this] {return stop || !tasks.empty(); });
						if (stop && tasks.empty()) return;
						task = move(tasks.front());
						tasks.pop();
						
					}
					
					if (task.process->alive())
						task.process->tick(&_registry, task.data);
					else if (task.process->finished() || task.process->rejected())
						task.process = nullptr; // Move on if the process stopped.

				}
				});
		}
	};

	virtual ~BaseProcessPool()
	{
		{
			std::lock_guard<mutex> _loack(queue_mutex);
			stop = true;
		}
		cv.notify_all();
		for (thread& worker : workers)
			worker.join();
	}

	inline void Add(Shared(_Type)& _process, const sm2k& _data = nullptr)
	{
		{
			std::lock_guard<mutex> _lock(queue_mutex);
			Func f{(_process), _data};
			tasks.emplace(move(f));
		}
		cv.notify_one();
	}


	private:
		vector(thread) workers;
		queue(Func) tasks;
		mutex queue_mutex;
		std::condition_variable cv;
		bool stop = false;
	};


	using ProcessPoolFunc = std::function<void(_Registry*, sm2k)>;

	template<class _Type = Stream>
	class BaseProcessRegistry
	{
	public:
		BaseProcessRegistry(_Registry& _registry, u32 _poolSize)
			: registry{_registry}, pool{ make_a(BaseProcessPool<_Type>, _registry, _poolSize) }
		{}

		template<typename... Args>
		inline BaseProcess& _Add(const string& _name, Args&&... args)
		{
			auto process = make_a(_Type, std::forward<Args>(args)...);
			auto& ref = processes[_name] = move(process);
			return *static_cast<BaseProcess*>(&*ref);
		}
		
		template<typename... Args>
		inline _Type& Add(const string& _name, Args&&... args)
		{
			return *static_cast<_Type*>(_Add(_name, std::forward<Args>(args)...));
		}

		inline virtual void Clear()
		{
			scheduler.clear();
			processes.clear();
		}

		inline virtual void Remove(const string& _name)
		{

			if (processes[_name]->alive())
			{
				processes[_name]->abort(true);
			}
			processes.erase(_name);
		}

		inline virtual void StartProcess(const string& _name)
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

			scheduler.attach<BaseProcess>(*static_cast<BaseProcess*>(&*processes[_name]));
			pool->Add(processes[_name]);


		}

		inline virtual bool Has(const string& _name)
		{
			return processes.find(_name) != processes.end();
		}

		inline virtual const _Type* GetProcess(const string& _name)
		{
			if (!Has(_name)) return nullptr;
			return reinterpret_cast<_Type*>(&*(processes.find(_name)->second));
		}

		inline virtual bool IsRegistryEmpty()
		{
			return !scheduler.empty();
		}

		inline virtual u64 GetProcessCount()
		{
			return processes.size();
		}
		inline virtual u64 GetRunningProcessCount()
		{
			return scheduler.size();
		}

		inline virtual ~BaseProcessRegistry() 
		{
			Clear();
		}
	protected:

		_Registry& registry;
		std::unordered_map<string, Shared(_Type)> processes;
		_Scheduler scheduler;
		Shared(BaseProcessPool<_Type>) pool;

	};

	

#define PROCESS_POOL(_name, _type) class _name : public BaseProcessPool<_type>

#define PROCESS_REGISTRY(_name, _processType) class _name : public BaseProcessRegistry<_processType>

#define LINK(_name) friend class _name

};