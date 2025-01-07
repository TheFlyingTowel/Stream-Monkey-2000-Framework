#include <memory>
/*
* Stream Monkey 2000: Handler
*
* SM2K handler interface.
*
* Author:						Tourel A Britton
* Author email:					britton.tourel@gmail.com
*
* Note:
* License: N/A
*
*/
namespace SM2K 
{
	


#define _HandlerPtr(_retrun, _request)				std::shared_ptr<Handler<_retrun, _request>>

	template<typename ReturnType = void, typename RequestType = void>
	class Handler
	{
	public:
	

		virtual _HandlerPtr(ReturnType, RequestType) SetNext(_HandlerPtr(ReturnType, RequestType)*&) = 0;
		virtual ReturnType Handle(RequestType&) = 0;
	};

	template<typename ReturnType = void, typename RequestType = void>
	using HandlerPtr = _HandlerPtr(ReturnType, RequestType);

};
