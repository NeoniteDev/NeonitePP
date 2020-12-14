#include "../util.h"
#include "../GUI/gui.h"


inline HANDLE hLauncher = INVALID_HANDLE_VALUE;
inline HANDLE hEAC = INVALID_HANDLE_VALUE;
inline HANDLE hClient = INVALID_HANDLE_VALUE;

namespace launcher
{
	inline void init()
	{
		char programData[MAX_PATH];
		char datPath[] = "\\Epic\\UnrealEngineLauncher\\LauncherInstalled.dat";
		json launcherInstalled;

		SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA,
		                NULL, 0, programData); //why would someone run this on linux ?

		//TODO: change this
		char* launcherInstalledPath = new char[
			std::strlen(programData) + std::strlen(datPath) + 1];

		std::strcpy(launcherInstalledPath, programData);
		std::strcat(launcherInstalledPath, datPath);

		std::ifstream f(launcherInstalledPath);
		f >> launcherInstalled;

		json installationList = launcherInstalled["InstallationList"];

		std::string fnPath;

		for (int i = 0; i < installationList.size(); i++)
		{
			if (installationList[i]["AppName"] == "Fortnite")
			{
				fnPath = installationList[i]["InstallLocation"];
			}
		}

		if (fnPath.empty())
		{
			console.AddLog("[x] Can't find the Fortnite directory.");
			return;
		}

		std::string szClientFile =
		fnPath +
		"\\FortniteGame\\Binaries\\Win64\\FortniteClient-Win64-Shipping.exe";
		std::string szEACFile =
		fnPath +
		"\\FortniteGame\\Binaries\\Win64\\FortniteClient-Win64-Shipping_EAC.exe";

		std::ostringstream oss;
		oss <<
		" -epicapp=Fortnite -epicenv=Prod -epiclocale=en-us -epicportal -noeac -nobe -fltoken=8c4aa8a9b77acdcbd918874b -AUTH_LOGIN="
		<< name
		<< "@unused.com -AUTH_PASSWORD=unused -AUTH_TYPE=epic";
		std::string s = oss.str();
		char* args = new char[s.length() + 1];
		std::copy(s.c_str(), s.c_str() + s.length() + 1, args);

		hEAC = startup(szEACFile.c_str(), args);
		suspend(hEAC);

		hClient = startup(szClientFile.c_str(), args);

		if (hClient && hClient != INVALID_HANDLE_VALUE)
		{
			console.AddLog("[+] Launched Fortnite.");

			//prevent mapping the dll too fast
			while (pid == 0)
			{
				pid = GetProcId("FortniteClient-Win64-Shipping.exe");
			}

			if (!addDll.empty())
			{
				std::string addDllPath = GetEXEPath() + "\\" + addDll;

				if (!ManualMap(hClient, addDllPath.c_str()))
				{
					CloseHandle(hClient);
					console.AddLog(
						"[x] The additional DLL couldn't be injected, an error was encountered.");
					return;
				}

				console.AddLog("[+] The additional DLL was injected due to user choice.");
			}

			std::string dllPath = GetEXEPath() + "\\Platanium.dll";

			if (!noInj)
			{
				if (!ManualMap(hClient, dllPath.c_str()))
				{
					CloseHandle(hClient);
					console.AddLog("[x] Couldn't inject Platanium.dll!");
					system("PAUSE");
					return;
				}

				//console.AddLog("[+] Injected Platanium.dll!");
			}
			else
			{
				console.
				AddLog("[=] Skipping Platanium.dll injection due to user choice.");
			}

			while (true)
			{
				if (WaitForSingleObject(hClient, 10) != WAIT_TIMEOUT) break;
				if (GetAsyncKeyState(VK_F6) && isDebug)
				{
					if (!isPaused)
					{
						console.AddLog("[=] Paused the process.");
						suspend(hClient);
						isPaused = !isPaused;
					}
					else
					{
						console.AddLog("[=] Resumed the process.");
						resume(hClient);
						isPaused = !isPaused;
					}
				}
			}
			TerminateProcess(hLauncher, 1);
		}
	}
}
