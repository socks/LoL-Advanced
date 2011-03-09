#pragma once

#include <map>

class CAutomate
{
public:
	CAutomate( void );
	~CAutomate( void );

	void OnGameLoop( void );

private:
	DWORD	m_dwLastBestTick;
	DWORD	m_dwLastAutoTick;
	DWORD	m_dwLastCheck;
	bool    m_bInUse;
	std::map<DWORD, float>	m_cUnitHealth;
};