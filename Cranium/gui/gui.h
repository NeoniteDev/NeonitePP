#include "../framework.h"
#include "ImGui/imgui.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

using namespace ImGui;
using namespace Neoroyale;

VOID RenderGUI() {
	if (showMenu) {
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(1.00f, 0.00f, 0.00f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.41f, 0.00f, 0.03f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.48f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.46f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.46f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.75f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.46f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.46f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.46f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.46f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.46f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);

		ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5, 0.5);
		ImGui::GetStyle().FramePadding = ImVec2(4.0, 2.0);
		ImGui::GetStyle().WindowRounding = 0;
		ImGui::GetStyle().GrabMinSize = 12;
		ImGui::GetStyle().ScrollbarSize = 14;
		ImGui::GetStyle().ScrollbarRounding = 0;
		ImGui::GetStyle().FrameRounding = 4.0f;


		ImGui::Begin("Neonite++", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize); {
			ImGui::SetWindowSize(ImVec2(550, 450), ImGuiCond_FirstUseEver);

			ImVec2 size = ImGui::GetItemRectSize();

		}
		ImGui::End();
	}

	ImGui::PopStyleColor();

	ImGui::Render();
}


void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	if (uMsg == WM_KEYUP && (wParam == 0x2D /*INSERT*/ || (showMenu && wParam == VK_ESCAPE))) {
		showMenu = !showMenu;
		ImGui::GetIO().MouseDrawCursor = showMenu;

	}
	else if (uMsg == WM_QUIT && showMenu) {
		ExitProcess(0);
	}

	if (showMenu) {
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
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			initgui = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	/*if (showMenu) {
		ImGui::Begin("I love gay sex");
		ImGuiWindowFlags window_flags = 0;

		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoSavedSettings;
		window_flags |= ImGuiWindowFlags_NoScrollbar;
		window_flags |= ImGuiWindowFlags_NoScrollWithMouse;

		SetNextWindowPos(ImVec2(500, 100), ImGuiCond_FirstUseEver);
		SetNextWindowSize(ImVec2(535, 320), ImGuiCond_FirstUseEver);

		ImGui::PushItemWidth(GetFontSize() * -12);

		if (BeginTabBar("Neonite"), ImGuiTabBarFlags_AutoSelectNewTabs)
		{
			if (BeginTabItem("World"))
			{
				EndTabItem();
			}


			if (BeginTabItem("Character"))
			{


				EndTabItem();
			}

			if (BeginTabItem("Cheats"))
			{


				EndTabItem();
			}

			if (BeginTabItem("Credits"))
			{
				SetCursorPosX(GetCursorPosX() + 50);
				SetCursorPosY(GetCursorPosY() + 5);

				Text(XOR("Kemo (@xkem0x): Developer and mantainer of Neonite++"));

				SetCursorPosX(GetCursorPosX() + 50);
				SetCursorPosY(GetCursorPosY() + 5);

				Text(XOR("Sammy (@madSammy): Frontend, General."));

				SetCursorPosX(GetCursorPosX() + 50);
				SetCursorPosY(GetCursorPosY() + 5);

				Text(XOR("Taj (@AthenaBigBoi): Internal, General."));

				SetCursorPosX(GetCursorPosX() + 50);
				SetCursorPosY(GetCursorPosY() + 5);

				Text("Nyamimi (@nyameows): Internals, General.");

				SetCursorPosX(GetCursorPosX() + 50);
				SetCursorPosY(GetCursorPosY() + 5);

				Text("Makks (@MakksFN): ConsoleUnlocker.");

				SetCursorPosX(GetCursorPosX() + 50);
				SetCursorPosY(GetCursorPosY() + 5);

				Text("Syfe (@ItsSyfe): Github ReadME.");

				SetCursorPosX(GetCursorPosX() + 50);
				SetCursorPosY(GetCursorPosY() + 5);

				Text("AsrielD (@Asriel_Dev): SSL-Bypass, General.");

				SetCursorPosX(GetCursorPosX() + 50);
				SetCursorPosY(GetCursorPosY() + 5);

				Text("Beat YT (@Beat-YT): Backend.");

				SetCursorPosX(GetCursorPosX() + 50);
				SetCursorPosY(GetCursorPosY() + 5);

				Text("Irma (@omairma): Frontend, Internals.");

				EndTabItem();
			}
			ImGui::End();
		}
		ImGui::Render();
	}

	if (showMenu) {

		ImVec4* colors = ImGui::GetStyle().Colors;
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


		ImGui::Begin("Neonite++", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize); {
			ImGui::SetWindowSize(ImVec2(550, 450), ImGuiCond_FirstUseEver);

			ImVec2 size = ImGui::GetItemRectSize();
			if (BeginTabBar("Neonite"), ImGuiTabBarFlags_AutoSelectNewTabs)
			{
				if (BeginTabItem("World"))
				{
					EndTabItem();
				}


				if (BeginTabItem("Character"))
				{


					EndTabItem();
				}

				if (BeginTabItem("Cheats"))
				{


					EndTabItem();
				}

				if (BeginTabItem("Credits"))
				{
					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text(XOR("Kemo (@xkem0x): Developer and mantainer of Neonite++"));

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text(XOR("Sammy (@madSammy): Frontend, General."));

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text(XOR("Taj (@AthenaBigBoi): Internal, General."));

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("Nyamimi (@nyameows): Internals, General.");

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("Makks (@MakksFN): ConsoleUnlocker.");

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("Syfe (@ItsSyfe): Github ReadME.");

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("AsrielD (@Asriel_Dev): SSL-Bypass, General.");

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("Beat YT (@Beat-YT): Backend.");

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("Irma (@omairma): Frontend, Internals.");

					EndTabItem();
				}

			}
		}
		ImGui::End();
	}

	ImGui::PopStyleColor();

	ImGui::Render();*/

	ImVec4* colors = ImGui::GetStyle().Colors;
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


	if (showMenu) {

		if (ImGui::Begin(XOR("Neonite++"), NULL, ImGuiWindowFlags_NoCollapse))
		{

			PushItemWidth(GetFontSize() * -12);

			if (BeginTabBar("Neonite"), ImGuiTabBarFlags_AutoSelectNewTabs)
			{
				if (BeginTabItem("World"))
				{
					EndTabItem();
				}


				if (Neoroyale::PlayerPawn) {
					if (BeginTabItem("Character"))
					{

						if (ImGui::Button("Pawn Fly")) {
							PlayerPawn->Fly(bIsFlying);
							bIsFlying = !bIsFlying;
						}

						ImGui::SameLine(0.0f, 20.0f);

						if (ImGui::Button("Skydive")) {
							PlayerPawn->Skydive();
						}

						ImGui::SameLine(0.0f, 20.0f);

						if (ImGui::Button("Skydive")) {
							PlayerPawn->Skydive();
						}

						ImGui::NewLine();

						static int health = 1;
						static int shield = 1;
	
						if (ImGui::Button("Change Health")) {
							PlayerPawn->SetHealth(health);
						}
						ImGui::SameLine(0.0f, 20.0f);
						ImGui::SliderInt("Player Health", &health, 1, 100, "%.3f");

						ImGui::NewLine();

						if (ImGui::Button("Change Shield")) {
							PlayerPawn->SetShield(shield);
						}
						ImGui::SameLine(0.0f, 20.0f);
						ImGui::SliderInt("Player Shield", &shield, 1, 100, "%.3f");


						EndTabItem();
					}
				}

				if (BeginTabItem("Cheats"))
				{


					EndTabItem();
				}

				if (BeginTabItem("Credits"))
				{
					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text(XOR("Kemo (@xkem0x): Developer and mantainer of Neonite++"));

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text(XOR("Sammy (@madSammy): Frontend, General."));

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text(XOR("Taj (@AthenaBigBoi): Internal, General."));

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("Nyamimi (@nyameows): Internals, General.");

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("Makks (@MakksFN): ConsoleUnlocker.");

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("Syfe (@ItsSyfe): Github ReadME.");

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("AsrielD (@Asriel_Dev): SSL-Bypass, General.");

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("Beat YT (@Beat-YT): Backend.");

					SetCursorPosX(GetCursorPosX() + 50);
					SetCursorPosY(GetCursorPosY() + 5);

					Text("Irma (@omairma): Frontend, Internals.");

					EndTabItem();
				}
			}
			ImGui::End();
		}
	}
	ImGui::Render();



	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}