#include "pch.h"

namespace SM2K 
{


	BaseCmd::BaseCmd(cstring _cmd, ulong* _counter, Shared(Interpreter)& _interpObject)
		: ref_counter{*_counter}, p_interpObjectRef{_interpObject}
	{
		ulong size = strlen(_cmd);
		m_status.cmd = make_s(schar, size);
		memcpy(m_status.cmd.get(), _cmd, size);
		m_status.status = Core_layer::UNHANDLED;
	}

	CmdStatus BaseCmd::Handle(void* _optional_observer)
	{
		if (ok = run(_optional_observer) && !IsEnd()) 
		{
			m_status.resultData += "\n Command number " + std::to_string(ref_counter++) + ": OK\n"; // Append status to current result string buffer
			((BaseCmd*)p_nextHandler.get())->m_status.resultData = m_status.resultData; // Update the next cmd object in chain with the execution status history.
			return CmdBaseHandle::Handle(_optional_observer); // Move onto next command in the chain 
		}
		else if(!ok) // If we have an error.
		{

			m_status.resultData += "\n Command number " + std::to_string(ref_counter + 1) + ": ERROR\n";
			// TODO: Add error logger.
			return CmdBaseHandle::Handle(_optional_observer); // Move onto next command in the chain 
		}
			return m_status;
		
	}

}