#include "../framework.h"
#include "../mods.h"
#include "../console.h"
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
using namespace NeoRoyale;

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

static bool bIsHead;
FileBrowser fileDialog;

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
			fileDialog.SetTypeFilters({".png"});
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

			if (BeginTabBar(XOR("Neonite")), ImGuiTabBarFlags_AutoSelectNewTabs)
			{
				if (NeoPlayer.Pawn)
				{
					if (BeginTabItem("World"))
					{
						static int timeOfDay = 1;
						static int currentTimeOfDay = 1;

						if (currentTimeOfDay != timeOfDay)
						{
							UFunctions::SetTimeOfDay(timeOfDay);
							currentTimeOfDay = timeOfDay;
						}
						SliderInt(XOR("Time Of Day"), &timeOfDay, 1.000f, 24.000f, "%.01f");

						NewLine();

						if (Button(XOR("Teleport to Spawn Island")))
						{
							NeoPlayer.TeleportToSpawn();
						}

						SameLine(0.0f, 20.0f);
						if (Button(XOR("Teleport to Main Island")))
						{
							NeoPlayer.TeleportTo(FVector());
						}

						static float X = 1.0f;
						static float Y = 1.0f;
						static float Z = 1.0f;


						BeginChild(XOR("BugItGo"));
						PushItemWidth(60);
						InputFloat("X", &X);

						SameLine(0.0f, 10.0f);

						PushItemWidth(60);
						InputFloat("Y", &Y);

						SameLine(0.0f, 15.0f);

						PushItemWidth(60);
						InputFloat("Z", &Z);

						SameLine(0.0f, 18.0f);

						if (Button(XOR("Teleport")))
						{
							NeoPlayer.TeleportTo(FVector(X, Y, Z));
						}

						NewLine();

						if (Button("Fill level with water"))
						{
							//NeoPlayer.Summon(XOR(L"Apollo_Waterbody_Ocean_Parent_C"));
						}

						SameLine();

						if (Button("Remove water"))
						{
							Console::ExecuteConsoleCommand(XOR(L"destroyall Apollo_Waterbody_Ocean_Parent_C"));
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

						if (Button("Pick Custom Body Texture"))
						{
							fileDialog.Open();
						}

						SameLine();

						Checkbox(XOR("Head"), &bIsHead);

						NewLine();

						if (Button(XOR("First Person Camera")))
						{
							NeoPlayer.SetCameraMode(L"FirstPerson");
							NeoPlayer.HideHead(true);
						}

						//Commented becuase i don't know the custom thirdperson cameara mode name ;-;
						/*SameLine();

						if (Button(XOR("Third Person Camera")))
						{
							NeoPlayer.SetCameraMode(L"FortCameraMode_ThirdPerson");
							NeoPlayer.HideHead(false);
						}*/

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
							Console::ExecuteConsoleCommand(command.c_str());
							currentFov = fov;
						}
						SliderInt(XOR("FOV"), &fov, 20, 200, "%.03f");

						NewLine();

						if (currentGravityScale != gravityScale)
						{
							NeoPlayer.SetPawnGravityScale(gravityScale);
							currentGravityScale = gravityScale;
						}
						SliderInt(XOR("Gravity Scale"), &gravityScale, -5, 5, "%.01f");

						NewLine();

						if (currentSpeed != speed)
						{
							NeoPlayer.SetMovementSpeed(speed);
							currentSpeed = speed;
						}
						SliderFloat(XOR("Speed Multiplier"), &speed, 1.0f, 5.0f, "%.1f", 10.000001f);

						NewLine();

						if (currentHealth != health)
						{
							NeoPlayer.SetHealth(health);
							currentHealth = health;
						}
						SliderInt(XOR("Health Percent"), &health, 1, 100, "%.3f");

						NewLine();

						if (currentShield != shield)
						{
							NeoPlayer.SetShield(shield);
							currentShield = shield;
						}
						SliderInt(XOR("Shield Percent"), &shield, 1, 100, "%.3f");

						NewLine();

						if (currentMaxHealth != maxHealth)
						{
							NeoPlayer.SetMaxHealth(maxHealth);
							currentMaxHealth = maxHealth;
						}
						SliderInt(XOR("Max Health"), &maxHealth, 1, 10000000, "%.3f");

						NewLine();

						if (currentMaxShield != maxShield)
						{
							NeoPlayer.SetMaxShield(maxShield);
							currentMaxShield = maxShield;
						}
						SliderInt(XOR("Max Shield"), &maxShield, 1, 10000000, "%.3f");


						EndTabItem();
					}

					if (BeginTabItem("Helpers"))
					{
						static std::wstring currentItem;
						static std::wstring currentBlueprint;
						static std::wstring currentMesh;

						Text(XOR("Any item selected from the combos below will be copied to your clipboard, everything is generated at runtime."));

						NewLine();

						if (BeginCombo("Weapons", std::string(currentItem.begin(), currentItem.end()).c_str()))
						{
							for (auto n = 0; n < gWeapons.size(); n++)
							{
								const bool is_selected = (currentItem == gWeapons[n]);
								const std::string wid(gWeapons[n].begin(), gWeapons[n].end());
								if (Selectable(wid.c_str(), is_selected))
								{
									currentItem = gWeapons[n];
									Util::CopyToClipboard(wid);
								}
								if (is_selected)
								{
									SetItemDefaultFocus();
								}
							}
							EndCombo();
						}

						if (BeginCombo("Blueprints", std::string(currentBlueprint.begin(), currentBlueprint.end()).c_str()))
						{
							for (auto n = 0; n < gBlueprints.size(); n++)
							{
								const bool is_selected = (currentBlueprint == gBlueprints[n]);
								const std::string blueprint(gBlueprints[n].begin(), gBlueprints[n].end());
								if (Selectable(blueprint.c_str(), is_selected))
								{
									currentBlueprint = gBlueprints[n];
									Util::CopyToClipboard(blueprint);
								}
								if (is_selected)
								{
									SetItemDefaultFocus();
								}
							}
							EndCombo();
						}


						if (BeginCombo("Mesh", std::string(currentMesh.begin(), currentMesh.end()).c_str()))
						{
							for (auto n = 0; n < gMeshes.size(); n++)
							{
								const bool is_selected = (currentMesh == gMeshes[n]);
								const std::string mesh(gMeshes[n].begin(), gMeshes[n].end());
								if (Selectable(mesh.c_str(), is_selected))
								{
									currentMesh = gMeshes[n];
									NeoPlayer.SetSkeletalMesh(gMeshes[n].c_str());
								}
								if (is_selected)
								{
									SetItemDefaultFocus();
								}
							}
							EndCombo();
						}

						NewLine();

						Text(XOR("DICLAIMER: Changing Skeletal Mesh may cause crashes."));

						NewLine();


						static char command[1024];
						InputText(XOR("Command"), command, sizeof command);

						SameLine();

						if (Button("Execute"))
						{
							std::string commandS(command);
							const std::wstring coammndW(commandS.begin(), commandS.end());
							Console::ExecuteConsoleCommand(coammndW.c_str());
						}

						NewLine();

						if (Button("Test"))
						{
							UFunctions::Play(YOUGURT_EVENT_PLAYER);
						}

						if (Button("Bots Grenade"))
						{
							Util::CopyToClipboard("WID_Athena_FrenchYedoc_JWFriendly");
						}

						EndTabItem();
					}
				}

				if (!NeoPlayer.Pawn)
				{
					if (BeginTabItem(XOR("Override Skin")))
					{
						SetCursorPosY(GetCursorPosY() + 5);

						Text(XOR("NOTE: Doesn't work after jumping from bus\n(Recommendation: use battlelab)"));

						const char* overrides[] = {"None", "Thanos", "Chituari"};
						static int Item = 0;
						static int currentItem = 0;
						Combo("Skin", &Item, overrides, IM_ARRAYSIZE(overrides));

						if (currentItem != Item)
						{
							std::string OverrideName(overrides[Item]);
							NeoPlayer.SkinOverride = std::wstring(OverrideName.begin(), OverrideName.end());
							currentItem = Item;
						}

						NewLine();

						if (Button("Pick Custom Body Texture"))
						{
							fileDialog.Open();
						}

						SameLine();

						Checkbox(XOR("Head"), &bIsHead);

						EndTabItem();
					}
				}

				if (BeginTabItem("Help"))
				{
					SetCursorPosY(GetCursorPosY() + 5);

					Text(
						R"(
Commands
---------------------------
cheatscript event - Triggers the event for your version (e.g. Junior, Jerky, Flatter).
cheatscript debugcamera - Toggles a custom version of the debug camera.
cheatscript skydive | skydiving - Puts you in a skydive with deploy at 1500m above the ground.
cheatscript equip <WID | AGID> - Equips a weapon / pickaxe. (You can get them from the modifers tap)
cheatscript setgravity <NewGravityScaleFloat> - Changes the gravity scale.
cheatscript speed | setspeed <NewCharacterSpeedMultiplier> - Changes the movement speed multiplier.
cheatscript setplaylist <Playlist> - Overrides the current playlist.
cheatscript respawn - Respawns the player (duh)
cheatscript sethealth <NewHealthFloat> - Changes your health value.
cheatscript setshield <NewShieldFloat> - Changes your shield value.
cheatscript setmaxhealth <NewMaxHealthFloat> - Changes your max health value.
cheatscript setmaxshield <newMaxShieldFloat> - Changes your max shield value.
cheatscript dump - Dumps a list of all GObjects. (output at win64 folder)
cheatscript dumpbps - Dumps a list all blueprints. (output at win64 folder)
fly - Toggles flying.
enablecheats - Enables cheatmanager.
summon <BlueprintClass> - Summons a blueprint class. (You can get them from the helpers tap)
---------------------------
F3 - Back to lobby.
` (backquote key) - Open UE4 console.
<> - Argument (e.g: <NewHealthFloat> is replaced with 1.0).
| - Or.
)");


					EndTabItem();
				}


				if (BeginTabItem(XOR("About")))
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

					Text(XOR("Nyamimi (@nyameows): Internal, General."));

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text(XOR("Irma (@omairma): Frontend, Internal."));

					EndTabItem();
				}

				if (ProdMode)
				{
					if (BeginTabItem(XOR("Prod")))
					{
						if (Button("test"))
						{
							UE4::DumpGObjects();
						}

						if (Button("Hack"))
						{

						}

						EndTabItem();
					}
				}
			}
			End();

			fileDialog.Display();

			if (fileDialog.HasSelected())
			{
				UFunctions::SetBodyCustomTextureFromPng(fileDialog.GetSelected().wstring().c_str(), bIsHead);
				fileDialog.ClearSelected();
				fileDialog.Close();
			}
		}
	}

	Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}
