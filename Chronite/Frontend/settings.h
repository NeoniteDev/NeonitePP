#pragma once
#include "util.h"
#include "GUI/gui.h"

namespace settings
{
	inline bool config(bool sol)
	{
		std::string configPath = util::GetEXEPath() + "\\neonite.config";
		const char* configFile = configPath.c_str();
		json config = {
			{"name", name},
			{"path", path},
		};

		if (sol)
		{
			std::ofstream o(configFile);
			o << config;
			o.close();
			console.AddLog("[+] Config file was saved.");
		}
		else if (!sol)
		{
			try
			{
				std::ifstream i(configFile);
				i >> config;
				i.close();
				console.AddLog("[+] Config file was loaded.");
			}
			catch (...)
			{
				console.AddLog("[x] Invalid config file was loaded.");
				return false;
			}
			try
			{
				std::string sname = config["name"];
				std::string spath = config["path"];

				if (sname.size() > 32)
				{
					console.AddLog("[x] Name in config file is too long, falling back.");
					return false;
				}
				strncpy(name, sname.c_str(), 33);

				strncpy(path, spath.c_str(), spath.size() + 1);
			}
			catch (...)
			{
				console.AddLog("[x] Invalid config file was loaded.");
			}
		}

		return true;
	}

	//TODO: find path function.
	inline bool readLocker()
	{
		char programData[MAX_PATH];
		char datPath[] = "\\Epic\\UnrealEngineLauncher\\LauncherInstalled.dat";
		json launcherInstalled;
		std::string fnPath;

		SHGetFolderPath(nullptr, CSIDL_COMMON_APPDATA,
		                nullptr, 0, programData);

		//TODO: change this
		auto launcherInstalledPath = new char[
			std::strlen(programData) + std::strlen(datPath) + 1];

		std::strcpy(launcherInstalledPath, programData);
		std::strcat(launcherInstalledPath, datPath);

		std::ifstream f(launcherInstalledPath);
		f >> launcherInstalled;

		json installationList = launcherInstalled["InstallationList"];


		for (auto i = 0; i < installationList.size(); i++)
		{
			if (installationList[i]["AppName"] == "Fortnite")
			{
				fnPath = installationList[i]["InstallLocation"];
			}
		}

		std::string itemsPath = util::GetEXEPath() + "\\ids.config";
		auto itemsFile = itemsPath.c_str();
		try
		{
			std::ifstream i(itemsFile);

			std::string line;
			while (std::getline(i, line))
			{
				if (line.starts_with("Athena")) IDs.push_back(line.c_str());
			}
			i.close();
		}
		catch (...)
		{
			return false;
		}

		return true;
	}
}
