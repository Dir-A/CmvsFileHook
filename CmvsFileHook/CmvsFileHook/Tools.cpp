#include "Tools.h"

static FILE* streamconsole;
DWORD WINAPI SetConsoleTop(LPVOID lpParameter)
{
	WCHAR consoleTitle[256] = { 0 };

	while (true)
	{
		GetConsoleTitleW(consoleTitle, 256);
		HWND hConsole = FindWindowW(NULL, (LPWSTR)consoleTitle);
		if (hConsole != NULL)
		{
			SetWindowPos(hConsole, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			break;
		}
	}

	return 0;
}

VOID WINAPI SetConsole()
{
	DWORD mode = 0;
	HANDLE hStdin = 0;

	AllocConsole();
	AttachConsole(ATTACH_PARENT_PROCESS);
	freopen_s(&streamconsole, "CONIN$", "r+t", stdin);
	freopen_s(&streamconsole, "CONOUT$", "w+t", stdout);
	SetConsoleTitleW(L"CmvsFileHook");

	CreateThread(NULL, NULL, SetConsoleTop, NULL, NULL, NULL);

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hStdin, &mode);
	SetConsoleMode(hStdin, mode & ~ENABLE_QUICK_EDIT_MODE);

	//system("chcp 65001");
	//setlocale(LC_ALL, "chs");

	std::locale::global(std::locale(""));
}

BOOL SetHook(DWORD dwRawAddr, DWORD dwTarAddr, SIZE_T szRawSize)
{
	BOOL isProtect = 0;
	DWORD oldProtect = 0;
	PBYTE allocateAddr = 0;
	DWORD relativeAddr = 0;
	BYTE rawJmp[] = { 0xE9,0x00,0x00,0x00,0x00 };
	BYTE retJmp[] = { 0xE9,0x00,0x00,0x00,0x00 };
	BYTE tarCal[] = { 0xE8,0x00,0x00,0x00,0x00 };

	isProtect = VirtualProtect((LPVOID)dwRawAddr, 0x1000, PAGE_EXECUTE_READWRITE, &oldProtect);
	allocateAddr = (PBYTE)VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (allocateAddr && isProtect)
	{
		//Copy the Code for the original address to alloc address
		memcpy(allocateAddr, (PVOID)dwRawAddr, szRawSize);

		//Write Jmp Code
		relativeAddr = (DWORD)allocateAddr - dwRawAddr - sizeof(rawJmp);
		memcpy(&rawJmp[1], &relativeAddr, sizeof(DWORD));
		memcpy((PBYTE)dwRawAddr, rawJmp, sizeof(rawJmp));

		//Write Call TarFunc Code
		relativeAddr = dwTarAddr - (DWORD)(&allocateAddr[szRawSize]) - sizeof(tarCal);
		memcpy(&tarCal[1], &relativeAddr, sizeof(DWORD));
		memcpy(&allocateAddr[szRawSize], tarCal, sizeof(tarCal));

		//Write Ret Code
		relativeAddr = (dwRawAddr + szRawSize) - (DWORD)(&allocateAddr[szRawSize + sizeof(tarCal)]) - sizeof(retJmp);
		memcpy(&retJmp[1], &relativeAddr, sizeof(DWORD));
		memcpy(&allocateAddr[szRawSize + sizeof(tarCal)], retJmp, sizeof(retJmp));

		return TRUE;
	}
	else
	{
		MessageBoxW(NULL, L"SetHook Failed!!", NULL, NULL);
		return FALSE;
	}

}
