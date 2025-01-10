#pragma once



namespace SM2K
{


	INN_PROCESS(BaseProcess, _Registry&)
	{
	public:
		virtual ~BaseProcess() = default;
		virtual void configure() = 0;
		virtual void update(_Type, sm2k _optional = nullptr) = 0;
	};

#define PROCESS(_name) class _name : public BaseProcess
	



	template<typename _Type = BaseProcess>
	class BaseProcessPool
	{
	public:

	struct Func
	{
		_Type* process = nullptr;
		sm2k data = nullptr;
	};

		BaseProcessPool(_Registry& _registry, ProcessID _id, u64 _poolSize)
			:id{ _id }
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
							task = std::move(tasks.front());
							tasks.pop();
						}

						if (task.process->alive())
							task.process->tick(_registry, task.data);
						else if (task.process->finished() || task.process->rejected())
							task.process = nullptr; // Move on if the process stopped.

					}
					});
			}
		}


		void Add(BaseProcess*&& _process, const sm2k& _data = nullptr)
		{
			{
				std::lock_guard<mutex> _lock(queue_mutex);
				Func f{ std::forward<BaseProcess*>(_process), _data };
				tasks.emplace(std::move(f));
			}
			cv.notify_one();
		}

		~BaseProcessPool()
		{
			{
				std::lock_guard<mutex> _loack(queue_mutex);
				stop = true;
			}
			cv.notify_all();
			for (thread& worker : workers)
				worker.join();
		}


	private:
		vector(thread) workers;
		queue(Func) tasks;
		mutex queue_mutex;
		std::condition_variable cv;
		bool stop = false;
		ProcessPoolID id;

		
	};


	using ProcessPoolFunc = std::function<void(_Registry&, sm2k)>;

	template<typename _Type = BaseProcess>
	struct BaseProcessRegistry
	{

		BaseProcessRegistry(_Registry& _registry, u64 _size)
		{
			auto id = _registry.create();
			pool = make_a(BaseProcessPool<_Type>, ADD(BaseProcessPool<_Type>, id, { _registry, id, _size }));
		}

		template<typename... Args>
		inline _Type& Add(const string& _name, Args&&... args)
		{
			auto process = std::make_unique<_Type>(std::forward<Args>(args)...);
			auto& ref = processes[_name] = std::move(process);
			return *static_cast<_Type*>(&*ref);
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

		inline void Update(_Registry& _registry, sm2k _optional = nullptr)
		{
			scheduler.update(_registry, _optional);
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

			scheduler.attach<_Type>(*processes[_name]);


		}

		inline bool Has(const string& _name)
		{
			return processes.find(_name) != processes.end();
		}

		inline const _Type* GetProcess(const string& _name)
		{
			if (!Has(_name)) return nullptr;
			return const_cast<_Type*>(&(*processes.find(_name)->second));
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

		std::unordered_map<string, std::unique_ptr<_Type>> processes;
		_Scheduler scheduler;
		Shared(BaseProcessPool<_Type>) pool;

	};

	

#define PROCESS_POOL(_name, _type) class _name : public BaseProcessPool<_type>

#define PROCESS_REGISTRY(_name, _processType) struct _name : public BaseProcessRegistry<_processType>

};