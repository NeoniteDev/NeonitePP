#include "../framework.h"
#include "ImGui/imgui.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT (__stdcall* Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

Present oPresent;
HWND window = nullptr;
WNDPROC oWndProc;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* mainRenderTargetView;

using namespace ImGui;
using namespace Neoroyale;

void InitImGui()
{
	CreateContext();
	ImGuiIO& io = GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYUP && (wParam == 0x2D /*INSERT*/ || (showMenu && wParam == VK_ESCAPE)))
	{
		showMenu = !showMenu;
		GetIO().MouseDrawCursor = showMenu;
	}
	else if (uMsg == WM_QUIT && showMenu)
	{
		ExitProcess(0);
	}

	if (showMenu)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return TRUE;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool initgui = false;

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!initgui)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			initgui = true;
		}

		else return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();

	ImVec4* colors = GetStyle().Colors;
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

	colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImColor(85, 0, 255);
	colors[ImGuiCol_TitleBgCollapsed] = ImColor(85, 0, 255);
	colors[ImGuiCol_TitleBgActive] = ImColor(85, 0, 255);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(
		0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(
		0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImColor(85, 0, 255);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_ResizeGripHovered] =
	ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(
		0.25f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(
		1.00f, 0.98f, 0.95f, 0.73f);


	if (showMenu)
	{
		if (Begin(XOR("Neonite++"), nullptr, ImGuiWindowFlags_NoCollapse))
		{
			PushItemWidth(GetFontSize() * -12);

			if (BeginTabBar("Neonite"), ImGuiTabBarFlags_AutoSelectNewTabs)
			{
				if (PlayerPawn)
				{
					if (BeginTabItem("World"))
					{
						/*const char* TODMs[] = { 
							"TODM_Disabled_C",
							"TODM_BR_C",
							"TODM_BR_Halloween_C",
							"TODM_BR_S7_C",
							"TODM_BR_S7_W_C",
							"TODM_BR_S8_C",
							"TODM_BR_S8Hot_C",
							"TODM_BR_S8_Child_C",
							"TODM_Disabled_C",
							"TODM_PG_PS_Capture_C",
							"TODM_Papaya_C",
							"TODM_BR_floorislava_V01_C",
							"TODM_BR_s11_Fortnitemares_v2_C",
							"TODM_BR_s11_STATIC_SKYLIGHT_C",
							"TODM_BR_s13_FogOfWar_v2_C",
							"TODM_BR_s14_C",
							"TODM_BR_s14_Fortnitemares_C",
							"TODM_BR_s14_Fortnitemares_Thick_C",
							"TODM_BR_s14_Fortnitemares_Thin_C",
							"TODM_Creative_C",
							"TODM_STW_Parent_C",
							"TODM_24_Fall_C",
							"TODM_24_Fall_Storm_C",
							"TODM_STW_Arid-WtS_R99_C",
							"TODM_STW_Arid-WtS_R99_V2_C",
							"TODM_STW_Arid-WtS_WW_C",
							"TODM_STW_Arid-WtS_WW_V3_C",
							"TODM_STW_Arid-WtS_WW_V4_C",
							"TODM_STW_Arid_WtS_WW_V2_C",
							"TODM_STW_Dudebro_C",
							"TODM_STW_Onboarding_C",
							"TODM_STW_Temperate-FtS_C",
							"TODM_STW_TRV_C",
							"TODM_STW_TRV_SW_BF_C",
							"TODM_STW_TRV_SW_DF_C",
							"TODM_STW_TRV_SW_LF_C",
							"TODM_STW_TRV_SW_TF_C",
							"TODM_STW_Winter-Endless_C",
							"TODM_STW_Winter-Survival_C",
							"TODM_STW_Winter_2018_C" 
						};
	
						static int currentTODM = 0;
						static int TODM = 0;
						Combo("TODM", &TODM, TODMs, IM_ARRAYSIZE(TODMs));
						if (currentTODM != TODM)
						{
							UFunctions::Summon(TODMs[TODM].c_str());
						}*/

						if (Button("Summon Husk"))
						{
							UFunctions::Summon(L"HUSKPAWN_C");
						}

						SameLine();

						if (Button("Summon Smasher"))
						{
							UFunctions::Summon(L"SMASHERPAWN_C");
						}

						SameLine();

						if (Button("Summon Storm King"))
						{
							UFunctions::Summon(L"DUDEBRO_Pawn_C");
						}

						SameLine();

						if (Button("Start Event"))
						{
							if (gVersion == XOR("14.60"))
							{
								UFunctions::Play(GALACTUS_EVENT_PLAYER);
							}
							else if (gVersion == XOR("12.41"))
							{
								UFunctions::Play(JERKY_EVENT_PLAYER);
							}
							else if (gVersion == XOR("12.61"))
							{
								UFunctions::Play(DEVICE_EVENT_PLAYER);
							}
							else
							{
								UFunctions::ConsoleLog(XOR(L"Sorry the version you are using doesn't have any event we support."));
							}
						}

						static int timeOfDay = 1;
						static int currentTimeOfDay = 1;

						if (currentTimeOfDay != timeOfDay)
						{
							UFunctions::SetTimeOfDay(timeOfDay);
							currentTimeOfDay = timeOfDay;
						}
						SliderInt("Time Of Day", &timeOfDay, 1.000f, 24.000f, "%.01f");

						if (Button("Teleport to Spawn Island"))
						{
							UFunctions::TeleportToSpawn();
						}

						SameLine(0.0f, 20.0f);
						if (Button("Teleport to Main Island"))
						{
							UFunctions::TeleportToMain();
						}

						static float X = 1.0f;
						static float Y = 1.0f;
						static float Z = 1.0f;

						NewLine();

						BeginChild("BugItGo");
						PushItemWidth(60);
						InputFloat("X", &X);

						SameLine(0.0f, 10.0f);

						PushItemWidth(60);
						InputFloat("Y", &Y);

						SameLine(0.0f, 15.0f);

						PushItemWidth(60);
						InputFloat("Z", &Z);

						SameLine(0.0f, 18.0f);

						if (Button("Teleport"))
						{
							UFunctions::TeleportToCoords(X, Y, Z);
						}
						EndChild();

						EndTabItem();
					}

					if (BeginTabItem("Character"))
					{
						if (Button("Fly"))
						{
							PlayerPawn->Fly(bIsFlying);
							bIsFlying = !bIsFlying;
						}

						SameLine(0.0f, 20.0f);

						if (Button("Skydive"))
						{
							//??
							PlayerPawn->StartSkydiving(0);
							PlayerPawn->StartSkydiving(0);
							PlayerPawn->StartSkydiving(0);
							PlayerPawn->StartSkydiving(0);
							PlayerPawn->StartSkydiving(2000.0f);
						}

						SameLine(0.0f, 20.0f);

						if (Button("Respawn"))
						{
							Respawn();
						}

						NewLine();

						static int currentHealth = 100;
						static int currentShield = 100;
						static int currentMaxHealth = 100;
						static int currentMaxShield = 100;
						static float currentSpeed = 1;
						static int currentGravityScale = 1;
						static int currentFov = 80;
						static int health = 100;
						static int shield = 100;
						static int maxHealth = 100;
						static int maxShield = 100;
						static float speed = 1;
						static int gravityScale = 1;
						static int fov = 80;

						if (currentFov != fov)
						{
							std::wstring command(L"fov " + std::to_wstring(fov));
							PlayerPawn->ExecuteConsoleCommand(command.c_str());
							currentFov = fov;
						}
						SliderInt("FOV", &fov, 20, 200, "%.03f");

						NewLine();

						if (currentGravityScale != gravityScale)
						{
							PlayerPawn->SetPawnGravityScale(gravityScale);
							currentGravityScale = gravityScale;
						}
						SliderInt("Gravity Scale", &gravityScale, -5.001f, 5.000f, "%.01f");

						NewLine();

						if (currentSpeed != speed)
						{
							PlayerPawn->SetMovementSpeed(speed);
							currentSpeed = speed;
						}
						SliderFloat("Speed Multiplier", &speed, 1.0f, 5.0f, "%.1f", 10.000001f);

						NewLine();

						if (currentHealth != health)
						{
							PlayerPawn->SetHealth(health);
							currentHealth = health;
						}
						SliderInt("Health Percent", &health, 1, 100, "%.3f");

						NewLine();

						if (currentShield != shield)
						{
							PlayerPawn->SetShield(shield);
							currentShield = shield;
						}
						SliderInt("Shield Percent", &shield, 1, 100, "%.3f");

						NewLine();

						if (currentMaxHealth != maxHealth)
						{
							PlayerPawn->SetMaxHealth(maxHealth);
							currentMaxHealth = maxHealth;
						}
						SliderInt("Max Health", &maxHealth, 1, 10000000, "%.3f");

						NewLine();

						if (currentMaxShield != maxShield)
						{
							PlayerPawn->SetMaxShield(maxShield);
							currentMaxShield = maxShield;
						}
						SliderInt("Max Shield", &maxShield, 1, 10000000, "%.3f");


						EndTabItem();
					}


					static char weapon[1024];

					if (BeginTabItem("Modifiers"))
					{
						InputTextWithHint("Weapon ID", "WID/AGID", weapon, sizeof(weapon));

						if (Button("Equip Weapon"))
						{
							std::string weaponS = weapon;
							std::wstring weaponW(weaponS.begin(), weaponS.end());
							WeaponQueue = weaponW;
						}

						EndTabItem();
					}
				}

				if (BeginTabItem("Credits"))
				{
					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text(XOR("Kemo (@xkem0x): Developer and mantainer of Neonite++"));

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text(XOR("Sammy (@madSammy): Frontend, Internal, General."));

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text(XOR("Taj (@AthenaBigBoi): Internal, General."));

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("Nyamimi (@nyameows): Internal, General.");

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("AsrielD (@Asriel_Dev): SSL-Bypass, General.");

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("Irma (@omairma): Frontend, Internal.");

					EndTabItem();
				}
			}
			End();
		}
	}
	ImGui::Render();


	pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}
