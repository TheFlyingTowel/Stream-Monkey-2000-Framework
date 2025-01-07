#pragma once

namespace SM2K
{

	class Interpreter;
	struct CmdStatus
	{
		Core_layer::ResponceType status = Core_layer::NULL_RESPONCE;
		Shared(schar) cmd;
		string resultData;
		string msg;

		CmdStatus(Core_layer::ResponceType _status, Shared(schar) _cmd, string _resultData)
		{
			status = _status;

			size_t s;
			cmd = make_s(schar,s = strlen(_cmd.get()));
			memcpy(cmd.get(), _cmd.get(), s);

			resultData = _resultData;
		}
		
		CmdStatus() {}
	};

	using CmdBaseHandle = BaseHandler<CmdStatus, void*>;

	class BaseCmd 
		: public CmdBaseHandle
	{
	public:

		BaseCmd(cstring _cmd, ulong* _counter, Shared(Interpreter)& _interpObject);
		CmdStatus Handle(void* _optional_observer = nullptr);


	 protected:

		virtual bool run(void* _optional_observer = nullptr) = 0; // Executes cmds.

		
		Shared(Interpreter) p_interpObjectRef;
		ulong& ref_counter;
		CmdStatus m_status;
		bool ok = false;
	};

}
