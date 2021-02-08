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
	if (uMsg == WM_KEYUP && (wParam == 0x2D /*INSERT*/ || wParam == VK_F5 || (showMenu && wParam == VK_ESCAPE)))
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
				if (NeoPlayer.Pawn)
				{
					if (BeginTabItem("World"))
					{

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
							NeoPlayer.Fly(bIsFlying);
							bIsFlying = !bIsFlying;
						}

						SameLine(0.0f, 20.0f);

						if (Button("Skydive"))
						{
							//??
							NeoPlayer.StartSkydiving(0);
							NeoPlayer.StartSkydiving(0);
							NeoPlayer.StartSkydiving(0);
							NeoPlayer.StartSkydiving(0);
							NeoPlayer.StartSkydiving(2000.0f);
						}

						SameLine(0.0f, 20.0f);

						if (Button("Respawn"))
						{
							NeoPlayer.Respawn();
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
							NeoPlayer.ExecuteConsoleCommand(command.c_str());
							currentFov = fov;
						}
						SliderInt("FOV", &fov, 20, 200, "%.03f");

						NewLine();

						if (currentGravityScale != gravityScale)
						{
							NeoPlayer.SetPawnGravityScale(gravityScale);
							currentGravityScale = gravityScale;
						}
						SliderInt("Gravity Scale", &gravityScale, -5, 5, "%.01f");

						NewLine();

						if (currentSpeed != speed)
						{
							NeoPlayer.SetMovementSpeed(speed);
							currentSpeed = speed;
						}
						SliderFloat("Speed Multiplier", &speed, 1.0f, 5.0f, "%.1f", 10.000001f);

						NewLine();

						if (currentHealth != health)
						{
							NeoPlayer.SetHealth(health);
							currentHealth = health;
						}
						SliderInt("Health Percent", &health, 1, 100, "%.3f");

						NewLine();

						if (currentShield != shield)
						{
							NeoPlayer.SetShield(shield);
							currentShield = shield;
						}
						SliderInt("Shield Percent", &shield, 1, 100, "%.3f");

						NewLine();

						if (currentMaxHealth != maxHealth)
						{
							NeoPlayer.SetMaxHealth(maxHealth);
							currentMaxHealth = maxHealth;
						}
						SliderInt("Max Health", &maxHealth, 1, 10000000, "%.3f");

						NewLine();

						if (currentMaxShield != maxShield)
						{
							NeoPlayer.SetMaxShield(maxShield);
							currentMaxShield = maxShield;
						}
						SliderInt("Max Shield", &maxShield, 1, 10000000, "%.3f");


						EndTabItem();
					}


					static char weapon[128];

					if (BeginTabItem("Modifiers"))
					{
						InputTextWithHint("Weapon ID", "WID/AGID", weapon, sizeof(weapon));

						if (Button("Equip Weapon"))
						{
							std::string weaponS = weapon;
							std::wstring weaponW(weaponS.begin(), weaponS.end());
							NeoPlayer.EquipWeapon(weaponW.c_str());
						}

						EndTabItem();
					}

					if (BeginTabItem("Spawn"))
					{
						const char* blueprints[] = {"HUSKPAWN_C", "SMASHERPAWN_C", "DUDEBRO_Pawn_C", "SHIELDERPAWN_C", "BGA_FireExtinguisher_Pickup_C", "HuskPreset"};
						const char* items[] = {"Husk", "Smasher", "Storm King", "Shielder", "Fire Extinguisher", "4 Husks Preset"};
						static int item_current = 0;
						static int currentitem_current = 0;
						Combo("Blueprints", &item_current, items, IM_ARRAYSIZE(items));

						if (currentitem_current != item_current)
						{
							std::string StringOfBlueprint(blueprints[item_current]);
							const std::wstring Blueprint(StringOfBlueprint.begin(), StringOfBlueprint.end());
							if (Blueprint.find(L"HuskPreset") != std::string::npos)
							{
								for (auto i = 0; i < 4; i++)
								{
									NeoPlayer.Summon(L"HUSKPAWN_c");
									const auto currentLocation = NeoPlayer.GetLocation();
									UFunctions::TeleportToCoords(currentLocation.X, currentLocation.Y + (20 * i), currentLocation.Z);
								}
							}
							else
							{
								NeoPlayer.Summon(Blueprint.c_str());
							}

							currentitem_current = item_current;
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
