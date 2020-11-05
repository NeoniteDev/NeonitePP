#define _CRT_SECURE_NO_WARNINGS
#include "util.h"

int main(int argc, char* argv[])
{
	if (argc > 0)
	{
		for (int i = 1; i < argc; ++i) {
			std::string arg = argv[i];
			if (arg == "--debug") debug = !debug;
			//TODO: More
		}
	}
	
	char programData[MAX_PATH];
	char datPath[] = "\\Epic\\UnrealEngineLauncher\\LauncherInstalled.dat";
	json launcherInstalled;

	SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA,
	                NULL, 0, programData); //why would someone run this on linux ?

	//TODO: change this
	char* launcherInstalledPath = new char[std::strlen(programData) + std::strlen(datPath) + 1];

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
		else
		{
			i++;
		}
	}

	if (fnPath.empty())
	{
		printd(termcolor::red, "[x]Couldn't get fortnite path!.");
		return -1;
	}

	std::string szClientFile = fnPath + "\\FortniteGame\\Binaries\\Win64\\FortniteClient-Win64-Shipping.exe";
	std::string szEACFile = fnPath + "\\FortniteGame\\Binaries\\Win64\\FortniteClient-Win64-Shipping_EAC.exe";

	printd(termcolor::magenta, logo);

	printd(termcolor::blue,
	       "[=]Made by: kemo#1337, For neonite server\n[=]https://discord.com/invite/qSJ9jGp\n");

	if (debug)
		printd(termcolor::yellow,
		       "[=]DebugMode, Press F6 to pause/resume fortnite process\n");

	char* name = new char[32];
	printd(termcolor::yellow, "[>]Please enter your username (MAX 32): ");
	std::cin.getline(name, 33);
	
	std::ostringstream oss;
	oss <<
	" -epicapp=Fortnite -epicenv=Prod -epiclocale=en-us -epicportal -noeac -nobe -fltoken=8c4aa8a9b77acdcbd918874b -AUTH_LOGIN="
	<< name << "@unused.com -AUTH_PASSWORD=unused -AUTH_TYPE=epic";
	std::string s = oss.str();
	char* args = new char[s.length() + 1];
	std::copy(s.c_str(), s.c_str() + s.length() + 1, args);

	HANDLE Launcher = startup(szEACFile.c_str(), args);
	suspend(Launcher);

	HANDLE client = startup(szClientFile.c_str(), args);

	if (client && client != INVALID_HANDLE_VALUE)
	{
		printd(termcolor::green, "[+]Launched.\n");

		//prevent mapping the dll too fast
		while (pid == 0)
		{
			pid = GetProcId("FortniteClient-Win64-Shipping.exe");
		}

		std::string dllPath = GetEXEPath() + "\\Platanium.dll";

		if (!ManualMap(client, dllPath.c_str()))
		{
			CloseHandle(client);
			printd(termcolor::red, "[x]Something went wrong\n");
			system("PAUSE");
			return 0;
		}

		printd(termcolor::green, "[+]Injected.\n");

		while (true)
		{
			if (WaitForSingleObject(client, 10) != WAIT_TIMEOUT) break;
			if (GetAsyncKeyState(VK_F6) < 0 && debug)
			{
				if (!isPaused)
				{
					printd(termcolor::red, "[=]Paused.\n");
					suspend(client);
					isPaused = !isPaused;
				}
				else
				{
					printd(termcolor::green, "[=]Resumed.\n");
					resume(client);
					isPaused = !isPaused;
				}
			}
			Sleep(200); //SHUSH
		}
		TerminateProcess(Launcher, 1);
	}
}
