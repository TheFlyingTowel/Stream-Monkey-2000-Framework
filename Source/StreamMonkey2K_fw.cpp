// SM2K_Framework.cpp : Defines the entry point for the application.
//

#define SM2K_EXPORTS
#include "pch.h"
namespace SM2K
{

#define DEBUG_MEM_LEAK


	namespace
	{
		SM2K::BinTree<sm2k> registryVerify;

		void AddRegistry(sm2k& _registry)
		{
			if(!registryVerify.Insert(_registry))
				std::cout << "Couldn't insert registry " << PtrToString(_registry) << "." << std::endl;
		}
		void DeleteRegistry(sm2k& _registry)
		{
			if(!registryVerify.Delete(_registry))
				std::cout << "Couldn't delete registry @ " << PtrToString(_registry) << ". (May have already been deleted)" << std::endl;;
		}
		bool VerifyRegistry( const sm2k& _registry)
		{
			return registryVerify.Find(_registry);
		}
	};


	void AllocateAndStartNewInstance(sm2k& _smInstance, bool _enableConsoleLogging)
	{
		AllocateNewInstance(_smInstance, _enableConsoleLogging);
		Start(_smInstance);
	}

	void StopAndDestroyInstance(sm2k& _smInstance)
	{
		Stop(_smInstance);
		FreeRegistry(_smInstance);
	}

	void AllocateNewInstance(sm2k& _registry, bool _enableConsoleLogging)
	{
		if (VerifyRegistry(_registry)) return; //If it's already allocated don't bother doing it again.
		_Registry* reg = new _Registry;
		_Entity core;


		ECS::InitializeComponents(*reg);
		reg->ctx().emplace<_Entity>(core = reg->create());
		reg->emplace<Core_layer::_EnableConsoleLogging>(core).enableConsoleLog = _enableConsoleLogging;
		reg->emplace<Core>(core);

		//EnableConsoleLogging(*reg, _enableConsoleLogging);

		Print({ reg, core }, "A Stream Monkey 2000 registry was successfully allocated!", GetContex("Core", reg));

		_registry = reg;
		AddRegistry(_registry);
	}

	void Start(const sm2k& registry)
	{
		if (!VerifyRegistry(registry)) return;
		_Registry& _registry = *static_cast<_Registry*>(registry);
		auto core = GGET(_Entity);
		auto sch = _registry.create();
		ADD(StreamScheduler, sch); // Creates and adds the stream scheduler.
		ADD(CoreStart, core);
		SIGNAL_UPDATE(Core, core);

	}

	void AddStream(const sm2k& registry, const string& _name)
	{
		if (!VerifyRegistry(registry)) return;
		_Registry& _registry = *static_cast<_Registry*>(registry);
		const auto& streamScheduler = GRAB(StreamScheduler); // TODO: Use a global "StreamScheduler" entity container to avoid vector iterator problems.

		NewStream stream;
		stream.name = _name;

		ADD(NewStream, streamScheduler, stream);
		SIGNAL_UPDATE(StreamScheduler, streamScheduler);

	}

	void StartStream(const sm2k& registry, const string& _name)
	{
		if (!VerifyRegistry(registry)) return;
		_Registry& _registry = *static_cast<_Registry*>(registry);
		auto streamRegistry = *_registry.view<StreamRegistry>().begin();
		GET(StreamRegistry, streamRegistry).StartProcess(_name);
	}

	void ConfigureStream(const sm2k& registry, const string& _name, const string& _trackFile, const string& _type,
						const string& _streamLogPath, string* _statusObserver)
	{
		_Registry& _registry = *static_cast<_Registry*>(registry);

		const auto streamScheduler = GRAB(StreamScheduler);
		const auto sysPaths = GET(smSystemPaths, GGET(_Entity));
		auto& streamRegistry = GET(StreamRegistry, streamScheduler);

		auto process = const_cast<Stream*>(streamRegistry.GetProcess(_name));
		if (process)
		{
			process->configure(sysPaths.streamInstanceDumpPath, _streamLogPath, _trackFile, (_type == "VOD" ? stream_type::VOD : stream_type::LIVE), _statusObserver);
		}
		else Print({ &_registry, streamScheduler }, "Couldn't configure \"" + _name + "\". Not found in stream registry.", GetContex("StreamRegistry", &streamRegistry));


	}

	void Stop(const sm2k& registry)
	{
		if (!VerifyRegistry(registry)) return;
		_Registry& _registry = *static_cast<_Registry*>(registry);
		auto core = GGET(_Entity);
		auto sch = GRAB(StreamRegistry);
		GET(StreamRegistry, sch).Stop(); // This will ensure the other threads end before everything gets deleted.
		ADD(CoreStop, core);
		SIGNAL_UPDATE(Core, core);
		_registry.destroy(sch);
	}

	void FreeRegistry(sm2k& registry)
	{
		if (!VerifyRegistry(registry))
		{
			std::cout	<< "FreeRegistry" << " could not verify a registry @ " << PtrToString(registry)
						<< ". This registry may have been deleted." << std::endl;
			return;
		}
		_Registry& _registry = *static_cast<_Registry*>(registry);
		_registry.clear();
		DeleteRegistry(registry);
		delete &_registry;
		registry = nullptr;
	}

	void EnableConsoleLog(const sm2k& _registry, bool _state)
	{
		if (!VerifyRegistry(_registry)) return;
		EnableConsoleLogging(*(static_cast<_Registry*>(_registry)), _state);
	}

};





int main(int argsc, char** args) // For Testing
{
	using namespace SM2K;
	using namespace Core_layer;

#ifdef DEBUG_MEM_LEAK
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1);
#endif // DEBUG_MEM_LEAK




	sm2k test = nullptr;
	sm2k test2 = nullptr;
	sm2k test3 = nullptr;
	sm2k test4 = nullptr;
	sm2k test5 = nullptr;

	std::cout << SM2K::GetAppDataFolder() << std::endl;

	AllocateAndStartNewInstance(test, true);

	AddStream(test, "Lobby");
	AddStream(test, "Test");
	AddStream(test, "Wave");
	AddStream(test, "Ch0");
	ConfigureStream(test, "Lobby", "LobbyTrack", "LIVE");
	ConfigureStream(test, "Test", "TestTrack", "LIVE");
	ConfigureStream(test, "Wave", "WaveTrack", "LIVE");
	ConfigureStream(test, "Ch0", "Ch0Track", "LIVE");

	StartStream(test, "Lobby");






	StopAndDestroyInstance(test);






	return 0;
}



	/*std::vector<cstring> names = {
	"John", "Jane", "Alex", "Emily", "Chris", "Katie", "Michael", "Sarah",
		"David", "Laura", "James", "Hannah", "Daniel", "Samantha", "Robert", "Olivia",
		"Matthew", "Megan", "Andrew", "Jessica", "Joshua", "Amanda", "Joseph", "Brittany",
		"Ryan", "Lauren", "Jacob", "Victoria", "Brandon", "Natalie", "William", "Nicole",
		"Ethan", "Sophia", "Benjamin", "Ava", "Samuel", "Isabella", "Anthony", "Madison",
		"Gabriel", "Abigail", "Jack", "Elizabeth", "Nicholas", "Chloe", "Alexander", "Grace",
		"Elijah", "Alyssa", "Logan", "Lily", "Christian", "Anna", "Jonathan", "Lillian",
		"Nathan", "Addison", "Justin", "Ella", "Austin", "Mia", "Dylan", "Hailey",
		"Caleb", "Kayla", "Zachary", "Zoe", "Aaron", "Leah", "Tyler", "Sofia",
		"Lucas", "Brooke", "Gavin", "Natalia", "Connor", "Amelia", "Hunter", "Violet",
		"Isaiah", "Audrey", "Evan", "Julia", "Henry", "Bella", "Charles", "Aria",
		"Adrian", "Scarlett", "Josiah", "Maya", "Jordan", "Lucy", "Cameron", "Stella",
		"Brayden", "Alexis", "Luke", "Layla", "Avery", "Mason", "Claire",
		"Smith", "Johnson", "Williams", "Jones", "Brown", "Davis", "Miller", "Wilson",
		"Moore", "Taylor", "Anderson", "Thomas", "Jackson", "White", "Harris", "Martin",
		"Thompson", "Garcia", "Martinez", "Robinson", "Clark", "Rodriguez", "Lewis", "Lee",
		"Walker", "Hall", "Allen", "Young", "Hernandez", "King", "Wright", "Lopez",
		"Hill", "Scott", "Green", "Adams", "Baker", "Gonzalez", "Nelson", "Carter",
		"Mitchell", "Perez", "Roberts", "Turner", "Phillips", "Campbell", "Parker", "Evans",
		"Edwards", "Collins", "Stewart", "Sanchez", "Morris", "Rogers", "Reed", "Cook",
		"Morgan", "Bell", "Murphy", "Bailey", "Rivera", "Cooper", "Richardson", "Cox",
		"Howard", "Ward", "Torres", "Peterson", "Gray", "Ramirez", "Watson",
		"Brooks", "Kelly", "Sanders", "Price", "Bennett", "Wood", "Barnes", "Ross",
		"Henderson", "Coleman", "Jenkins", "Perry", "Powell", "Long", "Patterson", "Hughes",
		"Flores", "Washington", "Butler", "Simmons", "Foster", "Gonzales", "Bryant",
		"Russell", "Griffin", "Diaz", "Hayes", "Myers", "Ford", "Hamilton", "Graham"
	};

	struct HashPair
	{
		cstring name;
		uint count;
	};

	std::unordered_map<Hash, HashPair> hashes;
	std::vector<uint> counts;
	for( cstring s : names)
	{
		Hash hash = HashingFunctions::DefaultHash(s);

		hashes[hash].count += 1;

		if (hashes[hash].count > 1)
		{

			std::cout << "\nCorrent Name:\t" << s << "\n";
			std::cout << "\Collided Name:\t" << hashes[hash].name << "\n";
			std::cout << "Count:\t" << hashes[hash].count << "\n";
			std::cout << "Hash:\t0x" << std::hex << hash << "\n";
		}else
		{
			hashes[hash].name = s;
		}
	}

	*/

	/*CmdTable tbl;

	tbl["ECHO"] = Token(CMD, "ECHO");
	tbl["echo"] = Token(CMD, "echo");
	tbl["eh"] = Token(CMD, "eh");

	if(tbl.contains("echo"))
	{
		std::cout << "Working\n";
	}*/

	//std::cout << "0x" << std::hex << (HashingFunctions::DefaultHash("Jackson")) << "\n";
	//std::cout << "0x" << std::hex << (HashingFunctions::DefaultHash("Johnson")) << "\n";
	//std::cout << "0x" << std::hex << (HashingFunctions::DefaultHash("start")) << "\n";
	//std::cout << "0x" << std::hex << (HashingFunctions::DefaultHash("st")) << "\n";
	//std::cout << "0x" << std::hex << (HashingFunctions::DefaultHash("echo")) << "\n";
	//std::cout << "0x" << std::hex << (HashingFunctions::DefaultHash("still")) << "\n";
	//std::cout << "0x" << std::hex << (HashingFunctions::DefaultHash("strain")) << "\n";
