#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma warning( disable : 4267 4244 )

#include <iostream>

#include "gui.h"
#include "../launcher.h"
#include "../../Backend/server.h"
#include "..//settings.h"

using namespace std;

#include "../util.h"
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

static int FilterNoSpace(ImGuiTextEditCallbackData* data)
{
	if (data->EventChar == ' ') return 1;
	return 0;
}

static bool hasAdditional = false;

void ImGui::ShowLoader(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;

	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoSavedSettings;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoScrollWithMouse;

	SetNextWindowPos(ImVec2(500, 100), ImGuiCond_FirstUseEver);
	SetNextWindowSize(ImVec2(535, 320), ImGuiCond_FirstUseEver);


	if (!Begin("Neonite++", p_open, window_flags))
	{
		End();
		return;
	}

	PushItemWidth(GetFontSize() * -12);

	if (BeginTabBar("Neonite"), ImGuiTabBarFlags_AutoSelectNewTabs)
	{
		if (BeginTabItem("Main"))
		{
			if (Button("Start Server"))
			{
				if (hServer == INVALID_HANDLE_VALUE)
				{
					hServer = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&server::init,
					                       nullptr, NULL, nullptr);
					console.AddLog("[+] Server started...");
				}
				else
				{
					console.AddLog("[=] The server is already running.");
				}
			}

			SameLine(GetWindowWidth() - 390);

			InputTextWithHint(" ", "Input your Username", name, sizeof(name), ImGuiInputTextFlags_CallbackCharFilter,
			                  FilterNoSpace);

			SameLine(GetWindowWidth() - 100);

			if (strstr(name, " "))
			{
				memset(name, 0, sizeof(name));
			}

			if (Button("Start Game"))
			{
				if (hLauncher == INVALID_HANDLE_VALUE)
				{
					if constexpr (name == "")
					{
						console.AddLog("[x] Please input an username!");
					}

					if constexpr (sizeof(name) < 2)
					{
						console.AddLog("[x] Your name must be long 3 or more characters!");
					}

					hLauncher = CreateThread(nullptr, NULL,
					                         (LPTHREAD_START_ROUTINE)&launcher::init, nullptr,
					                         NULL, nullptr);
					console.AddLog("[+] Starting Game...");
				}
				else
				{
					console.AddLog("[=] The game is already running.");
				}
			}

			if (Button("Stop Server"))
			{
				if (hServer != INVALID_HANDLE_VALUE)
				{
					app.stop();
					TerminateThread(hServer, 0);
					hServer = INVALID_HANDLE_VALUE;
					console.AddLog("[x] The Server was stopped...");
				}
				else
				{
					console.AddLog("[=] The server isn't running..");
				}
			}

			SameLine(GetWindowWidth() - 100);

			if (Button("Close Game"))
			{
				if (hLauncher != INVALID_HANDLE_VALUE)
				{
					try
					{
						TerminateProcess(hEAC, 1);
					}
					catch (...)
					{
					}
					TerminateThread(hLauncher, 0);
					hLauncher = INVALID_HANDLE_VALUE;
					console.AddLog("[x] The game was terminated...");
				}
				else
				{
					console.AddLog("[=] The game wasn't running...");
				}
			}

			console.Draw("Server Console", p_open);

			SameLine(GetWindowWidth() - 325);

			if (Button("Save Config"))
			{
				settings::config(true);
			}

			SameLine(GetWindowWidth() - 100);

			if (Button("Load Config"))
			{
				settings::config(false);
			}

			EndTabItem();
		}
	}

	if (BeginTabItem("Launcher Settings"))
	{
		SetCursorPosX(GetCursorPosX() + 155);
		SetCursorPosY(GetCursorPosY() + 5);
		Text("Try some Launcher settings!");

		Checkbox("Additional DLL to Inject", &hasAdditional);

		SameLine(GetWindowWidth() - 200);

		Checkbox("Debug Mode (F6)", &isDebug);

		Checkbox("Skip Platanium.dll", &noInj);

		if (hasAdditional)
		{
			InputText("DLL Name", DllName, sizeof(DllName));
		}

		EndTabItem();
	}

	if (BeginTabItem("Server Settings"))
	{
		SetCursorPosX(GetCursorPosX() + 165);
		SetCursorPosY(GetCursorPosY() + 5);
		Text("Customize your Server!");

		Checkbox("Cataba Shop Style", &bIsCataba);

		Spacing();

		SetCursorPosX(GetCursorPosX() + 55);
		SetCursorPosY(GetCursorPosY() + 5);

		Text("*We are going to add more things soon. Stay updated.*");

		EndTabItem();
	}

	if (BeginTabItem("Profile Settings"))
	{
		SetCursorPosX(GetCursorPosX() + 115);
		SetCursorPosY(GetCursorPosY() + 5);
		Text("Modify your profile as much as you want!");

		InputInt("    Season Level", &seasonLevel);

		InputInt("    VBucks Amount", &vbucks);

		if (vbucks > 2147483647) {
			vbucks = 0;
		}
		
		if (seasonLevel > 1000) {
			seasonLevel = 100;
		}
		
		Checkbox("Has Battlepass", &bHasBattlepass);

		SameLine(GetWindowWidth() - 205);

		Checkbox("Skin Locker", &bIsLockerOpen);

		bool didntImport = true;

		if (bIsLockerOpen)
		{
			if (Begin("Locker", nullptr,
			          ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse))
			{
				if (!bHasImportedLocker)
				{
					settings::loadLocker(false);
				}
				SetWindowPos(ImVec2(300, 100), ImGuiCond_FirstUseEver);
				SetWindowSize(ImVec2(500, 460), ImGuiCond_FirstUseEver);

				std::string lowID = ID;

				boost::to_lower(lowID);

				InputTextWithHint("", "AthenaDefinition:ID", ID, sizeof(ID));
				SameLine();
				if (Button("+"))
				{
					//if (lowID.size() > 0) 
					if (std::find(IDs.begin(), IDs.end(), lowID) != IDs.end())
					{
						if (!strstr(lowID.c_str(), "athena"))
						{
							MessageBoxA(window, "The item added doesn't seem to be existing.",
							            "Neonite++", ERROR);
						}
						else
						{
							IDs.push_back(ID);
							LockerBackup.push_back(ID);
						}
					}
					else
					{
						MessageBoxA(window, "The item is already in the list.",
						            "Neonite++", ERROR);
					}
				}

				SameLine();
				if (Button("-"))
				{
					if (!strstr(lowID.c_str(), "athena"))
					{
						MessageBoxA(window, "This item isn't correct.", "Neonite++",
						            ERROR);
					}
					else
					{
						std::vector<std::string>::iterator it = std::find(
							IDs.begin(), IDs.end(), ID);

						int id = std::distance(IDs.begin(), it);


						if (it != IDs.end())
						{
							IDs.erase(IDs.begin() + id);
							//LockerBackup.erase(LockerBackup.begin() + id);
						}
						else
						{
							MessageBoxA(window, "This item doesn't seem to exist.",
							            "Neonite++", ERROR);
						}
					}
				}

				SameLine();
				if (Button(("Erase")))
				{
					if (IDs.empty())
					{
						MessageBoxA(window, "The list is already empty!", "Neonite++",
						            ERROR);
					}
					else
					{
						IDs.clear();
					}
				}
				SameLine();
				if (Button(("Save Locker")))
				{
					settings::loadLocker(true);
				}

				Columns(1, "");
				Separator();
				SetCursorPosX(GetCursorPosX() + 205);
				SetCursorPosY(GetCursorPosY() + 5);
				Text("ID");
				Separator();

				int length = IDs.size();

				static int selected = -1;

				if (bHasImportedLocker)
				{
					for (int i = 0; i < length; i++)
					{
						if (Selectable(IDs.at(i).c_str(), selected == i,
						               ImGuiSelectableFlags_SpanAllColumns))
							selected = i;

						NextColumn();
						Separator();
					}
				}
				else if (didntImport)
				{
					for (int i = 0; i < length; i++)
					{
						if (Selectable(IDs.at(i).c_str(), selected == i,
						               ImGuiSelectableFlags_SpanAllColumns))
							selected = i;

						NextColumn();
						Separator();
					}
				}

				End();
			}
		}
		EndTabItem();
	}

	if (BeginTabItem("Credits"))
	{
		SetCursorPosX(GetCursorPosX() + 145);
		SetCursorPosY(GetCursorPosY() + 5);

		float cX = GetCursorPosY();
		float cY = GetCursorPosY();

		Text("Backend: Kemo (@xkem0x)");

		SetCursorPosX(GetCursorPosX() + 145);
		SetCursorPosY(GetCursorPosY() + 5);

		Text("Frontend: Sammy (@madSammy)");

		SetCursorPosX(GetCursorPosX() + 145);
		SetCursorPosY(GetCursorPosY() + 5);

		Text("Manager: Ayal (@Ayal01)");

		SetCursorPosX(GetCursorPosX() + 145);
		SetCursorPosY(GetCursorPosY() + 5);

		Text("Rendering Engine: Dear, ImGui.");

		SetCursorPosY(GetCursorPosY() + 115);
		SetCursorPosX(GetCursorPosX() + 3.50);

		if (Button("Join Server"))
		{
			system("start https://discord.com/invite/qSJ9jGp");
		}

		SameLine(GetWindowWidth() - 150);

		if (Button("Github Repository"))
		{
			system("start https://github.com/NeoniteDev");
		}

		SameLine(GetWindowWidth() - 315);

		if (Button("Website"))
		{
			system("start https://neonite.dev");
		}

		EndTabItem();
	}

	EndTabBar();

	if (*p_open == false)
	{
		TerminateThread(hLauncher, 0);
		TerminateThread(hServer, 0);
		exit(1);
	}

	End();

	ImGuiStyle* style = &GetStyle();

	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style->WindowPadding = ImVec2(15, 8);
	style->WindowRounding = 3.0f;
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
