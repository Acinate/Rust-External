//#include <iostream>
//#include <windows.h>
//#include "Memory.h"
//// #include "Loops.h"
//
//
//static bool localPlayerIsExist = false;
//int BaseNetworkable_loop(DWORD64 BaseNetworkable)
//{
//
//    //TODO: Fix this
//    DWORD procId = GetProcId(L"RustClient.exe");
//
//    if (!procId) {
//        printf("Process not found");
//    }
//
//    uintptr_t client = GetModuleBaseAddress(procId, L"GameAssembly.dll");
//    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);
//    uintptr_t oBaseNetworkable = 0x36484E8;
//
//    uintptr_t BaseNetworkable = Memory::Read<uintptr_t>(handle, (client + oBaseNetworkable));
//    uintptr_t EntityRealm = Memory::Read<uintptr_t>(handle, (BaseNetworkable + 0xB8));
//    uintptr_t ClientEntities = Memory::Read<uintptr_t>(handle, (EntityRealm));
//    uintptr_t ClientEntities_list = Memory::Read<uintptr_t>(handle, (ClientEntities + 0x10));
//    uintptr_t ClientEntities_values = Memory::Read<uintptr_t>(handle, (ClientEntities_list + 0x28));
//
//    if (!ClientEntities_values) return 0;
//    int EntityCount = Memory::Read<uintptr_t>(handle, ClientEntities_values + 0x10);
//
//    DWORD64 EntityBuffer = Memory::Read<uintptr_t>(handle, ClientEntities_values + 0x18);
//
//    localPlayerIsExist = false;
//    for (int i = 0; i < EntityCount; i++) {
//        uintptr_t Entity = Memory::Read<uintptr_t>(handle, EntityBuffer + 0x20 + (i * 0x8));
//        if (Entity <= 100000) continue;
//
//        uintptr_t Object = Memory::Read<uintptr_t>(handle, Entity + 0x10);
//        if (Object <= 100000) continue;
//
//        uintptr_t ObjectClass = Memory::Read<uintptr_t>(handle, Object + 0x30);
//        if (ObjectClass <= 100000) continue;
//
//        if (Memory::Read<int16_t>(handle, ObjectClass + 0x54) == 6) {
//            std::cout << "Object:" << Object << std::endl;
//        }
//
//        uintptr_t addr_name = Memory::Read<uintptr_t>(handle, ObjectClass + 0x60);
//
//        static char BNname[110] = { '\0' };
//        ReadProcessMemory(handle, reinterpret_cast<const void*>(addr_name), &BNname, sizeof(BNname), NULL);
//        BNname[109] = { '\0' };
//
//        //objInf currentObj;
//        if (strstr(BNname, "LocalPlayer"))
//        {
//            localPlayerIsExist = true;
//            uintptr_t myLocalPlayer = Memory::Read<uintptr_t>(handle, Object + 0x28);
//            std::cout << "LocalPlayer:" << std::hex << myLocalPlayer << std::endl;
//        }
//        else if (strstr(BNname, "player.prefab") && !strstr(BNname, "prop") && !strstr(BNname, "corpse"))
//        {
//            std::cout << "Player prefab:" << std::hex << Object << std::endl;
//            uintptr_t player = Memory::Read<uintptr_t>(handle, Object + 0x28);
//
//
//        }
//        else if (strstr(BNname, "player_corpse.prefab"))
//        {
//            std::cout << "Player Corpse prefab:" << std::hex << Object << std::endl;
//        }
//        else if (strstr(BNname, "item_drop_backpack.prefab"))
//        {
//            std::cout << "Backpack prefab:" << std::hex << Object << std::endl;
//        }
//
//        // TODO: Get Player x,y,z coords
//        // TODO: Get ViewMatrix
//        // TODO: Get Player name
//        // TODO: Get Player health
//        // TODO: Get Player distance
//        // TODO: Get Player skeleton
//
//    }
//
//    std::cout << "Hello World!\n";
//    system("pause");
//
//    return 0;
//}
