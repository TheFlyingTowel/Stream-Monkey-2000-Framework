#pragma once
namespace SM2K
{
#define nullSignal 0x0
#define defaultSignalState 0xFFFFFFFFFFFF0000;



	class Signal
	{

		enum SignalTypes : ID
		{
			NONE = 0, OPEN = 0x01,  START = 0x02, STOP = 0x04, BACK = 0x08, FORWARD = 0x010, RELOAD = FORWARD | BACK
		};

		typedef
			union SignalBuffer // A signal buffer used for passing communication ids.
		{
			struct
			{
				ID type				: 16;
				uint ch0			: 8;
				uint ch1			: 8;
				uint ch2			: 8;
				uint ch3			: 8;
				uint ch4			: 8;
				uint ch5			: 7;
				uint isActive		: 1;

			};

			ID state[2];
			ulong raw;

			inline const ulong& operator=(const ulong& _ref) 
			{
				return raw = _ref;
			};

			inline operator ulong() 
			{
				return raw;
			}

			inline void set(const ulong& _index, ID _id)
			{
				switch (_index)
				{
				case 0:
				{
					ch0 = _id;
				}
				break;

				case 1:
				{
					ch1 = _id;
				}
				break;

				case 2:
				{
					ch2 = _id;
				}
				break;

				case 3:
				{
					ch3 = _id;
				}
				break;

				
				case 4:
				{
					ch4 = _id;
				}
				break;

				
				case 5:
				{
					ch5 = _id;
				}
				break;


				default:
					return;
				}
				isActive = true;
			}

			inline void remove(const ulong& _index)
			{
				set(_index, nullSignal);
			}
			inline ID operator[](ulong n)
			{
				switch (n)
				{
				case 0:
				{
					return ch0;
				}
				break;

				case 1:
				{
					return ch1;
				}
				break;

				case 2:
				{
					return ch2;
				}
				break;

				case 3:
				{
					return ch3;
				}
				break;

				
				case 4:
				{
					return ch4;
				}
				break;

				
				case 5:
				{
					return ch5;
				}
				break;


				default:
					break;
				}
			}

		}sigBuf;
	
	public:

		const bool& ActiveSignal = m_hasSignal;

		Signal();
		
		const ID GetNextSignal();
		ID PopNextSignal(mutex& _mutex);

		void Reset();
		
		bool SendSiganl(ID _id, mutex& _mutex);
		bool HasSpace();




	private:



		cv m_cv;
		sigBuf m_signal;
		byte m_count = 0;
		byte m_nextFree = 0;
		byte m_nextValid = -1;
		bool m_hasSignal;
	};

}
