#pragma once

class CMemory
{
public:
	CMemory( void );
	~CMemory( void );

	enum EPatchType
	{
		Jump,
		Call,
		VMT,
		Inline,
	};

	template<typename T> T Patch( EPatchType eType, DWORD dwAddress, DWORD dwDetour, DWORD dwLength = 0 )
	{
		return (T) _Patch( eType, dwAddress, dwDetour, dwLength );
	}
	DWORD ComputeLength( DWORD dwAddress );

private:
	static DWORD __cdecl _mlde32( DWORD dwAddress );
	DWORD _Patch( EPatchType eType, DWORD dwAddress, DWORD dwDetour, DWORD dwLength );

	struct SPatch
	{
		SPatch* lpsPrev;
		SPatch* lpsNext;
		EPatchType eType;
		DWORD dwAddress;
		DWORD dwLength;
		BYTE yBackup[ 64 ];
		DWORD dwTrampolin;
	};

	enum EOpCodes : BYTE
	{
		OpNop = 0x90,
		OpJmp = 0xE9,
		OpCall = 0xE8,
	};

	SPatch* m_lpsFirstPatch;
	SPatch* m_lpsLastPatch;
};