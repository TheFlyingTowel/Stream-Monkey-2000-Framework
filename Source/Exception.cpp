
#include "pch.h"

namespace SM2K
{
	Exception::Exception(ErrorType _type, string _msg)
		:string(_msg), m_type{_type}
	{
	}
	Exception::~Exception()
	{
	}
	Exception::Exception(const Exception&  _ref)
	{
		*this = _ref;
	}
	const Exception& Exception::operator=(const Exception& _ref)
	{
		m_type = _ref.m_type;
		*((string*)this) = _ref;
		return *this;
	}
}
