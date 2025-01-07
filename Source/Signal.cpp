#include "pch.h"

namespace SM2K
{
	Signal::Signal()
	{
		Reset();
	}
	bool Signal::SendSiganl(ID _id, mutex& _mutex)
	{
		
		lockUnique lock(_mutex, DONT_LOCK);
		m_cv.wait(lock, [this]() {return HasSpace(); });
		
		lock.lock();
		for(int i = m_nextFree; i < 4; ++i)
		{
			if(m_signal[m_nextFree] == nullSignal)
			{
				m_signal.set(m_nextFree = i, _id);
				break;
			}
		}
		lock.unlock();
		
		if (m_signal[m_nextFree] == _id) 
		{
			++m_count;
			m_hasSignal = !!m_count;
		}
		
		return m_hasSignal;
	}
	const ID Signal::GetNextSignal()
	{
		for (int i = m_nextValid + 1; i < 7; ++i)
		{
			if (m_signal[i % 4] != nullSignal)
			{
				return m_signal[m_nextValid = i % 4];
			}
		}
	}
	ID Signal::PopNextSignal(mutex& _mutex)
	{
		if (!m_count) return nullSignal;
		ID rtn = nullSignal;
		
		{
			lockGaurd lock(_mutex);
			rtn = GetNextSignal();
			m_signal.remove(m_nextValid);
			--m_count;

			m_hasSignal = m_signal.isActive = !!m_count;
		}

		m_cv.notify_all();
		return rtn;
	}
	void Signal::Reset()
	{
		m_signal = defaultSignalState; // Set the default buffer layout.
	}
	bool Signal::HasSpace()
	{
		return m_count < 4;
	}
}