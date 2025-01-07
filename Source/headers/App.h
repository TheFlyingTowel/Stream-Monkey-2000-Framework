#pragma once


namespace SM2K
{
	namespace Core_layer
	{
		
			using milli = std::chrono::milliseconds;

			// Tags
			struct CloseApp {};
			struct StartApp {};

			// Components
			
			struct smApp
			{
			
				string name;
				milli freq;
				mutex mutex;
				bool running = false;
				Shared(thread) p_app;

			};
			
			

		

	};
};