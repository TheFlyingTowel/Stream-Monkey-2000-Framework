#include "pch.h"


namespace SM2K 
{

	
	void Construct_Scheduler(_Registry& REGISTRY, _Entity e)
	{
		ADD(StreamRegistry, e);
	}
	
	void Update_Scheduler(_Registry& REGISTRY, _Entity e) 
	{

	}

	void Destroy_Scheduler(_Registry& REGISTRY, _Entity e)
	{
		
	}




	// Stream functions

	void Attach_Stream(_Registry& REGISTRY, _Entity e)
	{
		//TODO: make multi thread safe.
		//TODO: Add source loading logic.
		auto& streamName = GET(NewStream, e).name;
		auto& streamRegistry = GET(StreamRegistry, e);
		auto& stream = streamRegistry.Add<Stream>(streamName, REGISTRY.create());
		stream.init(REGISTRY, streamName);
		Print(_REGENT{&REGISTRY, e}, "Attached a stream named \"" + streamName + "\".", GetContex("Stream", &stream));
		DELETE(NewStream, e); // Clean up
		
	}
	void run_SkipBackStream(_Registry& REGISTRY, _Entity e)
	{
		auto& backCount = GET(SkipBackStream, e).skipBackCount;
		//DELETE(SkipBackStream, e);
	}
	
	void run_SkipForwardStream(_Registry& REGISTRY, _Entity e)
	{
		auto& forwardCount = GET(SkipForwardStream, e).skipForwardCount;
		//DELETE(SkipForwardStream, e);
	}
		
	void run_ReplaySourceStream(_Registry& REGISTRY, _Entity e)
	{
		auto& replayCount = GET(ReplaySourceStream, e).replayCount;
		//DELETE(ReplaySourceStream, e);
	}

	// TODO: Add stream command behaviours here ^^^

	CONNECT_COMPONENT()
	{
		CONNECT_ON_CONSTRUCT(StreamScheduler, Construct_Scheduler);
		CONNECT_ON_UPDATE(StreamScheduler, Update_Scheduler);
		CONNECT_ON_DESTROY(StreamScheduler, Destroy_Scheduler);


		CONNECT_ON_UPDATE(NewStream, Attach_Stream);
		CONNECT_ON_UPDATE(SkipBackStream, run_SkipBackStream);
		CONNECT_ON_UPDATE(SkipForwardStream, run_SkipForwardStream);
		CONNECT_ON_UPDATE(ReplaySourceStream, run_ReplaySourceStream);
	}

}
