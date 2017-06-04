// DiaryEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "Roboto-Medium-TTF-Font.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

// Splash window
bool splashWindowOpen = true;

bool entityListWindowOpen = true;
bool entityPropertiesWindowOpen = true;
bool workspaceObjectsWindowOpen = true;
bool mainViewOpen = true;

bool showImGuiTestWindow = false;

bool showConsole = true;
bool loadedLayout = false;

DiaryWorkspace * Workspace = NULL;

std::string workspaceToLoad = "";

static sf::Texture *s_customFontTexture = NULL; // used for the custom font
unsigned char* fontpixels;
int fntwidth, fntheight;
bool isEmptyWorkspace = true;

const char * Workspace_GetFloorName(int index)
{
	if (Workspace)
	{
		return Workspace->GetFloorName(index);
	}
	else {
		return 0;
	}
}

void Workspace_AddFloor(const char * sname, const char * name)
{
	Workspace->AddFloor(sname, name);
}

int main(int argc, char * argv[])
{
	Print("Initializing DiaryEditor...");
	for (int i = 0; i < argc; i++)
	{
		//Print("Arg %d: %s", i, argv[i]);
	}
	if (argc >= 2)
	{
		Print("Loading %s...", argv[1]);
		workspaceToLoad = argv[1];
	}
	sf::RenderWindow window(sf::VideoMode(800, 600), "DiaryEditor", sf::Style::Default);

	ImFont *f = ImGui::GetIO().Fonts->AddFontFromFileTTF("segoeui.ttf", 16.0f);//AddFontFromMemoryTTF((void *)roboto_data, roboto_size, 12.0f);
	ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&fontpixels, &fntwidth, &fntheight);
	s_customFontTexture = new sf::Texture;
	s_customFontTexture->create(fntwidth, fntheight);
	s_customFontTexture->update(fontpixels);

	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window, s_customFontTexture);
	window.setTitle("DiaryEditor 1.0.0");
	sf::Clock deltaClock;

	ImGuiStyle * style2 = &ImGui::GetStyle();

	style2->WindowPadding = ImVec2(15, 15);
	style2->WindowRounding = 5.0f;
	style2->FramePadding = ImVec2(5, 5);
	style2->FrameRounding = 4.0f;
	style2->ItemSpacing = ImVec2(12, 8);
	style2->ItemInnerSpacing = ImVec2(8, 6);
	style2->IndentSpacing = 25.0f;
	style2->ScrollbarSize = 15.0f;
	style2->ScrollbarRounding = 9.0f;
	style2->GrabMinSize = 5.0f;
	style2->GrabRounding = 3.0f;

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.90f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 0.90f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.28f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.18f, 0.18f, 0.23f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.44f, 0.15f, 0.42f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.64f, 0.07f, 0.94f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.53f, 0.32f, 0.04f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.95f, 0.50f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.50f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.50f, 0.00f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.00f, 0.00f, 0.00f, 0.80f);




	if (workspaceToLoad.length() != 0)
	{
		FILE * f;
		fopen_s(&f, workspaceToLoad.c_str(), "rb");
		if (f)
		{
			fclose(f);
			Workspace = new DiaryWorkspace();
			Workspace->Load(workspaceToLoad);
			isEmptyWorkspace = false;
		}
		else {
			Print("ERROR: Failed to open file!");
			Print("WARNING: Test");
		}
	}
	if (!Workspace)
		Workspace = new DiaryWorkspace();
	if (!loadedLayout)
	{
		FILE * l;
		fopen_s(&l, LayoutFile, "rb");
		if (l)
		{
			// disabled until we can find a way to fix this
			ImGui::LoadDock(l);
			fclose(l);
		}
		else
		{
			Print("WARNING: %s not found, will create the file on exit", LayoutFile);
		}
		loadedLayout = true;
	}
	ImGui::RootDock(ImVec2(0, 0), window.getSize());
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				if (loadedLayout)
				{
					FILE * l;
					fopen_s(&l, LayoutFile, "wb");
					if (l)
					{
						ImGui::SaveDock(l);
						fclose(l);
					}
					else
					{
						Print("ERROR: Couldn't save layout!");
					}
				}
				window.close();
			}

			if (event.type == sf::Event::Resized)
			{
				ImGuiIO& io = ImGui::GetIO();
				io.DisplaySize = window.getSize();
				ImGui::RootDock(ImVec2(0, 0), window.getSize());
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		if (splashWindowOpen)
		{
			ImGui::Begin("Welcome!", &splashWindowOpen, ImGuiWindowFlags_NoResize); // begin window

			ImGui::Text("Choose an action");
			if (ImGui::Button("New..."))
			{
				if (!isEmptyWorkspace)
				{
					Print("WARNING: Workspace is not empty! Saving...");
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Open..."))
			{

			}
			ImGui::End(); // end window
		}


		if (showConsole)
		{
			ShowConsole(&showConsole);
		}

		if (showImGuiTestWindow) ImGui::ShowTestWindow(&showImGuiTestWindow);

		ImVec2 d(200.0f, 200.0f);


		ImGui::BeginDock("Floors");
		ImGui::BeginGroup();
		ImGui::BeginChild("floor view", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
		for (int i = 0; i < Workspace->GetFloorCount(); i++)
		{
			if (Workspace->isValidFloor(i))
			{
				if (ImGui::Selectable(Workspace_GetFloorName(i), Workspace->currentFloor == i))
				{
					Workspace->currentFloor = i;
				}
			}
		}
		ImGui::EndChild();
		ImGui::BeginChild("fbuttons");
		if (ImGui::Button("Add Floor..."))
		{
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
		}
		ImGui::EndChild();
		ImGui::EndGroup();
		ImGui::EndDock();


		ImGui::BeginDock("Levels");
		ImGui::BeginGroup();

		ImGui::BeginChild("level view", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
		if (Workspace->isValidFloor(Workspace->currentFloor))
		{
			// TODO: List levels here
		}
		ImGui::EndChild();

		ImGui::BeginChild("lbuttons");
		if (ImGui::Button("Add Level..."))
		{
		} 
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
		}
		ImGui::EndChild();

		ImGui::EndGroup();
		ImGui::EndDock();


		ImGui::BeginDock("Entity List", &entityListWindowOpen, (ImGuiWindowFlags)0, d);
		ImGui::EndDock();


		ImGui::BeginDock("Property Editor", &entityPropertiesWindowOpen, (ImGuiWindowFlags)0, d);
		ImGui::EndDock();


		ImGui::BeginDock("Main View", NULL, ImGuiWindowFlags_MenuBar, d);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New")) {}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {}
				if (ImGui::BeginMenu("Open Recent"))
				{
					ImGui::MenuItem("TODO");
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {}
				if (ImGui::MenuItem("Save As..")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Options"))
				{

				}

				if (ImGui::MenuItem("Quit", "Alt+F4")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				/*
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				*/

				if (ImGui::MenuItem("Add new floor...")) {}
				if (ImGui::MenuItem("Add new level to floor...")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window"))
			{
				//ImGui::Checkbox("", bool);
				ImGui::Checkbox("Entity List", &entityListWindowOpen);
				ImGui::Checkbox("Property Editor", &entityPropertiesWindowOpen);
				ImGui::Checkbox("Workspace Objects", &workspaceObjectsWindowOpen);
				//ImGui::Checkbox("Main View", &mainViewOpen);
				ImGui::Checkbox("Console", &showConsole);
				ImGui::Separator();
				ImGui::Checkbox("Show Start Window", &splashWindowOpen);
#ifdef _DEBUG
				ImGui::Separator();
				ImGui::Checkbox("Show ImGui Test Window", &showImGuiTestWindow);
#endif
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::EndDock();
		ImGui::BeginDock("Workspace Objects", &workspaceObjectsWindowOpen, (ImGuiWindowFlags)0, d);
		ImGui::EndDock();
		window.clear();
		ImGui::Render();
		window.display();
	}

	/*
	ImGuiStyle& style = ImGui::GetStyle();
style.Colors[ImGuiCol_Text]                  = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.06f, 0.05f, 0.07f, 0.90f);
style.Colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.07f, 0.07f, 0.09f, 0.90f);
style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
style.Colors[ImGuiCol_Border]                = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.92f, 0.91f, 1.00f, 0.00f);
style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.56f, 0.28f, 0.09f, 1.00f);
style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.18f, 0.18f, 0.23f, 1.00f);
style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.80f, 0.44f, 0.15f, 0.42f);
style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(1.00f, 0.64f, 0.07f, 0.94f);
style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
style.Colors[ImGuiCol_ComboBg]               = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
style.Colors[ImGuiCol_Button]                = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.24f, 0.23f, 0.27f, 1.00f);
style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.56f, 0.56f, 0.69f, 1.00f);
style.Colors[ImGuiCol_Header]                = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.53f, 0.32f, 0.04f, 1.00f);
style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
style.Colors[ImGuiCol_Column]                = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
style.Colors[ImGuiCol_ColumnHovered]         = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
style.Colors[ImGuiCol_ColumnActive]          = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
style.Colors[ImGuiCol_CloseButton]           = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
style.Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
style.Colors[ImGuiCol_CloseButtonActive]     = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.95f, 0.50f, 0.00f, 1.00f);
style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.50f, 0.00f, 1.00f);
style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(1.00f, 0.50f, 0.00f, 0.43f);
style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);


	*/

	ImGui::SFML::Shutdown();

	return 0;
}

