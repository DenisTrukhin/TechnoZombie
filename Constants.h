#pragma once
#include "Box2D.h"
#include "CoreClasses.h"

namespace ZombieNS
{
	const Point2D size = { 32.f, 48.f };
	const Point2D speed = { 100.f, 100.f };
	const float speed_f = 100.f;
	const std::string image_path = "C:\\Users\\denis\\Programs\\Visual Studio\\projects\\TechnoAtom\\GameGraphics\\Debug\\zombie.png";
	const Point2D position(int i, int j) { return { i * 32.f, j * 32.f }; }
	const float next_frame = 3.98f;
	const float next_strike_frame = 1.98f;
	const float interval = 1.5f;
	const float rest_time = 1.05f;
	const int amount = 30;
	const float time_to_update_zombies = 30.f;
}

namespace BulletNS
{
	const Point2D speed_up = { 0.f, -1000.f };
	const Point2D speed_up_right = { 1000.f, -1000.f };
	const Point2D speed_right = { 1000.f, 0.f };
	const Point2D speed_right_down = { 1000.f, 1000.f };
	const Point2D speed_down = { 0.f, 1000.f };
	const Point2D speed_left_down = { -1000.f, 1000.f };
	const Point2D speed_left = { -1000.f, 0.f };
	const Point2D speed_up_left = { -1000.f, -1000.f };
	const Point2D size = { 20.f, 20.f };
	const std::string image_path = "C:\\Users\\denis\\Programs\\Visual Studio\\projects\\TechnoAtom\\GameGraphics\\Debug\\bullet.png";
}

namespace PlayerNS
{
	const Point2D size = { 55.f, 65.f };
	const Point2D speed = { 220.f, 220.f };
	const std::string image_path = "C:\\Users\\denis\\Programs\\Visual Studio\\projects\\TechnoAtom\\GameGraphics\\Debug\\moving(copy).png";
	const std::string image_hurt_path = "C:\\Users\\denis\\Programs\\Visual Studio\\projects\\TechnoAtom\\GameGraphics\\Debug\\hurt.png";
	const float next_frame = 7.99f;
}

namespace GunNS
{
	const float interval = 0.3f;
}

namespace MapNS
{
	const std::string image_path = "C:\\Users\\denis\\Programs\\Visual Studio\\projects\\TechnoAtom\\GameGraphics\\Debug\\background_text.png";
	const int mapHeight = 50;
	const int mapWidth = 100;
	const Point2D tileSize = {32.f, 32.f};
	const  Point2D mapSize = { 3200.f, 1600.f };
	sf::String map[mapHeight] =
	{
		"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w          wwwwwwwwwwwwwwwwwwww                                       wwwwwwwwwwwwwwwwwwww         w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w          w                        wwwwwwwwwwwwwwwwwwwwwwwwwwwww                        w         w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w                 w                           w         w                          w               w",
		"w                 w                          w           w                         w               w",
		"w                 w                         w             w                        w               w",
		"w                 w                        w               w                       w               w",
		"w                 w                                                                w               w",
		"w                 w                                                                w               w",
		"w                 w                                                                w               w",
		"w                 w                        w               w                       w               w",
		"w                 w                         w             w                        w               w",
		"w                 w                          w           w                         w               w",
		"w                 w                           w         w                          w               w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w          w                       wwwwwwwwwwwwwwwwwwwwwwwwwwwww                         w         w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w          w                                                                             w         w",
		"w          wwwwwwwwwwwwwwwwwwww                                       wwwwwwwwwwwwwwwwwwww         w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"w                                                                                                  w",
		"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
	};
	int hearts_amount = 0;
	void heartsGenerate()
	{
		int i = 0, j = 0, amount = 10;
		srand(time(0));
		while (amount > 0 && hearts_amount < 10)
		{
			i = 1 + rand() % (mapHeight - 1);
			j = 1 + rand() % (mapWidth - 1);
			if (map[i][j] == ' ')
			{
				map[i][j] = 'h';
				--amount;
				++hearts_amount;
			}
		}
	}
}

namespace MenuNS
{
	const std::string new_game_image_path = "C:\\Users\\denis\\Programs\\Visual Studio\\projects\\TechnoAtom\\GameGraphics\\Debug\\menu1.png";
	const std::string exit_image_path = "C:\\Users\\denis\\Programs\\Visual Studio\\projects\\TechnoAtom\\GameGraphics\\Debug\\menu2.png";
	const std::string multiplayer_image_path = "C:\\Users\\denis\\Programs\\Visual Studio\\projects\\TechnoAtom\\GameGraphics\\Debug\\menu3.png";
	const std::string server_image_path = "C:\\Users\\denis\\Programs\\Visual Studio\\projects\\TechnoAtom\\GameGraphics\\Debug\\menu4.png";
	const std::string client_image_path = "C:\\Users\\denis\\Programs\\Visual Studio\\projects\\TechnoAtom\\GameGraphics\\Debug\\menu5.png";
	const std::string background_image_path = "C:\\Users\\denis\\Programs\\Visual Studio\\projects\\TechnoAtom\\GameGraphics\\Debug\\menu_background.jpg";
}

namespace ExtraNS
{
	const std::string font_path = "C:\\Users\\denis\\Programs\\Visual Studio\\projects\\TechnoAtom\\GameGraphics\\Debug\\font.ttf";
}