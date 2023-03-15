#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

uintptr_t FindDMAAddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);

namespace Memory {
	template <typename T>
	constexpr T Read(HANDLE hProc, const uintptr_t& address)
	{
		T value = {};
		::ReadProcessMemory(hProc, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}

	template <typename T>
	constexpr void Write(HANDLE hProc, const uintptr_t& address, const T& value)
	{
		::WriteProcessMemory(hProc, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
}

class kernelmode_proc_handler
{
protected:
	HANDLE handle;
	uint32_t pid;
	//uint32_t this_process_pid;
public:
	//kernelmode_proc_handler();
	//~kernelmode_proc_handler();

	bool is_attached();

	bool attach(const wchar_t* proc_name);

	uint64_t get_module_base(const wchar_t* module_name);

	void read_memory(uint64_t src_addr, uint64_t dst_addr, size_t size);

	void write_memory(uint64_t dst_addr, uint64_t src_addr, size_t size);


	template <typename type>
	type readmem(uint64_t src)
	{
		type ret = {};
		read_memory(src, (uint64_t)&ret, sizeof(type));
		return ret;
	}

	template <typename type>
	void writemem(uint64_t dst_addr, type var)
	{
		type inp = var;
		write_memory(dst_addr, (uint64_t)&inp, sizeof(type));
	}
};

extern kernelmode_proc_handler kernelHandler;

#define read(src_addr, type) kernelHandler.readmem<type>(src_addr)
#define write(dst_addr, var, type) kernelHandler.writemem<type>(dst_addr, var)