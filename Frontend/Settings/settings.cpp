#define _CRT_SECURE_NO_WARNINGS
#include "settings.h"

bool config(bool sol) {
  std::string configPath = GetEXEPath() + "\\neonite.config";
  const char *configFile = configPath.c_str();
  json config = {
      {"name", name},
      {"vbucks", vbucks},
      {"seasonLevel", seasonLevel},
      {"bHasBattlepass", bHasBattlepass},
      {"bIsCataba", bIsCataba}
  };

  if (sol) {
    std::ofstream o(configFile);
    o << config;
    o.close();
    console.AddLog("[+] Config file was saved.");
  } else if (!sol) {
    try {
      std::ifstream i(configFile);
      i >> config;
      i.close();
      console.AddLog("[+] Config file was loaded.");
    } catch (...) {
      console.AddLog("[x] Invalid config file was loaded.");
      return false;
    }
    try {
      std::string sname = config["name"];
      int slevel = config["seasonLevel"];
      int sbucks = config["vbucks"];
      bool isCataba = config["bIsCataba"];
      bool hasBattlepass = config["bHasBattlepass"];

      if (sname.size() > 32) {
        console.AddLog("[x] Name in config file is too long, falling back.");
        return false;
      }
      strncpy(name, sname.c_str(), 33);

      seasonLevel = slevel;
      vbucks = sbucks;
      bIsCataba = isCataba;
      bHasBattlepass = hasBattlepass;
    } catch (...) {
      console.AddLog("[x] Invalid config file was loaded.");
    }
  }

  return true;
}

void loadLocker(bool sol) {
  if (sol) {
    try {
      std::string itemsPath = GetEXEPath() + "\\items.config";
      const char *itemsFile = itemsPath.c_str();
      std::fstream i(itemsFile,
                     std::fstream::in | std::fstream::out |
                     std::fstream::trunc);

      std::ostream_iterator<std::string> output_iterator(i, "\n");
      std::copy(IDs.begin(), IDs.end(), output_iterator);
      //bHasImportedLocker = false;
    } catch (...) {
      console.AddLog("[x] Impossible to import locker.");
    }
  }

  if (!sol) {
    std::string itemsPath = GetEXEPath() + "\\items.config";
    const char *itemsFile = itemsPath.c_str();
    std::ifstream i(itemsFile);

    std::string line;
    while (std::getline(i, line)) {
      printf("%s", line.c_str());
      IDs.push_back(line.c_str());
    }
    i.close();
    bHasImportedLocker = true;
  }
}
