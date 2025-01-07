#pragma once
#include "CmdTools.inl"
#include "CoreTypes.h"
#include "HashTable.h"


namespace SM2K 
{


	typedef
		enum  TokenType : ID
	{
		EOT				= 0x0, 
		UNKNOWN			= 0x1, 
		NAME			= 0x2, 
		CMD				= 0x4, 
		STRING			= 0x8, 
		INT				= 0x10, 
		FLOAT			= 0x20, 
		DOUBLE			= 0x40, 
		STR_OPEN		= 0x80, 
		STR_CLOSE		= 0x100, 
		OP				= 0x200
	}tkt;

	inline  TokenType operator|(TokenType a, TokenType b)
	{
		return static_cast<TokenType>(static_cast<ID>(a) | static_cast<ID>(b));
	}

	struct Token
	{
		TokenType type;
		string data;
		Hash hash;

		Token()
			:type{ NONE }, data{ "" }, hash{0x0}
		{}
		Token(tkt _type, cstring _data)
			:type{ _type }, data{_data}, hash{HashingFunctions::fnv1a_hash(data.c_str()) }
		{
		}

		inline const Token& operator=(const Token& _ref)
		{

			type = _ref.type;
			data = _ref.data;
			hash = _ref.hash;
			return *this;
		}

		inline const Token& operator=(string& _data)
		{
			data = _data;
			return *this;
		}

		inline Token& operator[](TokenType _type)
		{
			type = _type;
			return *this;
		}

	};


};

