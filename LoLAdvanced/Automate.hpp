#pragma once

#include <map>

class CAutomate
{
public:
	CAutomate( void );
	~CAutomate( void );

	void OnGameLoop( void );

private:
	DWORD	m_dwAttackTick;
	DWORD	m_dwLastCheck;
	std::map<DWORD, float>	m_cUnitHealth;
};