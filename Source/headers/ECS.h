#pragma once

namespace SM2K
{

#define X(x) x
#define EXPAND(a) X(a)

#ifndef REGISTRY
// Use this in arguments to enable the helper preprocessers.
#define REGISTRY _registry	
#endif

#define X(x) x
#define EXPAND(a) X(a)
#define COMBINE_EXPAND(a, b) a##b
#define COMBINE(a, b) COMBINE_EXPAND(a, b)

	namespace ECS
	{
		struct sm2kComponentLogic
		{
			sm2kComponentLogic(ComponentFunc _logic);
		};


#define INN_CONNECT_COMP_LOGIC(func) \
			static void func(_Registry& REGISTRY); \
			namespace{ECS::sm2kComponentLogic COMBINE(__StoreComp_, __COUNTER__)(func);} \
			static void func(_Registry& REGISTRY)

#define CONNECT_COMPONENT() INN_CONNECT_COMP_LOGIC( COMBINE(__LogicDef_, __COUNTER__)) // Used to connect component functions.

		void InitializeComponents(_Registry& REGISTRY);


#define CONNECT_ON_CONSTRUCT(_type, _func) REGISTRY.on_construct<_type>().connect<_func>()			// Used for connecting an init function
#define CONNECT_ON_UPDATE(_type, _func) REGISTRY.on_update<_type>().connect<_func>()				// Used for connecting an update function
#define CONNECT_ON_DESTROY(_type, _func) REGISTRY.on_destroy<_type>().connect<_func>()				// Used for connecting  a destroy function
#define DISCONNECT_ON_CONSTRUCT(_type, _func) REGISTRY.on_construct<_type>().disconnect<_func>()	// Used for disconnecting an init function
#define DISCONNECT_ON_UPDATE(_type, _func) REGISTRY.on_update<_type>().disconnect<_func>()			// Used for disconnecting an update function
#define DISCONNECT_ON_DESTROY(_type, _func) REGISTRY.on_destroy<_type>().disconnect<_func>()		// Used for disconnecting  a destroy function

#define IS(_entity, _type) EXPAND(REGISTRY).all_of<_type>(_entity) 
#define ANY(_entity, ...) EXPAND(REGISTRY).any_of<__VA_ARGS__>(_entity) 
#define GET(_type, _entity) EXPAND(REGISTRY).get<_type>(_entity) 
#define GGET(_type) EXPAND(REGISTRY).ctx().get<_type>() 
#define CGET(_type) GET(_type, GGET(_Entity))
#define DELETE(_type, _entity) EXPAND(REGISTRY).remove<_type>(_entity) 
#define GRAB_ALL(...) EXPAND(REGISTRY).view<__VA_ARGS__>() 
#define GRAB(...) *(EXPAND(REGISTRY).view<__VA_ARGS__>().begin()) 
#define GRAB_AT(_index, ...) EXPAND(REGISTRY).view<__VA_ARGS__>()[_index] 
#define ADD(_type, ...) EXPAND(REGISTRY).emplace<_type>(__VA_ARGS__) 
#define REMOVE(_entity, ...) EXPAND(REGISTRY).remove<__VA_ARGS__>(_entity) 
#define ADD_GLOBAL(_type, ...) EXPAND(REGISTRY).ctx().emplace<_type>(__VA_ARGS__) 

#define SIGNAL_UPDATE(_type, _entity) EXPAND(REGISTRY).patch<_type>(_entity);


#define TAG(_name) struct _name {}
#define COMPONENT(_name) struct _name
#define INN_PROCESS(_name, _deltaType)using _Type = EXPAND(_deltaType);\
									class EXPAND(_name) : public entt::process <EXPAND(_name), _Type>


	}

};
