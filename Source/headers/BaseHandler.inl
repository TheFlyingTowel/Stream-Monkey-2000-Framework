#pragma once


namespace SM2K
{
	
	template<typename ReturnType = void, typename RequestType = void>
	class BaseHandler 
		: public Handler<ReturnType, RequestType>
	{
	public:

	protected:
		HandlerPtr<ReturnType, RequestType> p_nextHandler{ nullptr };


	public:

		/// <summary>
		/// Sets the next handler in the chain.
		/// </summary>
		/// <param name="_next">:  Next handler in the chain.</param>
		/// <returns></returns>
		HandlerPtr<ReturnType, RequestType> SetNext(_HandlerPtr(ReturnType, RequestType)& _next)
		{
			return p_nextHandler = _next ;
		}

		/// <summary>
		/// Handles the request. Passes on request to the next object in the chain if this object can't handle it.
		/// </summary>
		/// <param name="_request">: Request being made.</param>
		/// <returns></returns>
		ReturnType Handle(RequestType& _request)
		{
			if (!IsEnd()) 
			{
				return p_nextHandler->Handle(_request);
			}

			return ReturnType();
		}

		/// <summary>
		/// Tells you if this handler is the last handler in the chain.
		/// </summary>
		/// <returns>End of chain condition.</returns>
		inline const bool IsEnd() const 
		{
			return !p_nextHandler;
		}

	};

};