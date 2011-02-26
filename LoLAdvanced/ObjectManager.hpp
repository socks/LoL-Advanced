#pragma once

class Unit;

class ObjectManager
{
public:
	inline Unit** GetFirst( void ) { return m_lpcUnits; }
	inline DWORD GetMaxObjects( void ) { return m_dwMaxObjects; }
	inline DWORD GetObjects( void ) { return m_dwObjects; }
	inline Unit** GetEnd( void ) { return m_lpcUnits + m_dwEnd; }

private:
	/* 0x00 */ Unit** m_lpcUnits;
	/* 0x04 */ DWORD m_dwMaxObjects;
	/* 0x08 */ DWORD m_dwObjects;
	/* 0x0C */ DWORD m_dwEnd;
};