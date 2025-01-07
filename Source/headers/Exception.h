#pragma once

#include "SM2K.h"

namespace SM2K 
{

	typedef
	enum ExceptionType : byte
	{
		LEVEL_0 = 1,
		LEVEL_1,
		LEVEL_2,
		SYNTAX
	}ErrorType;


	typedef
	class Exception : public string
	{
	public:

		Exception(ErrorType _type, string _msg = "Error message not set...");
		~Exception();
		Exception(const Exception&);
		const Exception& operator=(const Exception& _ref);

		inline const ErrorType& GetType() const { return m_type; }
		inline string GetMsg() 
		{
			return *this;
		}


	protected:
		ErrorType m_type;

	} Error;
};