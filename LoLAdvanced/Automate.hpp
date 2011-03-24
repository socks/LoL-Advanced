#pragma once

#include <map>
#include <list>

class CAutomate
{
public:
	CAutomate( void );
	~CAutomate( void );

	void OnGameLoop( void );

private:
	DWORD	m_dwLastBestTick;
	DWORD	m_dwLastCheck;
	bool    m_bInUse;
	std::map<DWORD, std::list<float>>	m_cUnitHealth;
};