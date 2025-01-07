#pragma once
#include "Token.inl"
#include "BaseCmd.h"

namespace SM2K
{





	using namespace Core_layer;

	
	using CmdTable = StringTable <Token>;
	typedef vector(Token)	TokenString;
	typedef Shared(Token)	TokenCString;
	
#define INIT_TOKEN(_token, _type) Interpreter::s_cmdTable[_token] = Token(_type, _token)



	class Interpreter
	{
	public:

		const TokenString& TokenizedData;
		const string& UnTokenizedData;

		Interpreter(const _REGENT& _reg, const string& _data, RequestType _requestType);


		static CmdTable s_cmdTable;


	private:

		uint m_pos = 0;
		uint m_tpos = 0;
		TokenString m_currentArgList;
		string m_unTokenizedData;
		TokenString m_TokenizedData;
		Token m_current;
		ReqType m_requestType;
		_Registry& r_registry;
		_Entity r_entity;

		void _tokenize();
		void _interprete();
		void eat(TokenType);
	};

}
