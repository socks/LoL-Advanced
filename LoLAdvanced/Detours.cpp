#include "StdAfx.hpp"
/*
DWORD __stdcall New_Game_ProcessWorldEvent2( BYTE yId, BYTE* lpyPacket, DWORD dwLength)
{
	if( CCore::s_lpcCore->BeforePacketReceived( yId, lpyPacket, dwLength ) == false )
	{
		return 1;
	}
	return Game_ProcessWorldEvent2( yId, lpyPacket, dwLength );
}

DWORD __fastcall New_NetApp_ClientFacade_SendPacket( NetApp::ClientFacade* lpcClientFacade, DWORD _EDX, DWORD dwArg0, BYTE* lpyPacket, DWORD dwArg2, DWORD dwArg3 )
{
	return NetApp_ClientFacade_SendPacket( lpcClientFacade, dwArg0, lpyPacket, dwArg2, dwArg3 );
}*/

void __fastcall New_Game_Loop( void )
{
	CCore::s_lpcCore->GameLoop( );
	Game_Loop( );
	return;
}