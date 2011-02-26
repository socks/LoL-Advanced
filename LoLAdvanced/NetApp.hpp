#pragma once

namespace NetApp
{
	class ClientFacade
	{
	public:
		virtual void Function0( void ) = 0; // +00
		virtual void Function1( void ) = 0; // +04
		virtual void Function2( void ) = 0; // +08
		virtual void Function3( void ) = 0; // +0C
		virtual void Function4( void ) = 0; // +10
		virtual void Function5( void ) = 0; // +14
		virtual void Function6( void ) = 0; // +18
		virtual void Function7( void ) = 0; // +1C
		virtual void Function8( void ) = 0; // +20
		virtual void Function9( void ) = 0; // +24
		virtual void Function10( void ) = 0; // +28
		virtual void Function11( void ) = 0; // +2C
		virtual void SendPacket( DWORD dwArg0, BYTE* lpyPacket, DWORD dwArg1, DWORD dwArg2 ) = 0; // +30

	private:
	};
};