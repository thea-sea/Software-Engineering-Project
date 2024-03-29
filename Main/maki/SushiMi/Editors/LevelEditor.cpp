/*!*****************************************************************************

\file       LevelEditor.cpp
\author     Thea Sea. thea.sea, 2102348
\par        DP email: thea.sea@digipen.edu
\par        Course: CSD2400/GAM200
\date       7-10-2022

\brief
This program implements a Game Engine Editor using ImGui
Modifies and saves each game project into a file upon pressing the save option.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

*******************************************************************************/
#define STB_IMAGE_IMPLEMENTATION
//#include "../../stb-master/stb_image.h"

#include "LevelEditor.h"
#include "imfilebrowser.h"
#include "../Engine/Shaders/ShaderLibrary.h"
#include "../src/Window.h"
#include "../Engine/TileMap/Map.h"
#include "../Engine/Serialiser/JSONSerializer.h"
#include "../Engine/Factory/Factory.h"
#include "../Headers/SceneManager.h"
#include <sstream>


//#include "../Engine/Core/Core.h"
namespace Core
{
	int value = 'Q';
	int IDcounter = 0;

	// Simple helper function to load an image into a OpenGL texture with common settings
	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
	{
		// Load from file
		int image_width = 0;
		int image_height = 0;
		unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
		if (image_data == NULL)
			return false;

		// Create a OpenGL texture identifier
		GLuint image_texture;
		glGenTextures(1, &image_texture);
		glBindTexture(GL_TEXTURE_2D, image_texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

		// Upload pixels into texture
	#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	#endif
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		stbi_image_free(image_data);

		*out_texture = image_texture;
		*out_width = image_width;
		*out_height = image_height;

		return true;
	}
	

	//std::vector<std::string> ObjVector;
	//std::vector<const char*> objectString;
	int i = 0;
	//unsigned int VBO, VAO;
	//unsigned int EBO;
	// create a file browser instance
	static ImGui::FileBrowser fileDialog;
	std::filesystem::path m_curr_path;
	static const std::filesystem::path s_TextureDirectory = "../TileMap";

	//open text files that hold enums, char and filepath
	std::ifstream grid_number_textfile;
	std::ifstream wall_type_textfile;

	namespace Editor
	{
		

		void LevelEditor::imguiEditorInit(void)
		{
#if defined(EDITOR) | defined(_EDITOR)
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

			//float fontSize = 18.0f;// *2.0f;
			/*io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", fontSize);
			io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", fontSize);*/

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			//SetDarkThemeColors();

			GLFWwindow* window = static_cast<GLFWwindow*>(Window::window_ptr);
			//GLFWwindow* windowtest = glfwCreateWindow(400, 200, "TEST", NULL, NULL);/* = static_cast<GLFWwindow*>(Window::window_ptr)*/;

			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 450");


			//code to fill a vector with the names of all game objects, replace hardcoded 57 with the detected number of game elements
			/*for (int i = 1; i < 57 * 14; i += 14)
			{
				const char* s2 = Scene::ObjVector[i].c_str();
				objectString.push_back(s2);
			}*/

			// create a file browser instance
			//static ImGui::FileBrowser fileDialog;
			

			m_curr_path = s_TextureDirectory;
			// (optional) set browser properties
			fileDialog.SetTitle("ImGui File Explorer");
			//fileDialog.SetPwd("../maki/textures/");
			//fileDialog.SetCurrentTypeFilterIndex(0);
			alphabet = "click on a grid to see its object value";

			wall_type_textfile.open("../Data/EditorEnums/wall_type.txt");
			grid_number_textfile.open("../Data/EditorEnums/grid_number.txt");
			if (!wall_type_textfile)
			{
				std::cout << "Unable to open wall_type file for wGrids!";
				return;
			}
			if (!grid_number_textfile)
			{
				std::cout << "Unable to open grid_number file for gGrids!";
				return;
			}
			/*std::getline(wall_type_textfile, Window::realstring);*/

			
			//load wall (Bottom Layer)
			std::string str;
			wall_type_textfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore very first line

			while (getline(wall_type_textfile, str))
			{
				std::istringstream line_sstm{ str };
				addedobjinfo iter;
				line_sstm >> iter.objname;
				line_sstm >> iter.enum_;
				line_sstm >> iter.filepath;
				BottomLayerLegend.push_back(iter);
				//std::cout << i.enum_ << std::endl;

			}
			
			wall_type_textfile.close();

			//load ingredient grid (Top Layer)
			grid_number_textfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore very first line

			while (getline(grid_number_textfile, str))
			{
				std::istringstream line_sstm{ str };
				addedobjinfo iter;
				line_sstm >> iter.objname;
				line_sstm >> iter.enum_;
				line_sstm >> iter.filepath;
				TopLayerLegend.push_back(iter);
				//std::cout << i.enum_ << std::endl;

			}
			grid_number_textfile.close();

			hierarchyload = false;
#endif
		}

		void LevelEditor::imguiGraphicsTest(void)
		{
#if defined(EDITOR) | defined(_EDITOR)

			/*if (m_BlockEvents)
			{*/
			//}

			if (fileDialog.HasSelected())
			{
				std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
				path = fileDialog.GetSelected().string();
				std::replace(path.begin(), path.end(), '\\', '/');
				texpath = path.c_str();

				fileDialog.ClearSelected();
			}
#endif
		}

		void LevelEditor::imguiEditorDraw(void)
		{
#if defined(EDITOR) | defined(_EDITOR)
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			//make the whole screen dockable
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
			//ImGui::GetMainViewport();

			fileDialog.SetTypeFilters({ ".txt"});
			fileDialog.SetPwd("../TileMap"); //current working directory is TileMap

			//in-Editor viewport view:
			//ImGui::Begin("Screen Viewport FrameBuffer Window");
			//int my_image_width = 0;
			//int my_image_height = 0;
			//GLuint my_image_texture = 0;
			//
			//
			////get texel data
			////IM_ASSERT(ret);
			//ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80));
			////get frame buffer
			//ImGui::End(); //end frame buffer



			//***************************************DISPLAYING TILES*************************************
			//if (ImGui::Begin("Tile Selector"))
			//{
			//	//start tile selector
			//	int my_image_width = 0;
			//	int my_image_height = 0;
			//	GLuint my_image_texture = 0;

			//	//ImGui::BeginTabBar("Hi");

			//	bool ret = LoadTextureFromFile("../textures/Tiles/Ingredients/Ingredients0_cucumber.png", &my_image_texture, &my_image_width, &my_image_height);
			//	IM_ASSERT(ret);
			//	if (ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80)))
			//	{
			//		std::cout << "button for cucumber is pressed" << std::endl;
			//		texpath = "../textures/Tiles/Ingredients/Ingredients0_cucumber.png";
			//	}
			//	ImGui::SameLine();

			//	//salmon
			//	ret = LoadTextureFromFile("../textures/Tiles/Ingredients/Ingredients0_salmon.png", &my_image_texture, &my_image_width, &my_image_height);
			//	IM_ASSERT(ret);
			//	if (ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80)))
			//	{
			//		texpath = "../textures/Tiles/Ingredients/Ingredients0_salmon.png";
			//	}

			//	ImGui::SameLine();
			//	ret = LoadTextureFromFile("../textures/Tiles/Ingredients/Ingredients0_avocado.png", &my_image_texture, &my_image_width, &my_image_height);
			//	IM_ASSERT(ret);
			//	if (ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80)))
			//	{
			//		texpath = "../textures/Tiles/Ingredients/Ingredients0_avocado.png";
			//	}


			//	ret = LoadTextureFromFile("../textures/Tiles/Ingredients/Soya_Ingredient.png", &my_image_texture, &my_image_width, &my_image_height);
			//	IM_ASSERT(ret);
			//	if (ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80)))
			//	{
			//		texpath = "../textures/Tiles/Ingredients/Soya_Ingredient.png";
			//	}

			//	ImGui::SameLine();
			//	ret = LoadTextureFromFile("../textures/Tiles/Ingredients/Ingredients1_nori.png", &my_image_texture, &my_image_width, &my_image_height);
			//	IM_ASSERT(ret);
			//	if (ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80)))
			//	{
			//		texpath = "../textures/Tiles/Ingredients/Ingredients1_nori.png";
			//	}

			//	ImGui::SameLine();
			//	ret = LoadTextureFromFile("../textures/Tiles/Ingredients/Ingredients0_tuna.png", &my_image_texture, &my_image_width, &my_image_height);
			//	IM_ASSERT(ret);
			//	if (ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80)))
			//	{
			//		texpath = "../textures/Tiles/Ingredients/Ingredients0_tuna.png";
			//	}


			//	ret = LoadTextureFromFile("../textures/Tiles/Ingredients/Wasabi_Ingredient.png", &my_image_texture, &my_image_width, &my_image_height);
			//	IM_ASSERT(ret);
			//	if (ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80)))
			//	{
			//		texpath = "../textures/Tiles/Ingredients/Wasabi_Ingredient.png";
			//	}
			//	ImGui::SameLine();
			//	ret = LoadTextureFromFile("../textures/Tiles/Ingredients/Ingredients0_ew_corn.png", &my_image_texture, &my_image_width, &my_image_height);
			//	IM_ASSERT(ret);
			//	if (ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80)))
			//	{
			//		texpath = "../textures/Tiles/Ingredients/Ingredients0_ew_corn.png";
			//	}
			//	ImGui::SameLine();
			//	ret = LoadTextureFromFile("../textures/Tiles/Ingredients/Ingredients0_roes.png", &my_image_texture, &my_image_width, &my_image_height);
			//	IM_ASSERT(ret);
			//	if (ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80)))
			//	{
			//		texpath = "../textures/Tiles/Ingredients/Ingredients0_roes.png";
			//	}

			//	ret = LoadTextureFromFile("../textures/Tiles/Ingredients/Ingredients0_rice.png", &my_image_texture, &my_image_width, &my_image_height);
			//	IM_ASSERT(ret);
			//	if (ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80)))
			//	{
			//		texpath = "../textures/Tiles/Ingredients/Ingredients0_rice.png";
			//	}
			//	ImGui::SameLine();
			//	ret = LoadTextureFromFile("../textures/Tiles/Ingredients/Ingredients0_inari.png", &my_image_texture, &my_image_width, &my_image_height);
			//	IM_ASSERT(ret);
			//	if (ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80)))
			//	{
			//		texpath = "../textures/Tiles/Ingredients/Ingredients0_inari.png";
			//	}
			//	ImGui::SameLine();
			//	ret = LoadTextureFromFile("../textures/Tiles/Ingredients/Ingredients0_tofu.png", &my_image_texture, &my_image_width, &my_image_height);
			//	IM_ASSERT(ret);
			//	if (ImGui::ImageButton((void*)(intptr_t)my_image_texture, ImVec2(80, 80)))
			//	{
			//		texpath = "../textures/Tiles/Ingredients/Ingredients0_tofu.png";
			//	}
			//	//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f)); //uncomment if u want to make button bg transparent
			//	//ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
			//	//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));


			//	////salmon
			//	//int my_image_width2 = 0;
			//	//int my_image_height2 = 0;
			//	//GLuint my_image_texture2 = 0;
			//	//bool ret2 = LoadTextureFromFile("../textures/Tiles/Ingredients/Ingredients0_salmon.png", &my_image_texture2, &my_image_width2, &my_image_height2);
			//	//IM_ASSERT(ret2);
			//	//if (ImGui::ImageButton((void*)(intptr_t)my_image_texture2, ImVec2(80, 80)))
			//	//{
			//	//	texpath = "../textures/Tiles/Ingredients/Ingredients0_salmon.png";
			//	//}

			//	//ImGui::PopStyleColor(3); //free the custom transparency buttons
			//}
			//ImGui::End(); 
			//******************************************end tile selector****************************


			/*bool b = false;
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("menu"))
				{
					if (ImGui::MenuItem("menu item"))
					{
						b = true;
					}
					if (ImGui::MenuItem("menu item 2"))
					{
						b = true;
					}
					ImGui::EndMenu();

				}
				ImGui::EndMainMenuBar();
			}

			if (b)
			{
				ImGui::OpenPopup("popup");
			}

			if (ImGui::BeginPopupModal("popup"))
			{
				ImGui::Text("Load Level - Select JSON file");
				if (ImGui::Button("OK"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}*/

			//ImGui::ShowDemoWindow();
			//ImGui::ShowDebugLogWindow();


			/*if (drawTexture)
			{
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}*/
			ImGui::Begin("Ingredient/Animated Top Layer");
			ImGui::Text("Object Hierachy - Top Layer");
			if (ImGui::BeginTable("Object Hierachy", 4))
			{	
				
				for (auto& iter : Hierarchy_TopLayer)
				{
					ImGui::TableNextRow();

					ImGui::TableNextColumn();
					ImGui::Text("ID: %d", iter.ID);
					ImGui::TableNextColumn();
					ImGui::Text("Name: %s", iter.objname.c_str());
					ImGui::TableNextColumn();
					ImGui::Text("Grid X: %d", ((int)(iter.x) / SceneManager::getTileWidth()));
					ImGui::TableNextColumn();
					ImGui::Text("Grid Y: %d", ((int)(iter.y) / SceneManager::getTileHeight()));
					ImGui::TableNextColumn();

				}
				ImGui::EndTable();

			}
			ImGui::End(); //End Layer 1

			ImGui::Begin("Wall/Box Layer");
			ImGui::Text("Object Hierachy - Bottom Layer");
			if (ImGui::BeginTable("Object Hierachy", 4))
			{
				for (auto& iter : Hierarchy_BottomLayer)
				{
					ImGui::TableNextRow();

					ImGui::TableNextColumn();
					ImGui::Text("ID: %d", iter.ID);
					ImGui::TableNextColumn();
					ImGui::Text("Name: %s", iter.objname.c_str());
					ImGui::TableNextColumn();
					ImGui::Text("Grid X: %d", ((int)(iter.x) / SceneManager::getTileWidth()));
					ImGui::TableNextColumn();
					ImGui::Text("Grid Y: %d", ((int)(iter.y) / SceneManager::getTileHeight()));
					ImGui::TableNextColumn();

				}
				ImGui::EndTable();

			}
			ImGui::End(); //end Layer 2

			//ImGui::Text("%s", * JSONSerializer::LevelLoadPathPtr);
			ImGui::Begin("Object Editor - Imgui Window");
			//int value = Map::GetValue(xgrid, ygrid);
			ImGui::SliderInt("ASCII Value to set to tile", &value, '!', '~');
			/*if (ImGui::Button("Set Value"))
			{
				Map::SetValue(xgrid, ygrid, value);
			}*/
			if (hierarchyload == false)
			{

				for (auto& iter : SceneManager::tilecontainer)
				{
					for (auto& legend : BottomLayerLegend)
					{
						if (static_cast<char>(iter.first) == legend.enum_)
						{
							addedobjinfo objprop;

							objprop.objname = legend.objname;
							objprop.x = static_cast<int>(iter.second->transformation.Position.x);
							objprop.y = static_cast<int>(iter.second->transformation.Position.y);
							objprop.ID = ++IDcounter;
							Hierarchy_BottomLayer.push_back(objprop);
						}


					}
				}
				for (auto& iter : SceneManager::ingredientcontainer)
				{
					for (auto& legend2 : TopLayerLegend)
					{	
						
						if (static_cast<char>(iter.nametag) == legend2.enum_)
						{
							addedobjinfo objprop;

							objprop.objname = legend2.objname;
							objprop.x = static_cast<int>(iter.spr->transformation.Position.x);
							objprop.y = static_cast<int>(iter.spr->transformation.Position.y);
							objprop.ID = ++IDcounter;
							Hierarchy_TopLayer.push_back(objprop);
							//std::cout << static_cast<char>(iter.IC) << std::endl;
						}

					}
				}
				for (auto& iter : SceneManager::topcontainer)
				{
					for (auto& legend2 : TopLayerLegend)
					{

						if (static_cast<char>(iter.first) == legend2.enum_)
						{
							addedobjinfo objprop;

							objprop.objname = legend2.objname;
							objprop.x = static_cast<int>(iter.second->transformation.Position.x);
							objprop.y = static_cast<int>(iter.second->transformation.Position.y);
							objprop.ID = ++IDcounter;
							Hierarchy_TopLayer.push_back(objprop);
						}

					}
				}
				hierarchyload = true;
			}


			ImGui::Text("Select a file below to load a saved level!");
			ImGui::BulletText("level text files are in ../TileMap");


			/******************************
				DESERIALIZE - LOAD A LEVEL
			******************************/
			//OPEN FILES
			if (ImGui::Begin("Object Editor - Imgui Window"))
			{
				// open file dialog when user clicks this button
				if (ImGui::Button("Click here to Load level!"))
				{
					fileDialog.Open();
					loadnewlevel = true;
				}

			}
			ImGui::End();
			fileDialog.Display(); //only need to call once in entire file

			if (fileDialog.HasSelected() && loadnewlevel == true)
			{
				std::cout << "You are now loading a level!" << std::endl;

				//std::cout << "Selected Load file before conversion " << fileDialog.GetSelected().string() << std::endl;
				path2 = fileDialog.GetSelected().string();
				std::replace(path2.begin(), path2.end(), '\\', '/');
				imguiloadedmap = path2.c_str();
				//std::cout << "Selected Load file AFTER conversion" << *JSONSerializer::LevelLoadPathPtr << std::endl;

			
				
				//delete Window::player; //delete old player? but it feels so inappropriate to put it here if i need to delete all objects in future to replace them
				//Window::player = Core::Deserialize(*Core::LevelLoadPathPtr);
				Window::loaded = false;

				std::cout << "You have loaded Level from text file: " << imguiloadedmap << std::endl;


				//clear and update the object hierarchy
				Hierarchy_TopLayer.clear();
				Hierarchy_BottomLayer.clear();
				hierarchyload = false;

				fileDialog.ClearSelected();
				loadnewlevel = false; 
				Window::timetodeletegrid = true;


			}
			/*----------------------*/
			/*Content Browser Panel*/
			/*----------------------*/
			//ImGui::Begin("Content Browser");

			//if(m_curr_path != std::filesystem::path(s_TextureDirectory))
			//{
			//	if(ImGui::Button("<-")) //will only show if u went into a folder in the current directory above
			//	{
			//		m_curr_path = m_curr_path.parent_path();
			//	}
			//}
			//float padding = 10.f;
			//float thumbnail_size = 128;
			////float cellSize = thumbnail_size + padding;

			////float panelWidth = ImGui::GetContentRegionAvail().x;
			////float columnCount = (int)(panelWidth / cellSize);
			//ImGui::Columns(5, 0, false);

			//for( auto& directory_entry : std::filesystem::directory_iterator(m_curr_path))
			//{
			//	std::string path1 = directory_entry.path().string();
			//	auto relative_path = std::filesystem::relative(directory_entry.path(),s_TextureDirectory);

			//	ImGui::Button(path1.c_str(), { thumbnail_size, thumbnail_size });
			//	ImGui::Text(path1.c_str());

			//	if (directory_entry.is_directory())
			//	{
			//		//std::cout << "the directory im clicking is: " << directory_entry.is_directory() << std::endl;
			//		/*
			//		if (ImGui::Button(path.c_str()))
			//		{
			//			m_curr_path /= directory_entry.path().filename();
			//		}
			//		*/
			//	}
			//	else
			//	{
			//		/*
			//		if(ImGui::Button(path.c_str()))
			//		{
			//			
			//		}
			//		*/
			//	}
			//	ImGui::NextColumn();
			//	
			//}
			//ImGui::Columns(1);
			//ImGui::SliderFloat("Thumbnail Size", &thumbnail_size, 16, 512);
			//ImGui::SliderFloat("Padding", &padding, 0, 32);
			//ImGui::End();


			/*currently not working oops*/
			//ImGui::Text("Click to Create New Object/Modify Object");

			//ImGui::Text("The selected object is: ");
			////ImGui::Checkbox("Display Texture", &mainclass::drawTexture);
			//// Slider that appears in the window
			//ImGui::SliderFloat("Size", &mainclass::size, 0.5f, 2.0f);
			//// Fancy color editor that appears in the window
			//ImGui::ColorEdit4("Color", mainclass::color);


			//ImGui::Text(objectString[i]);
			//ImGui::SliderInt("Object Index", &i, 0, objectString.size()-1);
			//ImGui::SliderFloat("x-position", &Object::objects[objectString[i]].position.x, -500.0f, 500.0f);
			//ImGui::SliderFloat("y-position", &Object::objects[objectString[i]].position.y, -500.0f, 500.0f);


			//float *arr[3] = { &Object::objects["temp"].color.r, &Object::objects["temp"].color.g, &Object::objects["temp"].color.b/*, Object::objects["Object5"].color.a*/ };

			//RGB colour selection
			//ImGui::ColorEdit3("Color", *arr);
			ImGui::Spacing();
			ImGui::Text("Currently displaying: ");
			ImGui::Text(imguiloadedmap.c_str());

			ImGui::Spacing();
			ImGui::Text("Click on a cell to see its value");

			ImGui::Spacing();
			ImGui::Text("Grid column (X): ");
			ImGui::SameLine();
			std::stringstream xstring;
			xstring << xgrid;
			ImGui::Text(xstring.str().c_str());

			ImGui::Text("Grid row (Y): ");
			ImGui::SameLine();
			std::stringstream ystring;
			ystring << ygrid;
			ImGui::Text(ystring.str().c_str());

			ImGui::Text("Tile Value in clicked grid: ");
			ImGui::SameLine();
			ImGui::Text(alphabet.c_str());

			ImGui::Spacing();


			//ImGui::Text("Buttons to test textures load in editor");

			/*ImGui::ImageButton("gfx/image.png", 64, 64, 8, 0xffffff, 1, 0xff0000, 0.5);*/

			/*if (ImGui::Button("Cucumber"))
			{
				texpath = "../textures/Tiles/Ingredients/Ingredients0_cucumber.png";
			}*/

			//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
			//ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
			//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
			////ImTextureID("../textures/Tiles/Ingredients/Ingredients0_octopus.png");
			//if (ImGui::ImageButton(ImTextureID("../textures/Tiles/Ingredients/Ingredients0_octopus.png"), ImVec2((float)300, (float)50), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) {
			//	Beep(400, 300);
			//	
			//}
			//ImGui::PopStyleColor(3);

			/*if (ImGui::Button("Salmon"))
			{
				texpath = "../textures/Tiles/Ingredients/Ingredients0_salmon.png";
				
			}

			if (ImGui::Button("Level1 Background"))
			{
				texpath = "../textures/level1.jpg";
			}

			if (ImGui::Button("BaMi Art"))
			{
				texpath = "../textures/test.jpg";
			}

			if (ImGui::Button("Return to Main Menu"))
			{
				texpath = "../textures/menu.jpg";
			}*/

			//if (ImGui::Button("+"))
			//{
			//	/*Window::ImGuiToObjContainer()*/
			//}

			ImGui::Spacing();
			ImGui::Text("File Save Functions:");
			/******************************
				SERIALIZE - SAVE A LEVEL
			******************************/
			if (ImGui::Begin("Object Editor - Imgui Window"))
			{
				// save objects positions into json
				if (ImGui::Button("Click here to Save Level"))
				{
					levelsave = 1;
					//std::cout << "the level is saved, level save status is " << levelsave << std::endl;
				}
				if (levelsave >= 1)
				{
					ImGui::Text("Choose file to save level :D ");
					ImGui::Spacing();
					ImGui::BulletText("take note that you can only save to \nexisting level txt files!");
					ImGui::BulletText("If you want to save to new levels, \nyou will need to manually create a new txt file");
					if (ImGui::Button("filepath containing levels is '../TileMap' "))
					{
						fileDialog.Open();
					}
					//fileDialog.Display(); //already called once upstairs
					//change the Serialised JSON file path to store new level
					if (fileDialog.HasSelected())
					{
						//std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
						path2 = fileDialog.GetSelected().string();
						std::replace(path2.begin(), path2.end(), '\\', '/');
						*Core::LevelSavePathPtr = path2.c_str();
						//std::cout << "Selected filename" << *JSONSerializer::LevelSavePathPtr << std::endl;
						levelsave = 2;

						Map::saveEditedMap(*Core::LevelSavePathPtr);
						/*Core::Serialize(*Window::player, *Core::LevelSavePathPtr);*/

						std::cout << "the level is saved! level save status is " << levelsave << std::endl;

						fileDialog.ClearSelected();
					}

				}
				//create popup to tell user that level is saved
				if (levelsave == 2)
				{
					ImGui::Button("Level saved! Please exit window ^^"); //currently a button as popup gives problems
					//ImGui::CloseCurrentPopup();

					//ImGui::EndPopup();
				}

			}
			ImGui::End(); //end level save window

			ImGui::End(); //end the whole imgui process
			ImGuiIO& io = ImGui::GetIO();
			int width__, height__;

			glfwGetWindowSize(Window::window_ptr, &width__, &height__);

			io.DisplaySize = ImVec2((float)width__, (float)height__);

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
#endif
		}

		void LevelEditor::imguiShutDown(void)
		{
#if defined(EDITOR) | defined(_EDITOR)
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

#endif
		}

		/*void LevelEditor::imguiCreateObj(ObjectFactory& container)*/

		void LevelEditor::imguiCreateObj()
		{
#if defined(EDITOR) | defined(_EDITOR)

			double xpos = 0, ypos = 0;
			double gridxpos = 0, gridypos = 0;

			int width_, height_ = 0;
			//grid snapping logic
			glfwGetCursorPos(Window::window_ptr, &xpos, &ypos);
			glfwGetWindowSize(Window::window_ptr, &width_, &height_);

			/*std::cout << "You are clicking at grid position X: " << (int)(xpos/ width_*18) << " Y: " << (int)(ypos/height_*10) << std::endl;
			std::cout << "the object in the grid is: " << static_cast<char>(Map::GetValue((int)(xpos / width_ * 18), (int)(ypos / height_ * 10))) << std::endl;
			xgrid = (int)(xpos / width_ * 18);
			ygrid = (int)(ypos / height_ * 10);*/


			i = imguiPlacedObjs;

			//newobjarr[i].spritevector = new Sprite(texpath); replace with pushing back a struct

			if (SceneManager::getTileWidth() != 0 || SceneManager::getTileHeight() != 0)
			{
				gridxpos = ((int)(xpos) / SceneManager::getTileWidth());
				gridypos = ((int)(ypos) / SceneManager::getTileHeight());

				Map::SetValue(gridxpos, gridypos, value);
				//r / static_cast<float>(max_grid_cols_x) * width

				xpos = gridxpos / static_cast<float>(Map::max_grid_cols_x) * width_;
				ypos = gridypos / static_cast<float>(Map::max_grid_rows_y) * height_;
				/**************************
				* Step 1: set ur spritepath
				* 1a) set scale,
				* 1b) set pos
				* Step 2: set ur name given to object
				* Step 3: save your pos into x and y so that Object Container can read it
				* Step 4: push back the whole struct
				****************************************************/
				addedobjinfo a;
				a.spritepath = new Sprite(texpath); //1
				a.spritepath->transformation.Position = glm::vec2(xpos, ypos); //1a
				a.spritepath->transformation.Scale = glm::vec2(SceneManager::getTileWidth()*1.01f, SceneManager::getTileHeight()*1.01f);//1b
				a.objname = "path"; //2
				a.x = static_cast<int>(xpos);
				a.y = static_cast<int>(ypos); //3
				newobjarr.push_back(a); //push back the whole struct
				//std::cout << "X: " << xpos << " Y:" << ypos << std::endl; //debug
				//newobjarr[i].spritepath->transformation.position = glm::vec2(xpos, ypos);
				//if (texpath = "../textures/Tiles/Ingredients/Ingredients0_salmon.png")
				//{
				//	newobjarr[i].objname = "salmon";//you placed a salmon!
				//}
				//else
				//	newobjarr[i].objname = "unknown";
				std::cout << "name of object you just created: " << newobjarr[i].objname << std::endl;
				Map::print_map_to_console();
			}
			/*Window::a = new Sprite(texpath);*/
			/*xpos = (float)((int)(xpos) / 100 * 100);
			ypos = (float)((int)(ypos) / 100 * 100);*/


			
			++imguiPlacedObjs;
#endif
		}

		void LevelEditor::imguiDestroyObj()
		{
#if defined(EDITOR) | defined(_EDITOR)

			for (i = 0; i < imguiPlacedObjs; ++i)
			{
				delete newobjarr[i].spritepath;
			}
#endif
		}

		void LevelEditor::AddToFactory(ObjectFactory* container)
		{
			Object::GameObject* EditorObj = container->Create();
			container->AddObjects(EditorObj, "Editor Obj");
			//container
			
			//this function is called when the "+" button to add new objects is called.
		}


		/**************************
		VALUES FOR EACH TEXTURE
		*************************/
		
		

		void LevelEditor::imguiObjectCursor(void)
		{
#if defined(EDITOR) | defined(_EDITOR)
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			if (io.WantCaptureMouseUnlessPopupClose == false)
			{
				for (auto& iter : BottomLayerLegend)
				{
					if (value == iter.enum_)
					{
						texpath = iter.filepath.c_str();
					}
				}
				for (auto& iter : TopLayerLegend)
				{
					if (value == iter.enum_)
					{
						texpath = iter.filepath.c_str();
					}
				}
				Window::ingredient = new Sprite(Editor::LevelEditor::texpath);

				//display obj to place on cursor
				int width_ = 0, height_ = 0;
				double xpos = 0, ypos = 0;
				double gridxpos = 0, gridypos = 0;

				//grid snapping logic
				glfwGetCursorPos(Window::window_ptr, &xpos, &ypos);
				glfwGetWindowSize(Window::window_ptr, &width_, &height_);
				//r / static_cast<float>(max_grid_cols_x) * width
				/*width = SceneManager::getTileWidth() * Map::max_grid_cols_x;
				height = SceneManager::getTileHeight() * Map::max_grid_rows_y;*/

				//xpos += 100 * 0.5f;
				//ypos += 100 * 0.5f;
				//xpos = col number
				//static_cast<float>(max_grid_cols_x) * width
				//xpos = (int)(xpos / SceneManager::getTileWidth()) /** (double)SceneManager::getTileWidth()*/;
				//ypos = ((int)(ypos / (double)SceneManager::getTileHeight())) * (double)SceneManager::getTileHeight();
				/*xpos = (int) (float)((int)(xpos / width_ / 18) * (width_ / 18));
				ypos = (int)(float)((int)(ypos / height_ / 10) * (height_ / 10));*/
				Window::ingredient->transformation.Scale = glm::vec2(SceneManager::getTileWidth()*1.01f, SceneManager::getTileHeight()*1.01f);
				if (SceneManager::getTileWidth() != 0 || SceneManager::getTileHeight() != 0)
				{
					gridxpos = static_cast<double>((int)(xpos) / SceneManager::getTileWidth()); 
					gridypos = static_cast<double>((int)(ypos) / SceneManager::getTileHeight());
					xgrid = ((int)(xpos) / SceneManager::getTileWidth());
					ygrid = ((int)(ypos) / SceneManager::getTileHeight());
					//std::cout << "Grid pos x: " << gridxpos << " and y: " << gridypos << std::endl;
					//std::cout << " Max Grid col x is: " << Map::max_grid_rows_y << " and Max Grid row y is: " << Map::max_grid_cols_x << std::endl;
					
					//if you are in an invalid grid range of the map
					if (gridxpos >= Map::max_grid_cols_x || gridxpos < 0 || gridypos < 0 || gridypos >= Map::max_grid_rows_y)
					{
						alphabet = "out of range";

					}
					else
					{
						//std::cout << "you are out of range!" << std::endl;
						//return 3; //if you are pressing out of this grid, return 0 as tile value 
						alphabet = static_cast<char>(Map::GetValue(gridxpos, gridypos));
						//std::cout << "x value is : " << gridxpos << " and y value is: " << gridypos << std::endl;
						if (ImGui::IsMouseClicked(0)) //0 means left
						{

							/*gridxpos = ((int)(xpos) / SceneManager::getTileWidth());
							gridypos = ((int)(ypos) / SceneManager::getTileHeight());*/

							//r / static_cast<float>(max_grid_cols_x) * width

							//Window::ingredient->transformation.Position.x =  gridxpos / static_cast<float>(Map::max_grid_cols_x) * width_ ;
							//Window::ingredient->transformation.Position.y = gridypos / static_cast<float>(Map::max_grid_rows_y) * height_;
							imguiCreateObj();

							//std::cout << "placing NEW obj at x: " << Window::ingredient->transformation.Position.x << " and y: " << Window::ingredient->transformation.Position.y << std::endl;
							Map::print_map_to_console();



						}
					}
					
				}

				Window::ingredient->transformation.Position.x = static_cast<float>(gridxpos / static_cast<float>(Map::max_grid_cols_x) * width_);
				Window::ingredient->transformation.Position.y = static_cast<float>(gridypos / static_cast<float>(Map::max_grid_rows_y) * height_);
				/*SceneManager::rows*/
					/*= glm::vec2(xpos, ypos)*/
				//place object on click
				//std::cout << "Grid width: " << SceneManager::getTileWidth() << " and height: " << SceneManager::getTileHeight() << std::endl;

				
				Shaders->Textured_Shader()->Send_Mat4("model_matrx", Window::ingredient->transformation.Get());
				Window::ingredient->draw();

				if (Window::ingredient)
				{
					delete Window::ingredient;
				}
			}
		
#endif
			
		}
	}
}