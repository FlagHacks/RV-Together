#include "Drawing.h"
#include <fstream>
#include <string>
#include <windows.h> 

LPCSTR Drawing::lpWindowName = "RV There Yet? | Multiplayer Mod";
ImVec2 Drawing::vWindowSize = { 750, 440 };
ImGuiWindowFlags Drawing::WindowFlags =
ImGuiWindowFlags_NoCollapse |
ImGuiWindowFlags_NoResize |
ImGuiWindowFlags_NoScrollbar |
ImGuiWindowFlags_NoScrollWithMouse |
ImGuiWindowFlags_NoSavedSettings |
ImGuiWindowFlags_NoNav;
bool Drawing::bDraw = true;
int PlayerCount = 4;

void Drawing::Active()
{
	bDraw = true;
}

bool Drawing::isActive()
{
	return bDraw == true;
}

void Drawing::Draw()
{
	if (isActive())
	{
		ImGui::SetNextWindowSize(vWindowSize, ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::Begin(lpWindowName, &bDraw, WindowFlags);
		{
            ImGui::Spacing();

            ImGui::Text("Information");

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Text("- This Mod allows you to play with up to 24 Players!");
            ImGui::Text("- This Mod overwrites the MaxPlayers value");
            ImGui::Text("- Game has to be closed to Apply Settings!");
            ImGui::Text("- Only the Host has to have the Mod installed");
            ImGui::Text("- The Amount of seats in the RV does not change");
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
			
            ImGui::SliderInt(" Player Count", &PlayerCount, 1, 24);
			if (ImGui::Button("Apply Player Count"))
			{
                char* appData = nullptr;
                size_t len = 0;
                _dupenv_s(&appData, &len, "LOCALAPPDATA");
                if (appData)
                {
                    std::string path = std::string(appData) + "\\Ride\\Saved\\Config\\Windows\\Game.ini";

                    CreateDirectoryA((std::string(appData) + "\\Ride").c_str(), NULL);
                    CreateDirectoryA((std::string(appData) + "\\Ride\\Saved").c_str(), NULL);
                    CreateDirectoryA((std::string(appData) + "\\Ride\\Saved\\Config").c_str(), NULL);
                    CreateDirectoryA((std::string(appData) + "\\Ride\\Saved\\Config\\Windows").c_str(), NULL);

                    std::ofstream file(path);
                    if (file.is_open())
                    {
                        MessageBoxA(NULL, "Successfully overwrote Data!", "Success", MB_OK | MB_ICONINFORMATION);
                        file << "[/Script/Engine.GameSession]\n";
                        file << "MaxPlayers=" << PlayerCount << "\n";
                        file.close();
                    }
                    else
                    {
                        MessageBoxA(NULL, "Failed to overwrite Data", "Error", MB_ICONERROR);
                    }
                }
			}

            if (ImGui::Button("Revert Changes"))
            {
                char* appData = nullptr;
                size_t len = 0;
                _dupenv_s(&appData, &len, "LOCALAPPDATA");

                if (appData)
                {
                    std::string path = std::string(appData) + "\\Ride\\Saved\\Config\\Windows\\Game.ini";

                    if (DeleteFileA(path.c_str()))
                    {
                        MessageBoxA(NULL, "Successfully reverted", "Success", MB_OK | MB_ICONINFORMATION);
                    }
                    else
                    {
                        MessageBoxA(NULL, "Failed to revert changes", "Error", MB_OK | MB_ICONERROR);
                    }

                    free(appData);
                }
            }
		}

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Exit"))
        {
            PostQuitMessage(0);
        }
		ImGui::End();
	}

	#ifdef _WINDLL
	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDraw = !bDraw;
	#endif
}
