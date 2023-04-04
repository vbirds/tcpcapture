#include "dynamiclib_mgr.h"
#include <stdio.h>


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <string>

std::string GetLastErrorMessage() {
    DWORD error_code = GetLastError();
    if (error_code == 0) {
        return "";
    }

    LPSTR buffer = nullptr;
    DWORD size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                nullptr,
                                error_code,
                                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                (LPSTR)&buffer,
                                0,
                                nullptr);
    if (size == 0) {
        return "";
    }

    std::string error_message(buffer, size);
    LocalFree(buffer);
    return error_message;
}

#else
#include <dlfcn.h>
#endif

DynamicLibMgr::DynamicLibMgr() : m_inited(false), m_handle(nullptr)
{

}

DynamicLibMgr::~DynamicLibMgr()
{
    UnloadLib();
}

DynamicLibMgr *DynamicLibMgr::Instance()
{
    static DynamicLibMgr instance;
    return &instance;
}

int DynamicLibMgr::LoadLib(const char *file)
{
    if (m_inited) {
        return 0;
    }

#if defined(_WIN32) || defined(_WIN64)
    HINSTANCE handle = LoadLibrary(file);
    if (handle == nullptr) {
        printf("load %s fail: %s\n", file, GetLastErrorMessage().c_str());
        return -1;
    }
#else
    void* handle = dlopen(file, RTLD_NOW|RTLD_GLOBAL);
    if (handle == nullptr) {
        printf("load %s fail: %s\n", file, dlerror());
        return -1;
    }
#endif

    m_inited = true;
    m_handle = handle;
    return 0;
}

void DynamicLibMgr::UnloadLib()
{
    if (m_handle != nullptr)
    {
#if defined(_WIN32) || defined(_WIN64)
        FreeLibrary(m_handle);
#else
        dlclose(m_handle);
#endif
        m_handle = nullptr;
    }
    m_inited = false;
}

void *DynamicLibMgr::GetSymbol(const char *symbolName)
{
#if defined(_WIN32) || defined(_WIN64)
    void* dl = GetProcAddress(m_handle, symbolName);
#else
    void* dl = dlsym(m_handle, symbolName);
#endif

    if (!dl) {
        printf("load %s symble not found\n", symbolName);
        return nullptr;
    }
    return dl;
}
