#include "StdAfx.hpp"
#define _DEFINE_PTRS
#include "LoLPtrs.hpp"

CMemory::CMemory( void )
{
	m_lpsFirstPatch = NULL;
	m_lpsLastPatch = NULL;
}

CMemory::~CMemory( void )
{
	DWORD dwOldProtect;

	for( SPatch* lpsPatch = m_lpsFirstPatch; lpsPatch != NULL; lpsPatch = lpsPatch->lpsNext )
	{
		if( lpsPatch->lpsPrev != NULL )
		{
			delete lpsPatch->lpsPrev;
		}

		VirtualProtect( (void*) lpsPatch->dwAddress, lpsPatch->dwLength, PAGE_EXECUTE_READWRITE, &dwOldProtect );
		memcpy( (void*) lpsPatch->dwAddress, lpsPatch->yBackup, lpsPatch->dwLength );
		VirtualProtect( (void*) lpsPatch->dwAddress, lpsPatch->dwLength, dwOldProtect, &dwOldProtect );

		if( lpsPatch->eType == Inline )
		{
			VirtualFree( (void*) lpsPatch->dwTrampolin, 0, MEM_RELEASE );
		}
	}

	delete m_lpsLastPatch;

	m_lpsFirstPatch = NULL;
	m_lpsLastPatch = NULL;
}

DWORD CMemory::_Patch( EPatchType eType, DWORD dwAddress, DWORD dwDetour, DWORD dwLength )
{
	DWORD dwProtect = 0;
	SPatch* lpsPatch = new SPatch;
	
	ZeroMemory( lpsPatch, sizeof( SPatch ) );

	lpsPatch->eType = eType;
	lpsPatch->dwAddress = dwAddress;
	lpsPatch->dwLength = dwLength;

	switch( eType )
	{
		case Call: // Oldschool Call/Jump Detours..
		case Jump:
			{
				BYTE yCave[ 64 ];

				// Compute Length if no Length paramter was given.
				if( dwLength == 0 )
				{
					for( ; dwLength < 5; dwLength += ComputeLength( dwAddress + dwLength ) );
					lpsPatch->dwLength = dwLength;
				}

				// Prepare the new assembler code.
				memset( yCave, 0x90, dwLength );

				yCave[ 0 ] = eType == Call ? OpCall : OpJmp;
				*(DWORD*)&yCave[ 1 ] = dwDetour - dwAddress - 5;

				// Calculate Trampoline Jump/Call if the Opcode is either 0xE8 or 0xE9.
				if( *(BYTE*) dwAddress == OpCall || *(BYTE*) dwAddress == OpJmp )
				{
					lpsPatch->dwTrampolin = *(DWORD*)( dwAddress + 1 ) + dwAddress + 5;
				}

				// Change Page Protection, backup old code and write the detour.
				VirtualProtect( (void*) dwAddress, dwLength, PAGE_EXECUTE_READWRITE, &dwProtect );
				
				memcpy( lpsPatch->yBackup, (void*) dwAddress, dwLength );
				memcpy( (void*)dwAddress, yCave, dwLength );

				VirtualProtect( (void*) dwAddress, dwLength, dwProtect, &dwProtect );
			}
			break;
		case VMT: 
				// A bit newer, VMT hooks! Easy to make, easy to manage!
			{	// Length Parameter is in this case the index of the method in the virtual method table.

				dwAddress = *(DWORD*) dwAddress;
				dwAddress += dwLength * sizeof( DWORD );

				lpsPatch->dwAddress = dwAddress;
				lpsPatch->dwTrampolin = *(DWORD*) dwAddress;
				lpsPatch->dwLength = sizeof( DWORD );

				// Backup old address, and patch it.
				VirtualProtect( (void*) dwAddress, sizeof( DWORD ), PAGE_EXECUTE_READWRITE, &dwProtect );
				memcpy( lpsPatch->yBackup, (void*) dwAddress, sizeof( DWORD ) );
				*(DWORD*)&dwAddress = dwDetour;
				VirtualProtect( (void*) dwAddress, sizeof( DWORD ), dwProtect, &dwProtect );
			}
			break;
		case Inline:
				// Very comfortable, Inline Hooks!
			{
				BYTE yCave[ 64 ];

				// Compute the length
				if( dwLength == 0 )
				{
					for( ; dwLength < 5; dwLength += ComputeLength( dwAddress + dwLength ) );
					lpsPatch->dwLength = dwLength;
				}
				
				// Prepare the new assembler code.
				memset( yCave, OpNop, dwLength );
				yCave[ 0 ] = OpJmp;
				*(DWORD*)&yCave[ 1 ] = dwDetour - dwAddress - 5;

				// Backup the old code
				VirtualProtect( (void*) dwAddress, dwLength, PAGE_EXECUTE_READWRITE, &dwProtect );
				memcpy( lpsPatch->yBackup, (void*) dwAddress, dwLength );

				// Allocate some memory on which the back up'd code gets copied onto.
				lpsPatch->dwTrampolin = (DWORD) VirtualAlloc( NULL, dwLength, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
				memcpy( (void*) lpsPatch->dwTrampolin, (void*) lpsPatch->yBackup, dwLength );

				// Write the jmp opcode to the rest of the original code
				*(BYTE*)(lpsPatch->dwTrampolin + dwLength ) = OpJmp;
				*(DWORD*)(lpsPatch->dwTrampolin + dwLength + 1 ) = ( dwAddress + dwLength ) - ( lpsPatch->dwTrampolin + dwLength ) - 5;

				// now patch it				
				memcpy( (void*)dwAddress, yCave, dwLength );
				
				VirtualProtect( (void*) dwAddress, dwLength, dwProtect, &dwProtect );
			}
			break;
		default:
			delete lpsPatch;
			return NULL;
	}

	if( m_lpsFirstPatch == NULL )
	{
		m_lpsFirstPatch = lpsPatch;
	}

	if( m_lpsLastPatch != NULL )
	{
		m_lpsLastPatch->lpsNext = lpsPatch;
	}

	lpsPatch->lpsPrev = m_lpsLastPatch;
	m_lpsLastPatch = lpsPatch;

	return lpsPatch->dwTrampolin;
}

DWORD
CMemory::ComputeLength( DWORD dwAddress )
{
	return _mlde32( dwAddress );
}

__declspec(naked) DWORD
CMemory::_mlde32( DWORD dwAddress )
{
	enum
	{
		O_UNIQUE      = 0,
		O_PREFIX      = 1,
		O_IMM8        = 2,
		O_IMM16       = 3,
		O_IMM24       = 4,
		O_IMM32       = 5,
		O_IMM48       = 6,
		O_MODRM       = 7,
		O_MODRM8      = 8,
		O_MODRM32     = 9,
		O_EXTENDED    = 10,
		O_WEIRD       = 11,
		O_ERROR       = 12,
	};
	__asm
	{
		   pushad
		   cld
		   xor  edx, edx

		   mov  esi, [esp+(8*4)+4]
		   mov  ebp, esp

		   // 256 bytes, index-compressed opcode type table
		   push 01097F71Ch
		   push 0F71C6780h
		   push 017389718h
		   push 0101CB718h
		   push 017302C17h
		   push 018173017h
		   push 0F715F547h
		   push 04C103748h
		   push 0272CE7F7h
		   push 0F7AC6087h
		   push 01C121C52h
		   push 07C10871Ch
		   push 0201C701Ch
		   push 04767602Bh
		   push 020211011h
		   push 040121625h
		   push 082872022h
		   push 047201220h
		   push 013101419h
		   push 018271013h
		   push 028858260h
		   push 015124045h       
		   push 05016A0C7h
		   push 028191812h
		   push 0F2401812h
		   push 019154127h
		   push 050F0F011h
		   mov  ecx, 015124710h
		   push ecx
		   push 011151247h
		   push 010111512h
		   push 047101115h
		   mov  eax, 012472015h
		   push eax
		   push eax
		   push 012471A10h
		   add  cl, 10h
		   push ecx
		   sub  cl, 20h
		   push ecx

		   xor  ecx, ecx
		   dec  ecx

		   ; code starts
	_ps:  inc  ecx
		   mov  edi, esp
	_go:  lodsb
		   mov  bh, al
	_ft:  mov  ah, [edi]
		   inc  edi
		   shr  ah, 4
		   sub  al, ah
		   jnc  _ft

		   mov  al, [edi-1]
		   and  al, 0Fh

		   cmp  al, O_ERROR
		   jnz  _i7
       
		   pop  edx
		   not  edx

	_i7:  inc  edx
		   cmp  al, O_UNIQUE
		   jz   _t_exit

		   cmp  al, O_PREFIX
		   jz   _ps

		   add  edi, 51h          //(__ettbl - __ttbl)

		   cmp  al, O_EXTENDED
		   jz   _go

		   mov  edi, [ebp+(8*4)+4]

	       inc  edx
		   cmp  al, O_IMM8
		   jz   _t_exit
		   cmp  al, O_MODRM
		   jz   _t_modrm
		   cmp  al, O_WEIRD
		   jz   _t_weird

	_i5:  inc  edx
		   cmp  al, O_IMM16
		   jz   _t_exit
		   cmp  al, O_MODRM8
		   jz   _t_modrm

	      inc  edx
		   cmp  al, O_IMM24
		   jz   _t_exit

		  inc  edx
	      inc  edx

		   pushad
		   mov  al, 66h
		   repnz scasb
		   popad
		   jnz  _c32

	_d2:  dec  edx
		   dec  edx

	_c32: cmp  al, O_MODRM32
		   jz   _t_modrm
		   sub  al, O_IMM32
		   jz   _t_imm32

	_i1:  inc  edx

	_t_exit:
		   mov  esp, ebp
		   mov  [esp+(7*4)], edx
		   popad
		   ret

	/*********************************
	;* PROCESS THE MOD/RM BYTE       *
	;*                               *
	;*   7    6 5          3 2    0  *
	;*   | MOD | Reg/Opcode | R/M |  *
	;*                               *
	;*********************************/
	_t_modrm:
		   lodsb
		   mov  ah, al
		   shr  al, 7
		   jb   _prmk
		   jz   _prm

		   add  dl, 4

		   pushad
		   mov  al, 67h
		   repnz scasb
		   popad
		   jnz  _prm

		   sub  dl, 3

		   dec  al
	_prmk:jnz  _t_exit
		   inc  edx
		   inc  eax
	_prm:
		   and  ah, 00000111b

		   pushad
		   mov  al, 67h
		   repnz scasb
		   popad
		   jz   _prm67chk

		   cmp  ah, 04h
		   jz   _prmsib

		   cmp  ah, 05h
		   jnz  _t_exit

		   dec  al
		   jz   _t_exit
	_i42: add  dl, 4
		   jmp  _t_exit

	_prm67chk:
		   cmp  ax, 0600h
		   jnz  _t_exit
		   inc  edx
		   jmp  _i1

	_prmsib:
		   cmp  al, 00h
		   jnz  _i1
		   lodsb
		   and  al, 00000111b
		   sub  al, 05h
		   jnz  _i1
		   inc  edx
		   jmp  _i42

	/****************************
	;* PROCESS WEIRD OPCODES    *
	;*                          *
	;* Fucking test (F6h/F7h)   *
	;*                          *
	;***************************/
	_t_weird:
		   test byte ptr [esi], 00111000b
		   jnz  _t_modrm

		   mov  al, O_MODRM8

		   shr  bh, 1
		   adc  al, 0
		   jmp  _i5

	/*********************************
	;* PROCESS SOME OTHER SHIT       *
	;*                               *
	;* Fucking mov (A0h/A1h/A2h/A3h) *
	;*                               *
	;********************************/
	_t_imm32:
		   sub  bh, 0A0h

		   cmp  bh, 04h
		   jae  _d2

		   pushad
		   mov  al, 67h
		   repnz scasb
		   popad
		   jnz  _chk66t

	      dec  edx
		   dec  edx

	_chk66t:
		   pushad
		   mov  al, 66h
		   repnz scasb
		   popad
		   jz   _i1
		   jnz  _d2
	}
}