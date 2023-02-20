#include "Map.h"
#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include "../../src/Window.h"
#include "../Engine/Serialiser/JSONSerializer.h"
#include "../Engine/Shaders/ShaderLibrary.h"
#include "../Headers/SceneManager.h"

/*
 * What each grid number means:
 * 0 - free space
 * 1 - wall
 * 2 - Player
 * 3 - ingredient
 * 4 - ingredient 2
 * 50 - sinkhole
 * 9 - Goal1
 * 10- Goal2
 * If player go to sinkhole, grid change to 50. When reset, need reset sinkhole back
 * If player complete a goal, set grid of goal to 100.
 */

namespace Core
{
	
	//int Map::gGrids[GRID_ROW][GRID_COL];
	int width, height;
	unsigned int SceneManager::amt_of_win_conditions, win_amt;

	Map::Map()
	{
	}

	Map::~Map()
	{
		for (int i = 0; i < grid_row; i++)
		{
			delete gGrids[i];
		}
		delete gGrids;
		//this does deleting of textures
		/*SceneManager::destroyTile();
		SceneManager::destroyGoal1();
		SceneManager::destroyGoal2();
		SceneManager::destroyCover1();
		SceneManager::destroyIngr();
		SceneManager::destroyIngr();*/

		SceneManager::destroyTile();
		SceneManager::destroyIngr();
	}

	int** Map::gGrids;

	void Map::initMap(std::string Filename)
	{
		//Open File using ifstream
		std::ifstream fin(Filename);
		if (!fin)
		{
			std::cout << "Unable to open tilemap file!";
			return;
		}
		/*store the row and col of the tile*/
		fin >> grid_row;
		fin >> grid_col;

		gGrids = new int* [grid_row];

		for (int i = 0; i < grid_row; i++)
		{
			gGrids[i] = new int[grid_col];
		}

		char ch;

		for (int c = 0; c < grid_col; c++)
		{
			for (int r = 0; r < grid_row; r++)
			{
				fin >> ch;
				gGrids[r][c] = ch;
			}
		}


		fin.close();
	}

	void Map::saveEditedMap(std::string Filename)
	{
		std::ofstream fout(Filename);
		fout << grid_row << " " << grid_col << std::endl;

		for (int c = 0; c < grid_col; c++)
		{
			for (int r = 0; r < grid_row; r++)
			{
				fout << static_cast<char>(gGrids[r][c]) << " ";
			}
			fout << std::endl;
		}
	}

	void Map::ResetMap()
	{
		for (int i = 0; i < grid_row; i++)
		{
			for (int j = 0; j < grid_col; j++)
			{
				gGrids[i][j] = 0;
			}
		}

		SceneManager::destroyTile();
		SceneManager::destroyIngr();

		if (Window::timetodeletegrid)
		{
			for (int i = 0; i < grid_row; i++)
			{
				delete gGrids[i];
			}
			delete gGrids;
		}

		win_amt = 0;
		SceneManager::amt_of_win_conditions = 0;
	}

	int Map::LoadMap()
	{
		glfwGetWindowSize(Window::window_ptr, &width, &height);
		
		tile_width = width / grid_row;
		tile_height = height / grid_col;
		SceneManager::setTileDimension(tile_width, tile_height);
		
		/*Testing whether is loaded correctly*/
		for (int c = 0; c < grid_col; c++)
		{
			for (int r = 0; r < grid_row; r++)
			{
				switch (gGrids[r][c])
				{
				case static_cast<int>(grid_number::player):
				{
					Window::player->playerpos.x = r / static_cast<float>(grid_row) * width;
					Window::player->playerpos.y = c / static_cast<float>(grid_col) * height;
					Window::player->playerpos_restart.x = Window::player->playerpos.x;
					Window::player->playerpos_restart.y = Window::player->playerpos.y;
//#ifndef EDITOR
					Window::player->sp->transformation.Position.x = r / static_cast<float>(grid_row) * width;
					Window::player->sp->transformation.Position.y = c / static_cast<float>(grid_col) * height;
//#endif
					/*save player index in grid*/
					Window::player->player_grid_pos.x = r;
					Window::player->player_grid_pos.y = c;
					/*save initial index for restart purposes*/
					Window::player->player_initial_grid_pos.x = r;
					Window::player->player_initial_grid_pos.y = c;
					break;
				}
					
				// Ingredients are: avocado, cucumber,corn,inari,octopus,rice,roes,salmon,tamago,tofu,tuna,nori,soya,wasabi,		

				
				case static_cast<int>(grid_number::avocado):
				{
					Sprite* avocado = new Sprite("../textures/Tiles/Ingredients/Ingredients0_avocado.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::avocado, avocado);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				case static_cast<int>(grid_number::cucumber):
				{
					Sprite* cucumber = new Sprite("../textures/Tiles/Ingredients/Ingredients0_cucumber.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::cucumber, cucumber);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				case static_cast<int>(grid_number::corn):
				{
					Sprite* corn = new Sprite("../textures/Tiles/Ingredients/Ingredients0_corn.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::corn, corn);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}

				case static_cast<int>(grid_number::inari):
				{
					Sprite* inari = new Sprite("../textures/Tiles/Ingredients/Ingredients0_inari.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::inari, inari);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				case static_cast<int>(grid_number::octopus):
				{
					Sprite* octopus = new Sprite("../textures/Tiles/Ingredients/Ingredients0_octopus.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::octopus, octopus);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				case static_cast<int>(grid_number::rice):
				{
					//Sprite* rice = new Sprite("../textures/Tiles/Ingredients/Ingredients0_rice.png");
					/*edited this to be bami first to show that Sprites can be animated too, just need the spritesheet*/
					Sprite* rice = new Sprite("../textures/spritesheet/ricespritesheet.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::rice, std::move(rice));
					rice->Add_animation("../textures/spritesheet/Run.txt");
					rice->curr_anim = Idle;
					rice->isSpriteSheet = 1;
					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				//roes
				case static_cast<int>(grid_number::roes):
				{
					Sprite* roes = new Sprite("../textures/Tiles/Ingredients/Ingredients0_roes.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::roes, roes);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				//salmon
				case static_cast<int>(grid_number::salmon):
				{
					Sprite* salmon = new Sprite("../textures/Tiles/Ingredients/Ingredients0_salmon.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::salmon, salmon);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				//tamago
				case static_cast<int>(grid_number::tamago):
				{
					Sprite* tamago = new Sprite("../textures/Tiles/Ingredients/Ingredients0_tamago.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::tamago, tamago);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				//tofu
				case static_cast<int>(grid_number::tofu):
				{
					Sprite* tofu = new Sprite("../textures/Tiles/Ingredients/Ingredients0_tofu.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::tofu, tofu);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				//tuna
				case static_cast<int>(grid_number::tuna):
				{
					Sprite* tuna = new Sprite("../textures/Tiles/Ingredients/Ingredients0_tuna.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::tuna, tuna);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				//nori
				case static_cast<int>(grid_number::nori):
				{
					Sprite* nori = new Sprite("../textures/Tiles/Ingredients/Ingredients1_nori.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::nori, std::move(nori));

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				//soya
				case static_cast<int>(grid_number::soya):
				{
					Sprite* soya = new Sprite("../textures/Tiles/Ingredients/Soya_1.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::soya, soya);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				//wasabi
				case static_cast<int>(grid_number::wasabi):
				{
					Sprite* wasabi = new Sprite("../textures/Tiles/Ingredients/Wasabi_1.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::wasabi, wasabi);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					SceneManager::loadIngr_initPos(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}

				case static_cast<int>(grid_number::box1):
				{
					Sprite* box = new Sprite("../textures/Tiles/Pods/Pod_Rice.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::box1, box);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);

					SceneManager::win_condition.push_back(std::make_pair(r, c));
					SceneManager::amt_of_win_conditions++;
					break;

				}
					

				case static_cast<int>(grid_number::box2):
				{
					Sprite* box = new Sprite("../textures/Tiles/Pods/Pod_Inari.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::box2, box);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);

					SceneManager::win_condition.push_back(std::make_pair(r, c));
					SceneManager::amt_of_win_conditions++;
					break;
				}
					

				case static_cast<int>(grid_number::boxcover):
				{
					Sprite* boxcover = new Sprite("../textures/Tiles/Pods/Pod_Cover.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::boxcover, boxcover);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}

				case static_cast<int>(grid_number::sinkhole):
				{
					Sprite* sinkhole = new Sprite("../textures/Tiles/Trap/Sinkhole0_1.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::boxcover, sinkhole);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
					
				case static_cast<int>(grid_number::filledsinkhole):
				{
					Sprite* filledsinkhole = new Sprite("../textures/Tiles/Trap/Sinkhole_Filled.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::boxcover, filledsinkhole);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}

				//Temporary placeholder for food in sinkhole
				case static_cast<int>(grid_number::temp):
				{
					Sprite* foodinsinkhole = new Sprite("../textures/doge.png");
					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::temp, foodinsinkhole);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				case static_cast<int>(grid_number::ground0):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Ground/RicePlain_Ground0_0.jpg");

					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::temp, tile);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				case static_cast<int>(grid_number::ground1):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Ground/RicePlain_Ground0_1.jpg");

					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::temp, tile);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				case static_cast<int>(grid_number::ground2):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Ground/RicePlain_Ground0_2.jpg");

					std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::temp, tile);

					SceneManager::loadIngr(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, r, c, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall0):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall0.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall0, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall0_1):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall0_1.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall0_1, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall1):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall1.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall1, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall2):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall2.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall2, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
					


				case static_cast<int>(wall_type::Wall2_1):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall2_1.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall2_1, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
					

				case static_cast<int>(wall_type::Wall2_2):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall2_2.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall2_2, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall2_3):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall2_3.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall2_3, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
					
				case static_cast<int>(wall_type::Wall3):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall3.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall3, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}

				case static_cast<int>(wall_type::Wall3_1):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall3_1.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall3_1, tile);
					//tile->Add_animation()
					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}

				case static_cast<int>(wall_type::Wall3_2):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall3_2.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall3_2, tile);
					//tile->Add_animation()
					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}


				case static_cast<int>(wall_type::Wall3_3):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall3_3.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall3_3, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}

				case static_cast<int>(wall_type::Wall4):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall4.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall4, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
					
				case static_cast<int>(wall_type::Wall4_1):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall4_1.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall4_1, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall4_2):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall4_2.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall4_2, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall4_3):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall4_3.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall4_3, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall4_4):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall4_4.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall4_4, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
					

					
				case static_cast<int>(wall_type::Wall5):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall5.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall5, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
					
					
				//most common wall
				case static_cast<int>(wall_type::Wall5_1):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall5_1.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall5_1, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;

				}
					
				case static_cast<int>(wall_type::Wall5_2):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall5_2.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall5_2, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;

				}

				case static_cast<int>(wall_type::Wall5_3):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall5_3.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall5_3, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;

				}
				
				case static_cast<int>(wall_type::Wall5_4):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall5_4.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall5_4, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;

				}
				case static_cast<int>(wall_type::Wall6):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall6.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall6, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall6_1):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall6_1.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall6_1, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;

				}

				case static_cast<int>(wall_type::Wall6_2):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall6_2.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall6_2, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;

				}

				case static_cast<int>(wall_type::Wall6_3):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall6_3.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall6_3, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;

				}

				case static_cast<int>(wall_type::Wall6_4):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall6_4.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall6_4, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;

				}
				//wall 7
				case static_cast<int>(wall_type::Wall7):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall7.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall7, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall7_1):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall7_1.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall7_1, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;

				}

				case static_cast<int>(wall_type::Wall7_2):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall7_2.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall7_2, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;

				}

				case static_cast<int>(wall_type::Wall7_3):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall7_3.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall7_3, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;

				}

				case static_cast<int>(wall_type::Wall7_4):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall7_4.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall7_4, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;

				}
				//wall 8
				case static_cast<int>(wall_type::Wall8):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall8.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall8, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				//wall 9
				case static_cast<int>(wall_type::Wall9):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall9.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall9, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall9_1):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall9_1.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall9_1, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall9_2):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall9_2.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall9_2, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall9_3):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall9_3.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall9_3, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall9_4):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall9_4.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall9_4, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall9_5):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall9_5.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall9_5, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall9_6):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall9_6.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall9_6, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall9_7):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall9_7.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall9_7, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				case static_cast<int>(wall_type::Wall9_8):
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall9_8.jpg");
					std::pair<wall_type, Sprite*> combine = std::make_pair(wall_type::Wall9_8, tile);

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, combine);
					break;
				}
				default:
					assert("Texture type not found\n");
					break;
				}
			}
		}
		//scale the player according to map size
		Player::sp->transformation.Scale = glm::vec2(SceneManager::getTileWidth(), SceneManager::getTileHeight());


		print_map_to_console();

		return 1;
	}

	bool Map::isWin()
	{
		return ((win_amt == SceneManager::amt_of_win_conditions) ? true : false);
	}


	bool Map::isStuck()
	{
		// if player's grid index is 50, means its STUCK or put all ingr into goals
		if(gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::sinkhole) &&
			!isWin())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Map::collision_check_left()
	{
		if (!isStuck())
		{
			//Check if right tile is a box1
			if (gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::box1))
			{
				Window::player->move_left();
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::inbox1);
				gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
			}
			//Check if right tile is box2
			else if (gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::box2))
			{
				Window::player->move_left();
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::inbox2);
				gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
			}
			//Check if left tile is a wall or ingredient
			else if ((gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] > static_cast<int>(grid_number::ingredients) &&
				gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] < static_cast<int>(grid_number::items)) ||
				(gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] > static_cast<int>(wall_type::first) &&
					gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] < static_cast<int>(wall_type::last)))
			{
				//check if left tile is ingredient
				if (gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] > static_cast<int>(grid_number::ingredients) &&
					gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] < static_cast<int>(grid_number::items))
				{
					std::cout << "left ingredient\n";
					
					//check if tile on the left of ingredient is a wall
					if (gGrids[Window::player->player_grid_pos.x - 2][Window::player->player_grid_pos.y] > static_cast<int>(wall_type::first) &&
						gGrids[Window::player->player_grid_pos.x - 2][Window::player->player_grid_pos.y] < static_cast<int>(wall_type::last))
					{
						std::cout << "left ingredient wall\n";
						Window::player->stop();
					}
					//check if tile on the left of ingredient is a sinkhole
					else if (gGrids[Window::player->player_grid_pos.x - 2][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::sinkhole))
					{
						grid_number check = static_cast<grid_number>(gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y]);

						//Set grid
						gGrids[Window::player->player_grid_pos.x - 2][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::temp);
						gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);

						for (auto ingredient : SceneManager::ingredientcontainer)
						{
							if (ingredient.first == check)
							{
								ingredient.second->transformation.Position.x -= tile_width;
								break;
							}
						}

						std::cout << "left ingredient sinkhole\n";
						Window::player->move_left();
					}
					//check if tile on the left of ingredient is another food
					else if (gGrids[Window::player->player_grid_pos.x - 2][Window::player->player_grid_pos.y] > static_cast<int>(grid_number::ingredients) &&
						gGrids[Window::player->player_grid_pos.x - 2][Window::player->player_grid_pos.y] < static_cast<int>(grid_number::items))
					{
						std::cout << "left ingredient ingredient\n";
						Window::player->stop();
					}
					//check if it's a box
					else if (gGrids[Window::player->player_grid_pos.x - 2][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::box1) ||
						gGrids[Window::player->player_grid_pos.x - 2][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::box2))
					{
						grid_number check = static_cast<grid_number>(gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y]);

						gGrids[Window::player->player_grid_pos.x - 2][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::boxcover);

						Sprite* boxcover = new Sprite("../textures/Tiles/Pods/Pod_Cover.png");
						std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::boxcover, boxcover);
						SceneManager::loadIngr(Window::player->playerpos.x - (2 * tile_width + 5), Window::player->playerpos.y, Window::player->player_grid_pos.x - 2, Window::player->player_grid_pos.y, combine);

						gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);

						for (auto ingredient : SceneManager::ingredientcontainer)
						{
							if (ingredient.first == check)
							{
								ingredient.second->transformation.Position.x -= tile_width + 5;
								break;
							}
						}

						win_amt++;

						std::cout << "left ingredient box\n";
						Window::player->move_left();
					}
					//Otherwise, it is a space
					else
					{
						grid_number check = static_cast<grid_number>(gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y]);

						gGrids[Window::player->player_grid_pos.x - 2][Window::player->player_grid_pos.y] = static_cast<int>(check);
						gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);

						//check if current grid is box1
						if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox1))
						{
							gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::box1);
						}
						//check if current grid is box2
						else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox2))
						{
							gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::box2);
						}
						else
						{
							gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
						}

						for (auto ingredient : SceneManager::ingredientcontainer)
						{
							if (ingredient.first == check)
							{
								ingredient.second->transformation.Position.x -= tile_width;
								break;
							}
						}
						
						std::cout << "left ingredient space\n";
						Window::player->move_left();
					}
				}
				/*check wall*/
				else if (gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] > static_cast<int>(wall_type::first) ||
					gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] < static_cast<int>(wall_type::last))
				{
					std::cout << "Wall on left" << std::endl;
					Window::player->stop();
				}
			}
			//check with box
			else if (gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::box1) ||
				gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::box2))
			{
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
				
				Window::player->move_left();
			}
			/*check with covered box*/
			else if (gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::boxcover))
			{
				std::cout << "Cover on left\n";
				Window::player->stop();
			}
			/*check for sinkhole*/
			else if (gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::sinkhole))
			{
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);

				std::cout << "left sinkhole\n";
				Window::player->move_left();
			}
			else
			{
				//Check if up tile is a box1
				if (gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::box1))
				{
					Window::player->move_left();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::inbox1);
					gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
				}
				//Check if up tile is box2
				else if (gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::box2))
				{
					Window::player->move_left();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::inbox2);
					gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
				}
				//Check if current tile is inbox1
				else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox1))
				{
					Window::player->move_left();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
					gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::box1);
				}
				//Check if current tile is inbox2
				else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox2))
				{
					Window::player->move_left();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
					gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::box2);
				}
				else
				{
					Window::player->move_left();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
					gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
				}

				std::cout << "normal left" << std::endl;
			}
		}
	}

	void Map::collision_check_right()
	{
		if (!isStuck())
		{
			//Check if right tile is a wall or ingredient
			if ((gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] > static_cast<int>(grid_number::ingredients) &&
				gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] < static_cast<int>(grid_number::items)) ||
				(gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] > static_cast<int>(wall_type::first) &&
					gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] < static_cast<int>(wall_type::last)))
			{
				//check if right tile is ingredient
				if (gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] > static_cast<int>(grid_number::ingredients) &&
					gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] < static_cast<int>(grid_number::items))
				{
					std::cout << "right ingredient\n";

					//check if tile on the right of ingredient is a wall
					if (gGrids[Window::player->player_grid_pos.x + 2][Window::player->player_grid_pos.y] > static_cast<int>(wall_type::first) &&
						gGrids[Window::player->player_grid_pos.x + 2][Window::player->player_grid_pos.y] < static_cast<int>(wall_type::last))
					{
						std::cout << "right ingredient wall\n";
						Window::player->stop();
					}
					//check if tile on the right of ingredient is a sinkhole
					else if (gGrids[Window::player->player_grid_pos.x + 2][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::sinkhole))
					{
						grid_number check = static_cast<grid_number>(gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y]);

						gGrids[Window::player->player_grid_pos.x + 2][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::temp);
						gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);

						for (auto ingredient : SceneManager::ingredientcontainer)
						{
							if (ingredient.first == check)
							{
								ingredient.second->transformation.Position.x += tile_width;
								break;
							}
						}

						std::cout << "right ingredient sinkhole\n";
						Window::player->move_right();
					}
					//check if tile on the right of ingredient is another food
					else if (gGrids[Window::player->player_grid_pos.x + 2][Window::player->player_grid_pos.y] > static_cast<int>(grid_number::ingredients) &&
						gGrids[Window::player->player_grid_pos.x + 2][Window::player->player_grid_pos.y] < static_cast<int>(grid_number::items))
					{
						std::cout << "right ingredient ingredient\n";
						Window::player->stop();
					}
					//check if it's a box
					else if (gGrids[Window::player->player_grid_pos.x + 2][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::box1) ||
						gGrids[Window::player->player_grid_pos.x + 2][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::box2))
					{
						grid_number check = static_cast<grid_number>(gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y]);

						gGrids[Window::player->player_grid_pos.x + 2][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::boxcover);
						Sprite* boxcover = new Sprite("../textures/Tiles/Pods/Pod_Cover.png");
						std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::boxcover, boxcover);
						SceneManager::loadIngr(Window::player->playerpos.x + (2 * tile_width), Window::player->playerpos.y, Window::player->player_grid_pos.x + 2, Window::player->player_grid_pos.y, combine);

						gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);

						for (auto ingredient : SceneManager::ingredientcontainer)
						{
							if (ingredient.first == check)
							{
								ingredient.second->transformation.Position.x += tile_width;
								break;
							}
						}

						win_amt++;

						std::cout << "right ingredient box\n";
						Window::player->move_right();
					}
					//Otherwise, it is a space
					else
					{
						grid_number check = static_cast<grid_number>(gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y]);

						gGrids[Window::player->player_grid_pos.x + 2][Window::player->player_grid_pos.y] = static_cast<int>(check);
						gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);

						//check if current grid is box1
						if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox1))
						{
							gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::box1);
						}
						//check if current grid is box2
						else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox2))
						{
							gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::box2);
						}
						else
						{
							gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
						}

						for (auto ingredient : SceneManager::ingredientcontainer)
						{
							if (ingredient.first == check)
							{
								ingredient.second->transformation.Position.x += tile_width;
								break;
							}
						}

						std::cout << "right ingredient space\n";
						Window::player->move_right();
					}
				}
				/*check wall*/
				else if (gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] > static_cast<int>(wall_type::first) ||
					gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] < static_cast<int>(wall_type::last))
				{
					std::cout << "Wall on right" << std::endl;
					Window::player->stop();
				}
			}
			/*check with covered box*/
			else if (gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::boxcover))
			{
				std::cout << "Cover on right\n";
				Window::player->stop();
			}
			/*check for sinkhole*/
			else if (gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::sinkhole))
			{
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);

				std::cout << "right sinkhole\n";
				Window::player->move_right();
			}
			//Just move
			else
			{
				//Check if right tile is a box1
				if (gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::box1))
				{
					Window::player->move_right();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::inbox1);
					gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
				}
				//Check if right tile is box2
				else if (gGrids[Window::player->player_grid_pos.x + 1][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::box2))
				{
					Window::player->move_right();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::inbox2);
					gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
				}
				//Check if current tile is inbox1
				else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox1))
				{
					Window::player->move_right();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
					gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::box1);
				}
				//Check if current tile is inbox2
				else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox2))
				{
					Window::player->move_right();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
					gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::box2);
				}
				else
				{
					Window::player->move_right();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
					gGrids[Window::player->player_grid_pos.x - 1][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
				}

				std::cout << "normal right" << std::endl;
			}
		}
	}

	void Map::collision_check_down()
	{
		if (!isStuck())
		{
			//Check if down tile is a box1
			if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] == static_cast<int>(grid_number::box1))
			{
				Window::player->move_down();
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::inbox1);
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] = static_cast<int>(grid_number::space);
			}
			//Check if down tile is box2
			else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] == static_cast<int>(grid_number::box2))
			{
				Window::player->move_down();
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::inbox2);
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] = static_cast<int>(grid_number::space);
			}
			//Check if below tile is a wall or ingredient
			else if ((gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] > static_cast<int>(grid_number::ingredients) &&
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] < static_cast<int>(grid_number::items)) ||
				(gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] > static_cast<int>(wall_type::first) &&
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] < static_cast<int>(wall_type::last)))
			{
				//check if below tile is ingredient
				if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] > static_cast<int>(grid_number::ingredients) &&
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] < static_cast<int>(grid_number::items))
				{
					std::cout << "down ingredient\n";

					//check if tile below of ingredient is a wall
					if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 2] > static_cast<int>(wall_type::first) &&
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 2] < static_cast<int>(wall_type::last))
					{
						std::cout << "down ingredient wall\n";
						Window::player->stop();
					}
					//check if tile below of ingredient is a sinkhole
					else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 2] == static_cast<int>(grid_number::sinkhole))
					{
						grid_number check = static_cast<grid_number>(gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1]);

						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 2] = static_cast<int>(grid_number::temp);
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] = static_cast<int>(grid_number::player);
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);

						for (auto ingredient : SceneManager::ingredientcontainer)
						{
							if (ingredient.first == check)
							{
								ingredient.second->transformation.Position.y += tile_height;
								break;
							}
						}

						std::cout << "down ingredient sinkhole\n";
						Window::player->move_down();
					}
					//check if tile below of ingredient is another food
					else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 2] >= static_cast<int>(grid_number::ingredients) &&
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 2] <= static_cast<int>(grid_number::items))
					{
						std::cout << "down ingredient ingredient\n";
						Window::player->stop();
					}
					//check if it's a box
					else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 2] == static_cast<int>(grid_number::box1) ||
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 2] == static_cast<int>(grid_number::box2))
					{
						grid_number check = static_cast<grid_number>(gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1]);

						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 2] = static_cast<int>(grid_number::boxcover);
						Sprite* boxcover = new Sprite("../textures/Tiles/Pods/Pod_Cover.png");
						std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::boxcover, boxcover);
						SceneManager::loadIngr(Window::player->playerpos.x, Window::player->playerpos.y + (2 * tile_height), Window::player->player_grid_pos.x, Window::player->player_grid_pos.y + 2, combine);

						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] = static_cast<int>(grid_number::player);
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);

						for (auto ingredient : SceneManager::ingredientcontainer)
						{
							if (ingredient.first == check)
							{
								ingredient.second->transformation.Position.y += tile_height;
								break;
							}
						}

						win_amt++;

						std::cout << "down ingredient box\n";
						Window::player->move_down();
					}
					//Otherwise, it is a space
					else
					{
						grid_number check = static_cast<grid_number>(gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1]);

						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 2] = static_cast<int>(check);
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] = static_cast<int>(grid_number::player);

						//check if current grid is box1
						if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox1))
						{
							gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::box1);
						}
						//check if current grid is box2
						else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox2))
						{
							gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::box2);
						}
						else
						{
							gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
						}

						for (auto ingredient : SceneManager::ingredientcontainer)
						{
							if (ingredient.first == check)
							{
								ingredient.second->transformation.Position.y += tile_height;
								break;
							}
						}

						std::cout << "down ingredient space\n";
						Window::player->move_down();
					}
				}
				/*check wall*/
				else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] > static_cast<int>(wall_type::first) ||
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] < static_cast<int>(wall_type::last))
				{
					std::cout << "Wall on down" << std::endl;
					Window::player->stop();
				}
			}
			/*check with covered box*/
			else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] == static_cast<int>(grid_number::boxcover))
			{
				std::cout << "Cover on down\n";
				Window::player->stop();
			}
			/*check for sinkhole*/
			else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] == static_cast<int>(grid_number::sinkhole))
			{
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);

				std::cout << "down sinkhole\n";
				Window::player->move_down();
			}
			//Just move
			else
			{
				//Check if up tile is a box1
				if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] == static_cast<int>(grid_number::box1))
				{
					Window::player->move_down();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::inbox1);
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] = static_cast<int>(grid_number::space);
				}
				//Check if up tile is box2
				else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] == static_cast<int>(grid_number::box2))
				{
					Window::player->move_down();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::inbox2);
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] = static_cast<int>(grid_number::space);
				}
				//Check if current tile is inbox1
				else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox1))
				{
					Window::player->move_down();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] = static_cast<int>(grid_number::box1);
				}
				//Check if current tile is inbox2
				else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox2))
				{
					Window::player->move_down();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] = static_cast<int>(grid_number::box2);
				}
				else
				{
					Window::player->move_down();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] = static_cast<int>(grid_number::space);
				}

				std::cout << "normal down" << std::endl;
			}
		}
	}
	
	void Map::collision_check_up()
	{
		if (!isStuck())
		{
			//Check if above tile is a wall or ingredient
			if ((gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] > static_cast<int>(grid_number::ingredients) &&
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] < static_cast<int>(grid_number::items)) ||
				(gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] > static_cast<int>(wall_type::first) &&
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] < static_cast<int>(wall_type::last)))
			{
				//check if above tile is ingredient
				if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] > static_cast<int>(grid_number::ingredients) &&
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] < static_cast<int>(grid_number::items))
				{
					std::cout << "up ingredient\n";

					//check if tile above of ingredient is a wall
					if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 2] > static_cast<int>(wall_type::first) &&
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 2] < static_cast<int>(wall_type::last))
					{
						std::cout << "up ingredient wall\n";
						Window::player->stop();
					}
					//check if tile above of ingredient is a sinkhole
					else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 2] == static_cast<int>(grid_number::sinkhole))
					{
						grid_number check = static_cast<grid_number>(gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1]);

						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 2] = static_cast<int>(grid_number::temp);
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] = static_cast<int>(grid_number::player);
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);

						for (auto ingredient : SceneManager::ingredientcontainer)
						{
							if (ingredient.first == check)
							{
								ingredient.second->transformation.Position.y -= tile_height;
								break;
							}
						}

						std::cout << "up ingredient sinkhole\n";
						Window::player->move_up();
					}
					//check if tile above of ingredient is another food
					else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 2] > static_cast<int>(grid_number::ingredients) &&
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 2] < static_cast<int>(grid_number::items))
					{
						std::cout << "up ingredient ingredient\n";
						Window::player->stop();
					}
					//check if it's a box
					else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 2] == static_cast<int>(grid_number::box1) ||
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 2] == static_cast<int>(grid_number::box2))
					{
						grid_number check = static_cast<grid_number>(gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1]);

						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 2] = static_cast<int>(grid_number::boxcover);
						Sprite* boxcover = new Sprite("../textures/Tiles/Pods/Pod_Cover.png");
						std::pair<grid_number, Sprite*> combine = std::make_pair(grid_number::boxcover, boxcover);
						SceneManager::loadIngr(Window::player->playerpos.x, Window::player->playerpos.y - (2 * tile_height), Window::player->player_grid_pos.x, Window::player->player_grid_pos.y - 2, combine);

						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] = static_cast<int>(grid_number::player);
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);

						for (auto ingredient : SceneManager::ingredientcontainer)
						{
							if (ingredient.first == check)
							{
								ingredient.second->transformation.Position.y -= tile_height;
								break;
							}
						}

						win_amt++;

						std::cout << "up ingredient box\n";
						Window::player->move_up();
					}
					//Otherwise, it is a space
					else
					{
						grid_number check = static_cast<grid_number>(gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1]);

						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 2] = static_cast<int>(check);
						gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] = static_cast<int>(grid_number::player);

						//check if current grid is box1
						if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox1))
						{
							gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::box1);
						}
						//check if current grid is box2
						else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox2))
						{
							gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::box2);
						}
						else
						{
							gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);
						}

						for (auto ingredient : SceneManager::ingredientcontainer)
						{
							if (ingredient.first == check)
							{
								ingredient.second->transformation.Position.y -= tile_height;
								break;
							}
						}

						std::cout << "up ingredient space\n";
						Window::player->move_up();
					}
				}
				/*check wall*/
				else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] > static_cast<int>(wall_type::first) ||
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] < static_cast<int>(wall_type::last))
				{
					std::cout << "Wall on up" << std::endl;
					Window::player->stop();
				}
			}
			/*check with covered box*/
			else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] == static_cast<int>(grid_number::boxcover))
			{
				std::cout << "Cover on up\n";
				Window::player->stop();
			}
			/*check for sinkhole*/
			else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] == static_cast<int>(grid_number::sinkhole))
			{
				gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::space);

				std::cout << "up sinkhole\n";
				Window::player->move_up();
			}
			//Just move
			else
			{
				//Check if up tile is a box1
				if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] == static_cast<int>(grid_number::box1))
				{
					Window::player->move_up();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::inbox1);
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] = static_cast<int>(grid_number::space);
				}
				//Check if up tile is box2
				else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y - 1] == static_cast<int>(grid_number::box2))
				{
					Window::player->move_up();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::inbox2);
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] = static_cast<int>(grid_number::space);
				}
				//Check if current tile is inbox1
				else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox1))
				{
					Window::player->move_up();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] = static_cast<int>(grid_number::box1);
				}
				//Check if current tile is inbox2
				else if (gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] == static_cast<int>(grid_number::inbox2))
				{
					Window::player->move_up();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] = static_cast<int>(grid_number::box2);
				}
				else
				{
					Window::player->move_up();
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y] = static_cast<int>(grid_number::player);
					gGrids[Window::player->player_grid_pos.x][Window::player->player_grid_pos.y + 1] = static_cast<int>(grid_number::space);
				}

				std::cout << "normal up" << std::endl;
			}
		}
	}

	void Map::print_map_to_console()
	{
		std::cout << "**************************** MAP LAYOUT ************************************" << std::endl;

		for (int c = 0; c < grid_col; c++)
		{
			for (int r = 0; r < grid_row; r++)
			{
				std::cout << std::setw(4) << static_cast<char>(gGrids[r][c]) << std::setw(4);
			}
			std::cout << std::endl;
		}

		std::cout << "Goals: " << SceneManager::amt_of_win_conditions << std::endl;
		std::cout << "Current goals: " << win_amt << std::endl;

		std::cout << "****************************************************************************" << std::endl;
	}

	void Map::DrawMap()
	{
		SceneManager::drawTile();
		SceneManager::drawIngr();
		
		/*for (int c = 0; c < grid_col; c++)
		{
			for (int r = 0; r < grid_row; r++)
			{
				if (gGrids[r][c] == static_cast<char>(wall_type::bottomleftwall))
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall7_3.jpg");
					
					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, tile);
					SceneManager::drawTile();
				}
				if (gGrids[r][c] == 3)
				{
					SceneManager::loadIngr1(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height);
					SceneManager::drawIngr1();
				}
				if (gGrids[r][c] == 4)
				{
					SceneManager::loadIngr2(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height);
					SceneManager::drawIngr2();
				}
				if (gGrids[r][c] == 50)
				{
					Sprite* tile = new Sprite("../textures/Tiles/Wall/RicePlain_Wall5_2.jpg");

					SceneManager::loadTile(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height, tile);
					SceneManager::drawGoal();
				}
				if (gGrids[r][c] == static_cast<char>(wall_type::Wall5))
				{
					SceneManager::loadGoal1(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height);
					SceneManager::drawGoal1();
				}
				if (gGrids[r][c] == static_cast<char>(wall_type::Wall4))
				{
					SceneManager::loadGoal2(r / static_cast<float>(grid_row) * width, c / static_cast<float>(grid_col) * height);
					SceneManager::drawGoal2();
				}
			}
		}*/
	}
	/********************************************
	 Return the value inside a cell that you click
	********************************************/
	int Map::GetValue(int col_x, int row_y)
	{
		//if you are accessing out of the given grid
		//if (col_x > grid_col -1 || row_y > grid_row -1)
		//{
		//	return 0; //if you are pressing out of this grid, return 0 as tile value 
		//}
		return gGrids[col_x][row_y];
	}
	/********************************************
	 Set the value inside a cell that you click
	********************************************/
	void Map::SetValue(int col_x, int row_y, int value)
	{
		gGrids[col_x][row_y] = value;
		Window::loaded = false;
		print_map_to_console(); //debugging
	}
}