#pragma once

class Unit
{
public:
	inline bool IsDead( void ) { return m_yDead == 1; }
	inline DWORD GetTeam( void ) { return m_dwTeam; }
	inline char* GetName( void ) { return m_lpszName; }
	inline float* GetPos( void ) { return m_fPos; }
	inline float GetMissChance( void ) { return m_fMissChance; }
	inline DWORD GetNetworkId( void ) { return m_dwNetworkId; }
	inline float GetHealth( void ) { return m_fHealth; }
	inline float GetMana( void ) { return m_fMana; }
	inline float GetRange( void ) { return m_fRange; }
	inline float GetBaseDamage( void ) { return m_fBaseDamage; }
	inline float GetAdditionDamage( void ) { return m_fAddedDamage; }
	inline float GetTotalDamage( void ) { return m_fBaseDamage + m_fAddedDamage; }

	static float CalcDistance( Unit* lpcUnitA, Unit* lpcUnitB );
	static Unit* GetUnitByNetworkId( DWORD dwNetworkId );
	static Unit* GetPlayerToon( void );

private:
	/* 0x0000 */ BYTE _00[ 0x12 ];
	/* 0x0012 */ BYTE m_yDead;
	/* 0x0013 */ BYTE _13[ 0x09 ];
	/* 0x001C */ DWORD m_dwTeam;
	/* 0x0020 */ BYTE _20[ 0x08 ];
	/* 0x0028 */ char* m_lpszName;
	/* 0x002C */ BYTE _2C[ 0x30 ];
	/* 0x005C */ float m_fPos[ 3 ];
	/* 0x0068 */ BYTE _68[ 0x88 ];
	/* 0x00F0 */ float m_fMissChance;
	/* 0x00F4 */ BYTE _F4[ 0x04 ];
	/* 0x00F8 */ DWORD m_dwNetworkId;
	/* 0x00FC */ BYTE _FC[ 0x20 ];
	/* 0x011C */ float m_fHealth;
	/* 0x0120 */ BYTE _120[ 0x44 ];
	/* 0x0164 */ float m_fMana;
	/* 0x0168 */ BYTE _168[ 0x8D8 ];
	/* 0x0A40 */ float m_fAddedDamage;
	/* 0x0A44 */ BYTE _A44[ 0x60 ];
	/* 0x0AA4 */ float m_fBaseDamage;
	/* 0x0AA8 */ BYTE _AA8[ 0x40 ];
	/* 0x0AE8 */ float m_fRange;
};