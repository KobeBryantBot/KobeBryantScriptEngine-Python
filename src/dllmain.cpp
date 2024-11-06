// 此文件定义DLL入口
// 请不要修改

#include <windows.h>

void onEnable();
void onDisable();

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        onEnable();
        break;
    case DLL_PROCESS_DETACH:
        onDisable();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}