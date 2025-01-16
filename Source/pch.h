#pragma once
// We only need gateware for logging.
#define GATEWARE_ENABLE_CORE
#define GATEWARE_ENABLE_SYSTEM
// disable all graphics libs since we don't need em.
#define GATEWARE_DISABLE_GOPENGLSURFACE
#define GATEWARE_DISABLE_GVULKANSURFACE
#define GATEWARE_DISABLE_GRASTERSURFACE
#define GATEWARE_DISABLE_GDIRECTX11SURFACE
#define GATEWARE_DISABLE_GDIRECTX12SURFACE


#ifdef _WIN32
#define _popen_ _popen
#define _pclose_ _pclose
#define SM2K_APPDATA_CMD "echo %APPDATA%"
#else
#define _popen_ popen
#define _pclose_ pclose
#define SM2K_APPDATA_CMD "echo $XDG_CONFIG_HOME"
#endif // _WIN32
#define X(x) #x
#define _S(a) X(#a)
#define __S(a) X(a)
#define S(a) _S(a)
#define __SM2K_END_OF_CONFIG [BUFFER_START]
#define _SM2K_END_OF_CONFIG __S(__SM2K_END_OF_CONFIG)
#define SM2K_END_OF_CONFIG  S(__SM2K_END_OF_CONFIG ## \n\n)


#ifndef PATH_TO_CORE_PYTHON_LIBS
#define PATH_TO_CORE_PYTHON_LIBS "C:\\Users\\Towel\\AppData\\Local\\Programs\\Python\\Python312\\libs\\"
#endif // !PATH_TO_CORE_PYTHON_LIBS


#pragma comment(lib, PATH_TO_CORE_PYTHON_LIBS"python3.lib") 
#pragma comment(lib, PATH_TO_CORE_PYTHON_LIBS"python312.lib") 


#include <filesystem>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <stdio.h>
#include <pybind11/embed.h>

extern "C" {
// Added FFMPEG libs
#include <libavformat/avformat.h>
#include <libavutil/time.h>
}

#include "headers/externals/entt/entt.hpp"
#include "headers/externals/ini/inicpp.h"
#include "headers/externals/Gateware/Gateware.h"
#include "headers/SM2K.h"
#include "headers/Config.h"
#include "headers/BinTree.h"
#include "headers/ECS.h"
#include "headers/Util.h"
#include "headers/StreamMonkey2K_fw.h"
#include "headers/Handler.inl"
#include "headers/BaseHandler.inl"
#include "headers/BaseProcess.inl"
#include "headers/CoreTypes.h"
#include "headers/Core.h"
#include "headers/Stream.h"
#include "headers/HashTable.h"
#include "headers/CmdTools.inl"
#include "headers/Interpreter.h"
#include "headers/SM2K_CMDS.inl"
#include "headers/Exception.h"
#include "headers/Signal.h"
#include "headers/BitStream.h"