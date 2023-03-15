#include "Memory.h"

DWORD GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t FindDMAAddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), NULL);
		addr += offsets[i];
	}
	return addr;
}

kernelmode_proc_handler kernelHandler;

bool kernelmode_proc_handler::is_attached() { return handle != INVALID_HANDLE_VALUE; }

bool kernelmode_proc_handler::attach(const wchar_t* proc_name)
{
	pid = GetProcId(proc_name);

	if (!pid) {
		printf("Process not found");
		return false;
	}

	uintptr_t client = GetModuleBaseAddress(pid, L"GameAssembly.dll");
	handle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);

	if (handle == INVALID_HANDLE_VALUE) 
	{
		CloseHandle(handle);
		std::cout << "Invalid handle" << std::endl;
		return false;
	}

	return true;
}

uint64_t kernelmode_proc_handler::get_module_base(const wchar_t* module_name)
{
	if (handle == INVALID_HANDLE_VALUE) return 0;

	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, module_name))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

void kernelmode_proc_handler::read_memory(uint64_t src_addr, uint64_t dst_addr, size_t size)
{
	if (handle == INVALID_HANDLE_VALUE) return;

	ReadProcessMemory(handle, reinterpret_cast<const void*>(src_addr), reinterpret_cast<void*>(dst_addr), size, NULL);
}

void kernelmode_proc_handler::write_memory(uint64_t src_address, uint64_t dst_address, size_t size)
{
	if (handle == INVALID_HANDLE_VALUE) return;

	WriteProcessMemory(handle, reinterpret_cast<void*>(src_address), reinterpret_cast<void*>(dst_address), size, NULL);
}