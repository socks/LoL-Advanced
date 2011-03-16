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
MakeVar( ObjectManager*, g_lpcUnitManager, 0x2B22EFC )
MakeVar( Unit**, g_lpcLocalPlayer, 0x009EF10C )
MakeVar( DWORD, g_dwPrintArgument, 0x2A9C068 )

// Function Pointers
MakeFnc( void, __fastcall, ( void ), Game_Loop, 0x00754DFE )
MakeFnc( void, __thiscall, ( Unit* _this, DWORD dwActionType, float* lpfPos, Unit* lpcTarget, DWORD dwArg4, DWORD dwArg5, bool bUnk ), Unit_IssueOrder, 0x006EA2D0 )
MakeFnc( Unit*, __cdecl, ( DWORD dwNetworkId ), Unit_GetUnitByNetworkId, 0x00815100 )
MakeFnc( int, __fastcall, ( char* szText, DWORD dwArgument, bool bUnk1, bool bUnk2 ), Print_Console, 0x79F590 )

#undef MakeFnc
#undef MakeVar
#undef MakeAsm
