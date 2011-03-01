#include "StdAfx.hpp"

void __fastcall New_Game_Loop( void )
{
	CCore::s_lpcCore->GameLoop( );
	Game_Loop( );
	return;
}