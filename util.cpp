#include "util.h"

HANDLE startup(LPCSTR lpApplicationName, LPSTR lpArguments)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcessA(lpApplicationName,
        lpArguments,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        CloseHandle(pi.hThread);
        return pi.hProcess;
    }
    return NULL;
}

//https://stackoverflow.com/questions/11010165/how-to-suspend-resume-a-process-in-windows
typedef LONG(NTAPI* NtSuspendProcess)(IN HANDLE ProcessHandle);
typedef LONG(NTAPI* NtResumeProcess)(IN HANDLE ProcessHandle);

NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
    GetModuleHandle("ntdll"), "NtSuspendProcess");

NtResumeProcess pfnNtResumeProcess = (NtResumeProcess)GetProcAddress(
    GetModuleHandle("ntdll"), "NtResumeProcess");


void suspend(HANDLE processHandle)
{
    pfnNtSuspendProcess(processHandle);
}

void resume(HANDLE processHandle)
{
    pfnNtResumeProcess(processHandle);
}

DWORD GetProcId(const char* procName)
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
                if (!_stricmp(procEntry.szExeFile, procName))
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

std::string GetEXEPath()
{
    char result[MAX_PATH];
	std::string path(result, GetModuleFileName(NULL, result, MAX_PATH));
    size_t pos = path.find_last_of("\\/");
    return (std::string::npos == pos)
        ? ""
        : path.substr(0, pos);
}