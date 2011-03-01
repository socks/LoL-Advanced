#include "ObjectManager.hpp"
#include "Unit.hpp"
#include "NetApp.hpp"

#if defined( _DEFINE_PTRS )

#define MakeVar( x, y, z ) x y = (x) z;
#define MakeFnc( ret, call, param, name, offset ) \
	name##_t name = (##name##_t) offset;

#else

#define MakeVar( x, y, z ) extern x y;
#define MakeFnc( ret, call, param, name, offset ) \
	typedef ret##(##call##*##name##_t)##param##; \
	extern name##_t name;

#endif

// Variables
MakeVar( ObjectManager*, g_lpcUnitManager, 0x2B1BD5C )
MakeVar( Unit**, g_lpcLocalPlayer, 0x009e7f70 )

// Function Pointers
MakeFnc( void, __fastcall, ( void ), Game_Loop, 0x00750D95 )
MakeFnc( void, __thiscall, ( Unit* _this, DWORD dwActionType, float* lpfPos, Unit* lpcTarget, DWORD dwArg4, DWORD dwArg5, bool bUnk ), Unit_IssueOrder, 0x6E6120 )

#undef MakeFnc
#undef MakeVar