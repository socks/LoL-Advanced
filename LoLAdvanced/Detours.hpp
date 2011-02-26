#pragma once

extern DWORD __stdcall New_Game_ProcessWorldEvent2( BYTE yId, BYTE* lpyPacket, DWORD dwLength);
extern DWORD __fastcall New_NetApp_ClientFacade_SendPacket( NetApp::ClientFacade* lpcClientFacade, DWORD _EDX, DWORD dwArg0, BYTE* lpyPacket, DWORD dwArg2, DWORD dwArg3 );
extern void __fastcall New_Game_Loop( void );