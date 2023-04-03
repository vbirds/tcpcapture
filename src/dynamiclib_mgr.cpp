#include "dynamiclib_mgr.h"

#include "tcpcapture.h"

#include <stdio.h>
#include <dlfcn.h>


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
    void* handle = dlopen(file, RTLD_NOW|RTLD_GLOBAL);
    if (!handle) {
        printf("load %s fail: %s\n", file, dlerror());
        return -1;
    }

    m_inited = true;
    m_handle = handle;
    return 0;
}

void DynamicLibMgr::UnloadLib()
{
    if (m_handle != nullptr)
    {
        dlclose(m_handle);
        m_handle = nullptr;
    }
    m_inited = false;
}

void *DynamicLibMgr::GetSymbol(const char *symbolName)
{
    void* dl = dlsym(m_handle, symbolName);
    if (!dl) {
        printf("load %s symble not found\n", symbolName);
        return nullptr;
    }
    return dl;
}
