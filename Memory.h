#include "Includes.h"

class RPM {
private:
	DWORD Proc_ID; 
	LPCSTR _WindowName;
public:
	HANDLE hProcess; 
	DWORD *base;
	void attach(LPCSTR WindowName) {
		HWND hWindow = FindWindowA(NULL, WindowName); 
		if (hWindow)
		{
			GetWindowThreadProcessId(hWindow, &Proc_ID); 
			hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION/*PROCESS_ALL_ACCESS*/, FALSE, Proc_ID); 
			HANDLE hModule = INVALID_HANDLE_VALUE; 
			MODULEENTRY32 ePoint; 
			hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Proc_ID); 
			ePoint.dwSize = sizeof(MODULEENTRY32); 
			Module32First(hModule, &ePoint);
			base = (DWORD*)ePoint.modBaseAddr; 
			CloseHandle(hModule);
		}
	}

	template <class cData>
	cData read(DWORD64(Address)) {
		try {
			if (Proc_ID > 0) {
				cData B;
				ReadProcessMemory(hProcess, (LPCVOID)Address, &B, sizeof(B), NULL);
				return B;
			}
			else {
				throw 1;
			}
		}
		catch (int error) {
			std::cout << "ERROR:\t" << error << std::endl;
		}
	}
	template <class cData>

	cData write(DWORD64(Address), cData B) {
		try {
			if (Proc_ID > 0) {
				VirtualProtectEx(hProcess, (LPVOID)(Address), sizeof(B), PAGE_EXECUTE_READWRITE, NULL);
				WriteProcessMemory(hProcess, (LPVOID)(Address), &B, sizeof(B), NULL);
				return B;
			}
			else {
				throw 1; 
			}
		} 
		catch (int error) {
			std::cout << "ERROR:\t" << error << std::endl;
		}
//		return 0;
	} 
};

