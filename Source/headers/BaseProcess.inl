#pragma once



namespace SM2K
{


	INN_PROCESS(BaseProcess, _Registry*)
	{
	public:
		BaseProcess(ProcessID _id) 
			: id{ _id } {}
		virtual ~BaseProcess() = default;
		virtual void configure() {};
		virtual void update(_Type, sm2k _optional = nullptr) {};
	protected:
		ProcessID id;
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

		_REGENT ref{ &_registry, GGET(_Entity) };
		workers.reserve(_poolSize);
		Print(ref, "Starting worker pool with a size of " + std::to_string(_poolSize), GetContex("Pool", this));
		for (u64 i = 0; i < _poolSize; ++i)
		{
#ifdef NOT _DEBUG
			Print(ref, "Starting worker " + std::to_string(i), GetContex("Pool", this));
#endif
			workers.emplace_back([&, i]{
				
				{
					std::unique_lock<mutex>  _lock(queue_mutex);
#ifdef NOT _DEBUG
					Print(ref, "Now running worker " + std::to_string(i), GetContex("Worker", &workers[i]));
#endif		
					cv.wait(_lock, [this] {return ready ; });
					
				}
				Func task; bool b;
				while (true)
				{
					if (!task.process)
					{
						std::unique_lock<mutex>  _lock(queue_mutex);
						cv.wait(_lock, [this] {return stop || !tasks.empty(); });
						if (stop && tasks.empty()) return;
						
						task = move(tasks.front());
						tasks.pop();
						task.process->tick(&_registry, task.data);
					}
					


					if (task.process && task.process->alive())
						task.process->tick(&_registry, task.data);
					else if(task.process->finished() || task.process->rejected())task.process = nullptr; // Move on if the process has stopped.

				}
				});
		}
	};

	virtual ~BaseProcessPool()
	{
		ready = false;
		Stop();
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

	inline void Stop() 
	{
		if (stop) return;
		{
			std::lock_guard<mutex> _loack(queue_mutex);
			stop = true;
		}

		cv.notify_all();
		for (thread& worker : workers)
			worker.join();
	}

	inline void ReadyUp() 
	{
		{
			std::lock_guard<mutex> _loack(queue_mutex);
			ready = true;
		}
		cv.notify_all();
	}

	private:
		vector(thread) workers;
		queue(Func) tasks;
		mutex queue_mutex;
		std::condition_variable cv;
		bool stop = false;
		bool ready = false;
	};


	using ProcessPoolFunc = std::function<void(_Registry*, sm2k)>;

	template<class _Type = BaseProcess>
	class BaseProcessRegistry
	{
	public:
		BaseProcessRegistry(_Registry& _registry, u32 _poolSize)
			: registry{_registry}, pool{ make_a(BaseProcessPool<_Type>, _registry, _poolSize) }
		{}

		template<typename... Args>
		inline _Type& Add(const string& _name, Args&&... args)
		{
			auto process = make_a(_Type, std::forward<Args>(args)...);
			auto& ref = processes[_name] = move(process);
			return *ref;
		}
		
		template<typename... Args>
		inline BaseProcess& _Add(const string& _name, Args&&... args)
		{
			return *static_cast<BaseProcess*>(&_Add(_name, std::forward<Args>(args)...));
		}

		inline virtual void Clear()
		{
			scheduler.clear();
			processes.clear();
		}

		inline virtual void Remove(const string& _name)
		{
			std::lock_guard<mutex> _lock(processRegistryMutex);
			if (processes[_name]->alive())
			{
				
				processes[_name]->abort(true);
			}
		
			processes.erase(_name);
			
		}
		
		inline void StartProcess(const string& _name)
		{
			std::lock_guard<mutex> _lock(processRegistryMutex);
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
			return static_cast<_Type*>(&*(processes.find(_name)->second));
		}

		inline virtual const _Type*  operator[](const string& _name)
		{
			return GetProcess(_name);
		};
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
			Stop();
			Clear();
		}

		inline void Stop() { pool->Stop(); }
		inline void ReadyUp() { pool->ReadyUp(); }
	protected:

		_Registry& registry;
		std::unordered_map<string, Shared(_Type)> processes;
		_Scheduler scheduler;
		Shared(BaseProcessPool<_Type>) pool;
		mutex processRegistryMutex;
	};

	

#define PROCESS_POOL(_name, _type) class _name : public BaseProcessPool<_type>

#define PROCESS_REGISTRY(_name, _processType) class _name : public BaseProcessRegistry<_processType>

#define LINK(_name) friend class _name

};