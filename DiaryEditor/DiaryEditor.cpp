// DiaryEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "Roboto-Medium-TTF-Font.h"
#include "DiaryDefaultTexture.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/OpenGL.hpp>

#pragma message("Compiling "DE_STR)

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

sf::Clock deltaClock;

static sf::Texture *s_customFontTexture = NULL; // used for the custom font
static sf::Texture *defaultTexture = NULL; // used for you know what
static sf::Font defaultFont; // used for overlay

sf::Shader shader;

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

inline void AddFloorDialog()
{
	if (ImGui::BeginPopupModal("Add a new floor", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		//ImGui::Text("");
		static char name[128];
		static char sname[128];
		ImGui::InputText("Name", name, 128);
		ImGui::InputText("Script Name", sname, 128);

		if (ImGui::Button("Add", ImVec2(120, 0)))
		{
			Workspace_AddFloor(sname, name);
			name[0] = '\0';
			sname[0] = '\0';
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			name[0] = '\0';
			sname[0] = '\0';
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

inline void DeleteFloorDialog()
{
	if (ImGui::BeginPopupModal("Delete the floor?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure you want to delete this floor?\nThis action cannot be undone!");

		if (ImGui::Button("Yes", ImVec2(120, 0)))
		{
			Workspace->RemoveFloorAtIndex(Workspace->currentFloor);
			Workspace->currentSelection = SelectionType::None;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

inline void AddLevelDialog()
{
	if (ImGui::BeginPopupModal("Add a new level", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		//ImGui::Text("");
		static char name[128];
		static char sname[128];
		ImGui::InputText("Name", name, 128);
		ImGui::InputText("Script Name", sname, 128);

		if (ImGui::Button("Add", ImVec2(120, 0)))
		{
			Workspace->AddLevel(sname, name);
			name[0] = '\0';
			sname[0] = '\0';
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			name[0] = '\0';
			sname[0] = '\0';
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

inline void DeleteLevelDialog()
{
	if (ImGui::BeginPopupModal("Delete the level?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure you want to delete this level?\nThis action cannot be undone!");
		//static char name[128];
		//static char sname[128];
		//ImGui::InputText("Name", name, 128);
		//ImGui::InputText("Script Name", sname, 128);

		if (ImGui::Button("Yes", ImVec2(120, 0)))
		{
			Workspace->RemoveLevelAtIndex(Workspace->currentLevel);
			Workspace->currentSelection = SelectionType::None;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

inline void AddEntDefDialog()
{
	if (ImGui::BeginPopupModal("Add a new entity definition", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		//ImGui::Text("");
		static char name[128];
		static char scriptSpawn[128];
		static char imgPath[128];
		static char userpropn[MaxUserPropertiesForEntity][128];
		static char userpropv[MaxUserPropertiesForEntity][128];
		static int userPropCount = 0;
		ImGui::InputText("Name", name, 128); ImGui::SameLine();
		ImGui::InputText("Script spawn function", scriptSpawn, 128);
		ImGui::InputText("Image Path", imgPath, 128);
		ImGui::InputInt("Property Count", &userPropCount, 1, 1);
		if (userPropCount < 0) userPropCount = 0;
		if (userPropCount > MaxUserPropertiesForEntity) userPropCount = MaxUserPropertiesForEntity;
		if (ImGui::CollapsingHeader("Properties"))
		{
			for (int i = 0; i < userPropCount; i++)
			{
				ImGui::PushID(i);
				if (1)
				{
					ImGui::InputText("Name", userpropn[i], 128); ImGui::SameLine();
					ImGui::InputText("Value", userpropv[i], 128);
				}
				ImGui::PopID();
			}
		}

		if (ImGui::Button("Add", ImVec2(120, 0)))
		{
			int rIndex = Workspace->AddEntityDefinition(name, scriptSpawn, imgPath, userPropCount);
			for (int i = 0; i < userPropCount; i++)
			{
				Workspace->entities[rIndex].userProperties[i].name = userpropn[i];
				Workspace->entities[rIndex].userProperties[i].value = userpropv[i];
			}
			name[0] = '\0';
			userPropCount = 0;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			name[0] = '\0';
			userPropCount = 0;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

static float ZoomLevel = 1.0f;
static float wscroll = 1.0f;
static sf::RenderTexture levelRender;
static sf::View levelView;

int rendererW;
int rendererH;

bool refreshRenderTextures = true;
ImVec2 windowSize;


void RenderMainView()
{
	windowSize = ImGui::GetWindowContentRegionMax();
	rendererW = windowSize.x - ImGui::GetStyle().WindowPadding.x;
	rendererH = windowSize.y - (ImGui::GetStyle().WindowPadding.y + 26);
	if (refreshRenderTextures)
	{
		levelRender.create(rendererW, rendererH);
		refreshRenderTextures = false;
	}
	levelRender.clear();
	levelRender.setView(levelView);
	if (Workspace->isValidLevel(Workspace->currentLevel))
	{
		for (int y = 0; y < RoomHeight; y++)
		{
			for (int x = 0; x < RoomWidth; x++)
			{
				sf::Sprite s;
				s.setTexture(*defaultTexture);
				s.setPosition(sf::Vector2f(x * 32 , y * 32));
				s.setTextureRect(sf::IntRect(0, 0, 32, 32));
				levelRender.draw(s);
			}
		}
		sf::Text t;
		//sf::Text(std::string("test"), defaultFont)
		t.setFont(defaultFont);
		t.setString(std::string(Workspace->GetLevelName(Workspace->currentLevel)));
		t.setPosition(0, -36);
		levelRender.draw(t);
		levelRender.display();
		ImGui::Image(levelRender.getTexture());
		if (ImGui::IsItemHovered())
		{
			static ImVec2 lastOffset;
			static float lastScroll = 0.0f;
			if ((ImGui::IsMouseDown(2)) && ImGui::GetIO().KeyAlt)
			{
				levelView.move(sf::Vector2f(-ImGui::GetMouseDragDelta(2).x*wscroll, -ImGui::GetMouseDragDelta(2).y*wscroll) - sf::Vector2f(lastOffset));
			}
			float scroll = pow(1.25, -ImGui::GetIO().MouseWheel);
			wscroll *= scroll;
			//levelView.zoom(scroll);
			levelView.setSize(sf::Vector2f(rendererW*wscroll, -rendererH*wscroll)); // y axis has to be flipped, gosh
			lastOffset = ImVec2(-ImGui::GetMouseDragDelta(2).x*wscroll, -ImGui::GetMouseDragDelta(2).y*wscroll);
			lastScroll = scroll;
		}
	}
	else {
		ImGui::Text((std::to_string(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x) + " " + std::to_string(ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y + 26)).c_str());
		ImGui::Text("Please select a level");
	}
}

int main(int argc, char * argv[])
{
	Print("Initializing %s...", DE_STR);
	for (int i = 0; i < argc; i++)
	{
		//Print("Arg %d: %s", i, argv[i]);
	}
	if (argc >= 2)
	{
		Print("Loading %s...", argv[1]);
		workspaceToLoad = std::string(argv[1]);
	}
	sf::RenderWindow window(sf::VideoMode(800, 600), "DiaryEditor", sf::Style::Default);

	ImFont *f = ImGui::GetIO().Fonts->AddFontFromFileTTF("segoeui.ttf", 16.0f);//AddFontFromMemoryTTF((void *)roboto_data, roboto_size, 12.0f);
	ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&fontpixels, &fntwidth, &fntheight);
	s_customFontTexture = new sf::Texture;
	s_customFontTexture->create(fntwidth, fntheight);
	s_customFontTexture->update(fontpixels);

	defaultTexture = new sf::Texture;
	defaultTexture->loadFromMemory(defaultTex_data, defaultTex_size);

	defaultFont.loadFromFile("segoeui.ttf");

	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window, s_customFontTexture);
	window.setTitle(DE_STR);

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
	style2->WindowTitleAlign = ImVec2(0.5f,0.5f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.90f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 0.90f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
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

	levelView.setCenter(sf::Vector2f(RoomWidth * 16.0f, RoomHeight * 16.0f)); // for the viewport

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
				refreshRenderTextures = true;
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
					Workspace->currentLevel = 0;
					Workspace->currentSelection = SelectionType::Floor;
				}
			}
		}
		ImGui::EndChild();
		ImGui::BeginChild("fbuttons");
		if (ImGui::Button("Add Floor..."))
		{
			ImGui::OpenPopup("Add a new floor");
		}
		AddFloorDialog();
		ImGui::SameLine();
		if (ImGui::Button("Delete") && Workspace->isValidFloor(Workspace->currentFloor))
		{
			ImGui::OpenPopup("Delete the floor?");
		}
		DeleteFloorDialog();
		ImGui::EndChild();
		ImGui::EndGroup();
		ImGui::EndDock();


		ImGui::BeginDock("Levels");
		ImGui::BeginGroup();

		ImGui::BeginChild("level view", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
		if (Workspace->isValidFloor(Workspace->currentFloor))
		{
			// TODO: List levels here
			for (int i = 0; i < Workspace->GetLevelCount(); i++)
			{
				if (ImGui::Selectable(Workspace->GetLevelName(i), Workspace->currentLevel == i))
				{
					Workspace->currentLevel = i;
					Workspace->currentSelection = SelectionType::Level;
				}
			}
		}
		ImGui::EndChild();

		ImGui::BeginChild("lbuttons");
		if (ImGui::Button("Add Level...") && Workspace->isValidFloor(Workspace->currentFloor))
		{
			ImGui::OpenPopup("Add a new level");
		}
		AddLevelDialog();
		ImGui::SameLine();
		if (ImGui::Button("Delete") && Workspace->isValidLevel(Workspace->currentLevel))
		{
			ImGui::OpenPopup("Delete the level?");
		}
		DeleteLevelDialog();
		ImGui::EndChild();

		ImGui::EndGroup();
		ImGui::EndDock();


		ImGui::BeginDock("Entity List", &entityListWindowOpen, (ImGuiWindowFlags)0, d);
		ImGui::EndDock();


		ImGui::BeginDock("Property Editor", &entityPropertiesWindowOpen, (ImGuiWindowFlags)0, d);
		static int style = 0;
		static bool overrideStyle = false;
		switch (Workspace->currentSelection)
		{
		case SelectionType::None:
			ImGui::Text("Nothing is selected!");
			break;
		case SelectionType::Entity:
			ImGui::Text("An entity is currently selected");
			break;
		case SelectionType::Floor:
			ImGui::InputText("Name", Workspace->GetCurrentFloor()->name, 128);
			ImGui::InputText("Script Name", Workspace->GetCurrentFloor()->scriptName, 128);
			ImGui::InputInt("Style ID", &Workspace->GetCurrentFloor()->style);
			break;
		case SelectionType::Level:
			ImGui::InputText("Name", Workspace->GetCurrentLevel()->name, 128);
			ImGui::InputText("Script Name", Workspace->GetCurrentLevel()->scriptName, 128);
			ImGui::Checkbox("Override Floor's style", &Workspace->GetCurrentLevel()->overrrideFloorStyle);
			if(Workspace->GetCurrentLevel()->overrrideFloorStyle) ImGui::InputInt("Style ID", &Workspace->GetCurrentLevel()->style);
			if (ImGui::CollapsingHeader("Doors"))
			{
				ImGui::Checkbox("North", &Workspace->GetCurrentLevel()->doorEnabled[NORTH_DOOR]);
				if (Workspace->GetCurrentLevel()->doorEnabled[NORTH_DOOR]) ImGui::InputText("Target Level", Workspace->GetCurrentLevel()->doorTarget[NORTH_DOOR], 128);
				ImGui::Checkbox("East", &Workspace->GetCurrentLevel()->doorEnabled[EAST_DOOR]);
				if (Workspace->GetCurrentLevel()->doorEnabled[EAST_DOOR]) ImGui::InputText("Target Level", Workspace->GetCurrentLevel()->doorTarget[EAST_DOOR], 128);
				ImGui::Checkbox("South", &Workspace->GetCurrentLevel()->doorEnabled[SOUTH_DOOR]);
				if (Workspace->GetCurrentLevel()->doorEnabled[SOUTH_DOOR]) ImGui::InputText("Target Level", Workspace->GetCurrentLevel()->doorTarget[SOUTH_DOOR], 128);
				ImGui::Checkbox("West", &Workspace->GetCurrentLevel()->doorEnabled[WEST_DOOR]);
				if (Workspace->GetCurrentLevel()->doorEnabled[WEST_DOOR]) ImGui::InputText("Target Level", Workspace->GetCurrentLevel()->doorTarget[WEST_DOOR], 128);
			}
			break;
		case SelectionType::EntityDef:
			
			break;
		}
		ImGui::EndDock();

		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		
		ImGui::BeginDock("Main View", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
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
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					Workspace->Save(workspaceToLoad);
				}
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
		RenderMainView();
		ImGui::EndDock();
		ImGui::BeginDock("Workspace Objects", &workspaceObjectsWindowOpen, (ImGuiWindowFlags)0, d);
		ImGui::BeginChild("wObjs", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
		if (ImGui::CollapsingHeader("Entities"))
		{
			for (int i = 0; i < Workspace->entities.size(); i++)
			{
				if (ImGui::Selectable(Workspace->entities[i].name.c_str(), Workspace->currentEntityDef == i))
				{
					Workspace->currentEntityDef = i;
					Workspace->currentSelection = SelectionType::EntityDef;
				}
			}
		}
		if (ImGui::CollapsingHeader("Styles"))
		{
			
		}
		ImGui::EndChild();
		ImGui::BeginChild("WorkspaceBtns");
		if (ImGui::Button("Add entity definition"))
		{
			ImGui::OpenPopup("Add a new entity definition");
		}
		AddEntDefDialog();
		ImGui::SameLine();
		ImGui::Button("Add style definition");
		ImGui::EndChild();
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

