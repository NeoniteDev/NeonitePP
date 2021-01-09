#pragma once
#include "../includes/framework.h"

using json = nlohmann::json;

typedef LONG (NTAPI* NtSuspendProcess)(IN HANDLE ProcessHandle);
typedef LONG (NTAPI* NtResumeProcess)(IN HANDLE ProcessHandle);

inline NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
	GetModuleHandle("ntdll"), "NtSuspendProcess");

inline NtResumeProcess pfnNtResumeProcess = (NtResumeProcess)GetProcAddress(
	GetModuleHandle("ntdll"), "NtResumeProcess");

//Inline vars
inline bool bUseRPC = true;
inline HWND window;
inline ULONG pid = 0;

//config
inline char name[33] = "NeoniteUser";
inline bool bHasBattlepass = false;
inline bool bIsCataba = false;
inline bool bPaksIds = false;
inline int seasonLevel = 100;
inline int vbucks = 100000;

//locker
inline char ID[125];
inline std::vector<std::string> IDs;
inline std::vector<std::string> LockerBackup;


namespace util
{
	inline std::string GetEXEPath()
	{
		char result[MAX_PATH];
		std::string path(result, GetModuleFileName(nullptr, result, MAX_PATH));
		size_t pos = path.find_last_of("\\/");
		return (std::string::npos == pos)
			       ? ""
			       : path.substr(0, pos);
	}

	inline DWORD GetProcId(const char* procName)
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
				}
				while (Process32Next(hSnap, &procEntry));
			}
		}
		CloseHandle(hSnap);
		return procId;
	}

	inline void suspend(HANDLE processHandle)
	{
		pfnNtSuspendProcess(processHandle);
	}

	inline void resume(HANDLE processHandle)
	{
		pfnNtResumeProcess(processHandle);
	}

	inline HANDLE startup(LPCSTR lpApplicationName, LPSTR lpArguments)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		if (CreateProcessA(lpApplicationName,
		                   lpArguments,
		                   nullptr,
		                   nullptr,
		                   FALSE,
		                   0,
		                   nullptr,
		                   nullptr,
		                   &si,
		                   &pi
		))
		{
			CloseHandle(pi.hThread);
			return pi.hProcess;
		}
		return nullptr;
	}

	inline std::string genRandom(const int len)
	{
		std::string tmp_s;
		static const char alphanum[] =
		"0123456789"
		"abcdefghijklmnopqrstuvwxyz";

		srand(static_cast<unsigned>(time(nullptr)) * _getpid());

		for (int i = 0; i < len; ++i) tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


		return tmp_s;
	}

	inline std::string sSplit(std::string s, std::string delimiter)
	{
		size_t pos = 0;
		std::string token;
		while ((pos = s.find(delimiter)) != std::string::npos)
		{
			token = s.substr(0, pos);
			return token;
		}
	}

	inline std::string getQuery(std::string s, std::string query)
	{
		size_t pos = 0;
		std::string token;
		if ((pos = s.find(query)) != std::string::npos)
		{
			token = s.substr(pos);
			token.erase(0, query.length() + 1);
			size_t end;
			try
			{
				end = token.find("&");
				token.erase(end);
				return token;
			}
			catch (...)
			{
				return token;
			}
		}
		return "failed";
	}

	inline std::string urlDecode(std::string SRC)
	{
		std::string ret;
		char ch;
		int i, ii;
		for (i = 0; i < SRC.length(); i++)
		{
			if (static_cast<int>(SRC[i]) == 37)
			{
				sscanf(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
				ch = static_cast<char>(ii);
				ret += ch;
				i = i + 2;
			}
			else
			{
				ret += SRC[i];
			}
		}
		return (ret);
	}

	constexpr unsigned int str2int(const char* str, int h = 0)
	{
		return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
	}
}
