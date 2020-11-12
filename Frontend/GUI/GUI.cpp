#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <iostream>


#include "gui.h"
#include "../Launcher/launcher.h"
#include "../../Backend/server.h"
#include "../Settings/settings.h"

using namespace std;

#include "../../includes/imgui/imgui.h"
#include <limits.h>         // INT_MIN, INT_MAX
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>         // intptr_t
#else
#include <stdint.h>         // intptr_t
#endif


#ifdef _MSC_VER
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif

// Play it nice with Windows users. Notepad in 2017 still doesn't display text data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE  "\r\n"
#define snprintf    _snprintf
#define vsnprintf   _vsnprintf
#else
#define IM_NEWLINE  "\n"
#endif

#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))-

#if !defined(IMGUI_DISABLE_OBSOLETE_FUNCTIONS) && defined(IMGUI_DISABLE_TEST_WINDOWS) && !defined(IMGUI_DISABLE_DEMO_WINDOWS)   // Obsolete name since 1.53, TEST->DEMO
#define IMGUI_DISABLE_DEMO_WINDOWS
#endif

#if !defined(IMGUI_DISABLE_DEMO_WINDOWS)

const char *ShopTypes[] = {" Default", " Cataba"};
const char *Stages[] = {" Default", " Season 14", " Season 13", " Season 12",
                        " Season 11", " Season 10 (X)", " Winterfest",
                        " FortniteMares", " Summer", " BatMan", " Star Wars",
                        " World Cup"};


void Center(std::string text, bool isText, bool isButton) {
  float font_size = ImGui::GetFontSize() * text.size() / 2;
  ImGui::SameLine(
      ImGui::GetWindowSize().x / 2 -
      font_size + (font_size / 2)
      );

  if (isText) {
    ImGui::Text("%s", text);
  }

  const char *text2 = text.c_str();

  if (isButton) {
    ImGui::Button(text2);
  }

  ImGui::NewLine();
}


static void HelpMarker(const char *desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

static bool hasAdditional = false;

void ImGui::ShowLoader(bool *p_open) {
  // IMGUI FLAGS

  ImGuiWindowFlags window_flags = 0;

  window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoCollapse;
  window_flags |= ImGuiWindowFlags_NoSavedSettings;
  window_flags |= ImGuiWindowFlags_NoScrollbar;
  window_flags |= ImGuiWindowFlags_NoScrollWithMouse;

  SetNextWindowPos(ImVec2(500, 100), ImGuiCond_FirstUseEver);
  SetNextWindowSize(ImVec2(535, 320), ImGuiCond_FirstUseEver);

  // FORM
  if (!Begin("Neonite++", p_open, window_flags)) {
    End();
    return;
  }

  PushItemWidth(GetFontSize() * -12);

  if (BeginTabBar("Neonite"), ImGuiTabBarFlags_AutoSelectNewTabs) {
    if (BeginTabItem("Main")) {
      /*SetCursorPosX(GetCursorPosX() + 150);
      SetCursorPosY(GetCursorPosY() + 10);


      //Text("Welcome to Neonite++!");*/

      //ImGui::Dummy(ImVec2(0.0f, 10.0f));

      if (Button("Start Server")) {
        if (Server == INVALID_HANDLE_VALUE) {
          Server = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&server,
                                nullptr, NULL, nullptr);
          console.AddLog("[+] Server started...");
        } else {
          console.AddLog("[=] The server is already running.");
        }
      }

      SameLine(GetWindowWidth() - 390);

      InputTextWithHint(" ", "Input your Username", name, sizeof(name));

      SameLine(GetWindowWidth() - 100);

      if (Button("Start Game")) {
        if (hLauncher == INVALID_HANDLE_VALUE) {
          if constexpr (name == "") {
            console.AddLog("[x] Please input an username!");
          }
          if constexpr (sizeof(name) < 2) {
            console.AddLog("[x] Your name must be long 3 or more characters!");
          }

          hLauncher = CreateThread(nullptr, NULL,
                                   (LPTHREAD_START_ROUTINE)&launcher, nullptr,
                                   NULL, nullptr);
          console.AddLog("[+] Starting Game...");
        } else {
          console.AddLog("[=] The game is already running.");
        }
      }

      if (Button("Stop Server")) {
        if (Server != INVALID_HANDLE_VALUE) {
          TerminateThread(Server, 0);
          Server = INVALID_HANDLE_VALUE;
          console.AddLog("[x] The Server was stopped...");
        } else {
          console.AddLog("[=] The server isn't running..");
        }
      }

      SameLine(GetWindowWidth() - 100);

      if (Button("Close Game")) {
        if (hLauncher != INVALID_HANDLE_VALUE) {
          try {
            //this should kill your game at all :pepelaugh:
            // kek -sammy
            TerminateProcess(hEAC, 1);
          } catch (...) {
          }
          TerminateThread(hLauncher, 0);
          hLauncher = INVALID_HANDLE_VALUE;
          console.AddLog("[x] The game was terminated...");
        } else {
          console.AddLog("[=] The game wasn't running...");
        }
      }

      console.Draw("Server Console", p_open);

      SameLine(GetWindowWidth() - 325);

      if (Button("Save Config")) {
        config(true);
      }

      SameLine(GetWindowWidth() - 100);

      if (Button("Load Config")) {
        config(false);
      }

      EndTabItem();
    }
  }

  if (BeginTabItem("Launcher Settings")) {
    SetCursorPosX(GetCursorPosX() + 155);
    SetCursorPosY(GetCursorPosY() + 5);
    Text("Try some Launcher settings!");

    Checkbox("Additional DLL to Inject", &hasAdditional);

    SameLine(GetWindowWidth() - 200);

    Checkbox("Debug Mode (F6)", &isDebug);

    Checkbox("Skip Platanium.dll", &noInj);

    /*SameLine(GetWindowWidth() - 200);

    Checkbox("Use Discord RPC", &bUseRPC);*/

    if (hasAdditional) {
      InputText("DLL Name", DllName, sizeof(DllName));
    }

    EndTabItem();
  }

   
  if (BeginTabItem("Server Settings")) {
    SetCursorPosX(GetCursorPosX() + 165);
    SetCursorPosY(GetCursorPosY() + 5);
    Text("Customize your Server!");

    static int SelectedStage = 0;
    static int SelectedShopStyle = 0;
    static bool bEmergency = false;
    static bool bIsEmergencyHidden = false;
    static bool bIsEmergencySpotlight = false;
    static bool bNews = false;
    static bool bIsHidden = false;
    static bool bIsSpotlight = false;

    /*Combo(("Background Stage"), &SelectedStage, Stages, IM_ARRAYSIZE(Stages));

    Checkbox("Emergency Notice", &bEmergency);

    SameLine(GetWindowWidth() - 200);*/

    Checkbox("Cataba Shop Style", &bIsCataba);

    //SameLine(GetWindowWidth() - 200);

    /*if (bEmergency)
    {
            if (Begin("Emergency Notice", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                    SetNextWindowPos(ImVec2(300, 100), ImGuiCond_FirstUseEver);
                    SetNextWindowSize(ImVec2(300, 320), ImGuiCond_FirstUseEver);

                    InputTextWithHint(" Title", "Insert the title!", newsTitle, sizeof(newsTitleE));

                    InputTextWithHint(" Body", "Insert the body!", newsBody, sizeof(newsBodyE));

                    Checkbox("Hidden", &bIsEmergencyHidden);

                    SameLine(GetWindowWidth() - 150);

                    Checkbox("Spotlight", &bIsEmergencySpotlight);

                    End();
            }
    }*/

    Spacing();

    SetCursorPosX(GetCursorPosX() + 6);
    SetCursorPosY(GetCursorPosY() + 3);

    ImGui::Text("*We are going to add more things soon. Stay updated.*");

    /*ImGui::Checkbox("News", &bNews);
    if (bNews) {
            ImGui::InputTextWithHint(" Image", "Insert the image URL!", newsTitle, sizeof(imageURL));
            ImGui::InputTextWithHint(" Title", "Insert the title!", newsTitle, sizeof(newsTitle));
            ImGui::InputTextWithHint(" Body", "Insert the body!", newsBody, sizeof(newsBody));
            ImGui::Checkbox("Hidden", &bIsHidden);

            SameLine(GetWindowWidth() - 360);

            ImGui::Checkbox("Spotlight", &bIsSpotlight);
    }*/
    // madboy didnt want it here kek

    EndTabItem();
  }

  if (BeginTabItem("Profile Settings")) {
    SetCursorPosX(GetCursorPosX() + 115);
    SetCursorPosY(GetCursorPosY() + 5);
    Text("Modify your profile as much as you want!");

    InputInt("    Season Level", &seasonLevel);

    InputInt("    VBucks Amount", &vbucks);

    Checkbox("Has Battlepass", &bHasBattlepass);

    SameLine(GetWindowWidth() - 205);

    Checkbox("Skin Locker", &bIsLockerOpen);

    bool didntImport = true;

    if (bIsLockerOpen) {
      if (Begin("Locker", nullptr,
                ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse)) {
        if (!bHasImportedLocker) {
          loadLocker(false);
        }
        SetWindowPos(ImVec2(300, 100), ImGuiCond_FirstUseEver);
        SetWindowSize(ImVec2(500, 460), ImGuiCond_FirstUseEver);

        std::string lowID = ID;

        boost::to_lower(lowID);

        InputTextWithHint("", "AthenaDefinition:ID", ID, sizeof(ID));
        SameLine();
        if (Button("+")) {
          if (!strstr(lowID.c_str(), "athena")) {
            MessageBoxA(nullptr, "The item added doesn't seem to be existing.",
                        "Neonite++", ERROR);
          }

          IDs.push_back(ID);
          LockerBackup.push_back(ID);
        }

        SameLine();
        if (Button("-")) {
          if (!strstr(lowID.c_str(), "athena")) {
            MessageBoxA(nullptr, "This item isn't correct.", "Neonite++",
                        ERROR);
          } else {
            //bool it = std::find(CIDs.begin(), CIDs.end(), "abc") != CIDs.end();
            std::vector<std::string>::iterator it = std::find(
                IDs.begin(), IDs.end(), ID);

            int id = std::distance(IDs.begin(), it);

            //printf("%d", id);

            //if (bHasImportedLocker && !ImportedLocker.empty()) IDs = ImportedLocker; )

            if (it != IDs.end()) {
              IDs.erase(IDs.begin() + id);
              //LockerBackup.erase(LockerBackup.begin() + id);
            } else {
              MessageBoxA(nullptr, "This item doesn't seem to exist.",
                          "Neonite++", ERROR);
            }

            //CIDs.pop_back(ID);
          }
        }

        SameLine();
        if (Button(("Erase"))) {
          if (IDs.empty()) {
            MessageBoxA(nullptr, "The list is already empty!", "Neonite++",
                        ERROR);
          } else {
            IDs.clear();
          }
        }
        SameLine();
        if (Button(("Save Locker"))) {
          loadLocker(true);
        }
        /*
                                        SetCursorPosX(GetCursorPosX() + 215);
                                        SetCursorPosY(GetCursorPosY() + 5);

                                        if (Button(("Load Locker")))
                                        {

                                        }*/

        Columns(1, "");
        Separator();
        SetCursorPosX(GetCursorPosX() + 205);
        SetCursorPosY(GetCursorPosY() + 5);
        Text("ID");
        //ImGui::NextColumn();
        /*ImGui::Text("Item Name");
        ImGui::NextColumn();*/
        //ImGui::Text("Style");
        //ImGui::NextColumn();
        Separator();

        int length = IDs.size();

        static int selected = -1;

        std::vector <std::string>  test;
        std::string test2 = "patata";

        test.push_back(test2);
        int size = test.size();

        for (int i = 0; i < size; i++) {
            printf("%s", test.at(i).c_str());
        }

        if (bHasImportedLocker) {
          for (int i = 0; i < length; i++) {
            char label[32];
            if (Selectable(IDs.at(i).c_str(), selected == i,
                           ImGuiSelectableFlags_SpanAllColumns))
              selected = i;

            bool hovered = IsItemHovered();
            //ImGui::NextColumn();
            //ImGui::Text( "%s", CIDs.at(i).c_str()); // in case of removal of selectable tabs, uncomment, the selection is useless but idk. - sammy

            NextColumn();
            Separator(); // maybe? idk - sammy
          }
        } else if (didntImport) {
          for (int i = 0; i < length; i++) {
            char label[32];
            if (Selectable(IDs.at(i).c_str(), selected == i,
                           ImGuiSelectableFlags_SpanAllColumns))
              selected = i;

            bool hovered = IsItemHovered();
            //ImGui::NextColumn();
            //ImGui::Text( "%s", CIDs.at(i).c_str()); // in case of removal of selectable tabs, uncomment, the selection is useless but idk. - sammy

            NextColumn();
            Separator(); // maybe? idk - sammy
          }
        }

        End();
      }
    }
    EndTabItem();
  }



  /*if (BeginTabItem("Shop"))
  {
          SetCursorPosX(GetCursorPosX() + 150);
          SetCursorPosY(GetCursorPosY() + 10);


          Text("Customize your Item Shop!");

          ImGui::InputTextWithHint("Featured", " CID of the Skin", ID, sizeof(ID));
          ImGui::InputTextWithHint("VBucks Price", "VBucks Price", pricesDE, sizeof(pricesDE));

          ImGui::InputTextWithHint("Daily", " CID of the Skin", ID, sizeof(ID));
          ImGui::InputTextWithHint("VBucks Price", " Price", prices_, sizeof(prices_));


          SameLine(GetWindowWidth() - 200);


          EndTabItem();
  }*/

  if (BeginTabItem("Credits")) {
    SetCursorPosX(GetCursorPosX() + 145);
    SetCursorPosY(GetCursorPosY() + 5);

    float cX = GetCursorPosY();
    float cY = GetCursorPosY();

    Text("Backend: Kemo (@xkem0x)");

    SetCursorPosX(GetCursorPosX() + 145);
    SetCursorPosY(GetCursorPosY() + 5);

    Text("Frontend: Sammy (@sammyisntcool_)");

    SetCursorPosX(GetCursorPosX() + 145);
    SetCursorPosY(GetCursorPosY() + 5);

    Text("Manager: Ayal (@Ayal01)");

    SetCursorPosX(GetCursorPosX() + 145);
    SetCursorPosY(GetCursorPosY() + 5);

    Text("Rendering Engine: Dear, ImGui.");

    SetCursorPosY(GetCursorPosY() + 115);
    SetCursorPosX(GetCursorPosX() + 3.50);

    if (Button("Join Server")) {
      system("start https://discord.com/invite/qSJ9jGp");
    }

    SameLine(GetWindowWidth() - 150);

    if (Button("Github Repository")) {
      system("start https://github.com/NeoniteDev");
    }

    SameLine(GetWindowWidth() - 315);

    if (Button("Website")) {
      system("start https://neonite.dev");
    }

    EndTabItem();
  }

  EndTabBar();

  if (*p_open == false) {
    exit(1);
  }

  End();

  ImGuiStyle *style = &GetStyle();

  style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
  style->WindowPadding = ImVec2(15, 8);
  style->WindowRounding = 5.0f;
  style->FramePadding = ImVec2(5, 5);
  style->FrameRounding = 4.0f;
  style->ItemSpacing = ImVec2(12, 8);
  style->ItemInnerSpacing = ImVec2(8, 6);
  style->IndentSpacing = 25.0f;
  style->ScrollbarSize = 15.0f;
  style->ScrollbarRounding = 9.0f;
  style->GrabMinSize = 5.0f;
  style->GrabRounding = 3.0f;

  style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
  style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
  style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 0.9f);
  style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
  style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
  style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
  style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
  style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
  style->Colors[ImGuiCol_TitleBg] = ImColor(85, 0, 255);
  style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(85, 0, 255);
  style->Colors[ImGuiCol_TitleBgActive] = ImColor(85, 0, 255);
  style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
  style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
  style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
  style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(
      0.56f, 0.56f, 0.58f, 1.00f);
  style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(
      0.06f, 0.05f, 0.07f, 1.00f);
  style->Colors[ImGuiCol_CheckMark] = ImColor(85, 0, 255);
  style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
  style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
  style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
  style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
  style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
  style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
  style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
  style->Colors[ImGuiCol_Separator] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
  style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
  style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  style->Colors[ImGuiCol_ResizeGripHovered] =
      ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
  style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
  style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
  style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
  style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
  style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(
      0.25f, 1.00f, 0.00f, 1.00f);
  style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
  style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(
      1.00f, 0.98f, 0.95f, 0.73f);
}

#endif
