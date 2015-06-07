#ifndef __PHYSX_HEADER__
#define __PHYSX_HEADER__

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <PxPhysicsAPI.h>

#ifdef _DEBUG

#ifdef _WIN64
#pragma comment( lib, "PhysX3DEBUG_x64.lib" )
#pragma comment( lib, "PhysX3CommonDEBUG_x64.lib" )
#else
#pragma comment( lib, "PhysX3DEBUG_x86.lib" )
#pragma comment( lib, "PhysX3CommonDEBUG_x86.lib" )
#endif

#pragma comment( lib, "PhysX3ExtensionsDEBUG.lib" )
#pragma comment( lib, "PhysXVisualDebuggerSDKDEBUG.lib" )

#else

#ifdef _WIN64
#pragma comment( lib, "PhysX3_x64.lib" )
#pragma comment( lib, "PhysX3Common_x64.lib" )
#else
#pragma comment( lib, "PhysX3_x86.lib" )
#pragma comment( lib, "PhysX3Common_x86.lib" )
#endif

#pragma comment( lib, "PhysX3Extensions.lib" )

#endif


#endif