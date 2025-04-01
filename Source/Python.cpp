#include "pch.h"

namespace SM2K 
{

	namespace PY
	{
		bool wasPyInitAlready = false; // This ensures python only inits once no matter how manny registries we create.
		mutex pyMutex;

		constexpr const auto POINTER_CHAR = ((char)175);

		void Open_Python(_Registry & _registry, _Entity e)
		{
			lockGaurd lck(pyMutex);

			auto& log = GET(_Log, e); // This is fine since the python should only ever be attached to a core.

			if(!wasPyInitAlready)
			{

				int ndx = 0, size = funcSize(PYTHON_FUNCTIONS), errorCount = 0;
				py::initialize_interpreter();
				
			resume:
				try
				{
					while(ndx < size)
						py::exec(PYTHON_FUNCTIONS[ndx++]);	
					
				}
				catch (const std::exception& ee) 
				{
					oss out;
					string tmp = PYTHON_FUNCTIONS[ndx - 1];
					++errorCount;
					out << "Python error occurred at \'" << tmp.substr(1,tmp.find('\n') - 1) << "\'\n" << POINTER_CHAR << POINTER_CHAR << POINTER_CHAR << " " << ee.what() << std::endl;
					Print(_REGENT{ &_registry, e }, out.str(), GetContex("Python-Error", &e));

					if (ndx < size) goto resume; // Keep going if we still have more to execute.
					
				}
				wasPyInitAlready = true;
				if(errorCount)
					Print(_REGENT{ &_registry, e }, "There were " + std::to_string(errorCount) + " python function(s) with errors.", GetContex("Python", &e));

				Print(_REGENT{ &_registry, e }, "Initalized the python interpreter.", GetContex("Python", &e));
			}
		}

		void Close_Python(_Registry & _registry, _Entity e)
		{
			lockGaurd lck(pyMutex);

			if (wasPyInitAlready)
			{
				py::finalize_interpreter();
				wasPyInitAlready = false;
			}
		}


		CONNECT_COMPONENT()
		{
			CONNECT_ON_CONSTRUCT(Python, Open_Python);
			CONNECT_ON_DESTROY(Python, Close_Python);
		}


	}


};

