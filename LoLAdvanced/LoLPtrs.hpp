#include "ObjectManager.hpp"
#include "Unit.hpp"
#include "NetApp.hpp"

#if defined( _DEFINE_PTRS )

#define MakeVar( x, y, z ) x y = (x) z;
#define MakeFnc( ret, call, param, name, offset ) \
	name##_t name = (##name##_t) offset;
#define MakeAsm( x, y ) DWORD x = (DWORD) y;

#else

#define MakeVar( x, y, z ) extern x y;
#define MakeFnc( ret, call, param, name, offset ) \
	typedef ret##(##call##*##name##_t)##param##; \
	extern name##_t name;
#define MakeAsm( x, y ) extern DWORD x;

#endif

// Variables
MakeVar( ObjectManager*, g_lpcUnitManager, 0x02B1FE1C )
MakeVar( Unit**, g_lpcLocalPlayer, 0x009ec02c )

// Function Pointers
MakeFnc( void, __fastcall, ( void ), Game_Loop, 0x007520D5 )
MakeFnc( void, __thiscall, ( Unit* _this, DWORD dwActionType, float* lpfPos, Unit* lpcTarget, DWORD dwArg4, DWORD dwArg5, bool bUnk ), Unit_IssueOrder, 0x006E7450 )
MakeFnc( Unit*, __cdecl, ( DWORD dwNetworkId ), Unit_GetUnitByNetworkId, 0x00812DD0 )

#undef MakeFnc
#undef MakeVar
#undef MakeAsm
