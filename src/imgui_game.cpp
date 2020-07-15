#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

#include "main.h"
#include "input.h"
#include "render.h"

static bool gImGui_Hide = false;
static bool gImGui_ShowDemoWindow = false;
static bool gImGui_ShowMetrics = false;

void ImGuiGame_Options(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(200, 100));
	if (!ImGui::Begin("Options", p_open)) {
		ImGui::End();
		return;
	}

	ImGui::Checkbox("Metrics", &gImGui_ShowMetrics);
	ImGui::Checkbox("ImGui Demo", &gImGui_ShowDemoWindow);

	static int vsync_type = Render_GetVSync();
	ImGui::Combo("VSync", &vsync_type, "Off\0On\0Smart\0");
	Render_SetVSync(vsync_type);

	ImGui::End();
}

void ImGuiGame_Metrics()
{
	//ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background

	ImGuiWindowFlags overlayFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
									ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | 
									ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	if (ImGui::Begin("Metrics Overlay", &gImGui_ShowMetrics, overlayFlags)) {
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("%.3f ms (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	}

	ImGui::End();
}

void ImGui_Loop()
{
	static bool show_options = false;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(GetWindow());
	ImGui::NewFrame();

	if (ImGui::IsKeyPressed(SDL_SCANCODE_F1)) {
		gImGui_Hide = !gImGui_Hide;
	}
	if (gImGui_Hide) {
		goto render;
	}

	if (show_options)			{ ImGuiGame_Options(&show_options); }
	if (gImGui_ShowDemoWindow)	{ ImGui::ShowDemoWindow(&gImGui_ShowDemoWindow); }

	if (ImGui::BeginMainMenuBar()) {
		bool quitGame = false;
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("Options", nullptr, &show_options);
			ImGui::Separator();
			ImGui::MenuItem("Exit", "Esc", &quitGame);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

		Game_SetQuitStatus(quitGame);
	}

render:
	if (gImGui_ShowMetrics) { ImGuiGame_Metrics(); }

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
