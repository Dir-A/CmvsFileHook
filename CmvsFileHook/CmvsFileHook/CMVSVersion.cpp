#include "CMVSVersion.h"

BOOL g_isDebug_FilePath = FALSE;
PCHAR g_pFileName = 0;
CHAR g_sReadFolder[] = ".\\FileHook\\";

BOOL FileExist()
{
	if (g_isDebug_FilePath)
	{
		std::cout << "Read:" << g_pFileName << std::endl;
	}

	DWORD isFileExist = 0;;
	CHAR repPath[MAX_PATH] = { 0 };

	lstrcatA(repPath, g_sReadFolder);
	lstrcatA(repPath, g_pFileName);

	isFileExist = GetFileAttributesA(repPath);

	if ((isFileExist != INVALID_FILE_ATTRIBUTES) && (isFileExist != FILE_ATTRIBUTE_DIRECTORY))
	{
		if (g_isDebug_FilePath)
		{
			std::cout << "Hook:" << g_pFileName << std::endl;
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID __declspec(naked)CMVS_210_PS2Hook()
{
	__asm
	{
		pushfd
		pushad
		mov g_pFileName, edi
		test ebx, ebx
		je isEbxNULL
		mov g_pFileName, ebx
		isEbxNULL :
	}

	if (FileExist())
	{
		__asm
		{
			mov dword ptr[esp + 0x18], offset g_sReadFolder
		}
		*(BYTE*)SCRIPTXOR = 0xEB;
		*(BYTE*)SCRIPTCOM = 0xEB;
	}
	else
	{
		*(BYTE*)SCRIPTXOR = 0x76;
		*(BYTE*)SCRIPTCOM = 0x74;
	}

	__asm
	{
		popad
		popfd
		ret
	}
}

VOID __declspec(naked)CMVS_210_PB3Hook()
{
	__asm
	{
		pushfd
		pushad
		mov g_pFileName, ebp
	}

	if (FileExist())
	{
		__asm mov dword ptr[esp], offset g_sReadFolder
	}

	__asm
	{
		popad
		popfd
		ret
	}
}

VOID __declspec(naked)CMVS_342_PS3Hook()
{
	__asm
	{
		pushfd
		pushad
		mov eax, dword ptr[esp + 0x10]
		mov g_pFileName, eax
	}

	if (FileExist())
	{
		__asm
		{
			mov dword ptr[esp + 0x18], offset g_sReadFolder
		}
		*(BYTE*)SCRIPTXOR = 0xEB;
		*(BYTE*)SCRIPTCOM = 0xEB;
	}
	else
	{
		*(BYTE*)SCRIPTXOR = 0x74;
		*(BYTE*)SCRIPTCOM = 0x74;
	}

	__asm
	{
		popad
		popfd
		ret
	}
}

VOID __declspec(naked)CMVS_342_PB3Hook()
{
	__asm
	{
		pushfd
		pushad
		mov g_pFileName, ebx
	}

	if (FileExist())
	{
		__asm mov dword ptr[esp], offset g_sReadFolder
	}

	__asm
	{
		popad
		popfd
		ret
	}
}

VOID __declspec(naked)CMVS_380_PS3Hook()
{
	__asm
	{
		pushfd
		pushad
		mov eax, dword ptr[esp + 0x10]
		mov g_pFileName, eax
	}

	if (FileExist())
	{
		__asm
		{
			mov dword ptr[esp + 0x18], offset g_sReadFolder
		}
		*(BYTE*)SCRIPTXOR = 0xEB;
		*(BYTE*)SCRIPTCOM = 0xEB;
	}
	else
	{
		*(BYTE*)SCRIPTXOR = 0x74;
		*(BYTE*)SCRIPTCOM = 0x74;
	}

	__asm
	{
		popad
		popfd
		ret
	}
}

VOID __declspec(naked)CMVS_380_PB3Hook()
{
	__asm
	{
		pushfd
		pushad
		mov eax, dword ptr[esp + 0x2C]
		mov g_pFileName, eax
	}

	if (FileExist())
	{
		__asm mov dword ptr[esp + 0x4], offset g_sReadFolder
	}

	__asm
	{
		popad
		popfd
		ret
	}
}