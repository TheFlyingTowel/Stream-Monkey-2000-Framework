// SM2K_Framework.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#define SM2K_FRAMEWORK	
	

#ifdef SM2K_FRAMEWORK

#ifdef SM2K_EXPORTS
#define SM2K_FRAMEWORK_DLL	__declspec(dllexport)
#else
#define SM2K_FRAMEWORK_DLL	__declspec(dllimport)
#endif

#define		SM2K_REQUES_NULL		0
#define		SM2K_REQUES_CMD			1
#define		SM2K_REQUES_LOCAL		2
#define		SM2K_REQUES_NET			3

#define		SM2K_RESPONCE_NULL				0x00
#define		SM2K_RESPONCE_OK				0x01
#define		SM2K_RESPONCE_ERROR				0x02
#define		SM2K_RESPONCE_WARN				0x04
#define		SM2K_RESPONCE_PENDING			0x08
#define		SM2K_RESPONCE_UNHANDLED			0x10

#endif // SM2K_FRAMEWORK
namespace SM2K 
{
	typedef void* sm2k;	// The stream Monkey 2000 Core registry type. 


	extern "C" SM2K_FRAMEWORK_DLL void AllocateAndStartNewInstance(sm2k& _smInstance, bool _enableConsoleLogging = false);
	extern "C" SM2K_FRAMEWORK_DLL void StopAndDestroyInstance(sm2k& _smInstance);
	
	extern "C" SM2K_FRAMEWORK_DLL void AllocateNewInstance(sm2k& _registry, bool _enableConsoleLogging = false);
	extern "C" SM2K_FRAMEWORK_DLL void Start(const sm2k& _registry);
	
	extern "C" SM2K_FRAMEWORK_DLL void AddStream(const sm2k& _registry, const string& _name);
	extern "C" SM2K_FRAMEWORK_DLL void ConfigureStream(const sm2k& _registry, const string& _name, const string& _trackFileName, const string& _type, const string& _streamLogPath = "Log/", string* _statusObserver = nullptr);
	//TODO: Add stream start function.
	//TODO: Add stream control function.
	//TODO: Add command processing function.
	//TODO: Add command processing function.

	extern "C" SM2K_FRAMEWORK_DLL void Stop(const sm2k& _registry);
	extern "C" SM2K_FRAMEWORK_DLL void FreeRegistry(sm2k& _registry);
	extern "C" SM2K_FRAMEWORK_DLL void EnableConsoleLog(const sm2k& _registry, bool _state);
};


