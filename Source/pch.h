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


#include <filesystem>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <unordered_map>
// TODO: Add FFMPEG libs
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
#include "headers/Stream.h"
#include "headers/CoreTypes.h"
#include "headers/Core.h"
#include "headers/HashTable.h"
#include "headers/CmdTools.inl"
#include "headers/Interpreter.h"
#include "headers/SM2K_CMDS.inl"
#include "headers/Exception.h"
#include "headers/Signal.h"