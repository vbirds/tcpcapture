
#pragma once

class DynamicLibMgr
{
public:
    DynamicLibMgr();
    ~DynamicLibMgr();

    DynamicLibMgr(const DynamicLibMgr&) = delete;
    DynamicLibMgr& operator=(const DynamicLibMgr&) = delete;

    static DynamicLibMgr * Instance();

    int LoadLib(const char* file);
    void UnloadLib();

    bool IsInited() const { return m_inited; }

    void* GetSymbol(const char* symbolName);

private:
    bool m_inited;
    void* m_handle;

};