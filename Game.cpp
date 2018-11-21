#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#pragma warning(disable : 4996)
#include "Space.h"
#include <sstream>
#include <list>

enum Type { Single, Server, Client, Exit };

int menu(sf::RenderWindow& window)
{
	sf::Texture menuTexture1, menuTexture2, menuTexture3, menuTexture4, menuTexture5, menuBackground;
	menuTexture1.loadFromFile(MenuNS::new_game_image_path);
	menuTexture3.loadFromFile(MenuNS::exit_image_path);
	menuTexture2.loadFromFile(MenuNS::multiplayer_image_path);
	menuTexture4.loadFromFile(MenuNS::server_image_path);
	menuTexture5.loadFromFile(MenuNS::client_image_path);
	menuBackground.loadFromFile(MenuNS::background_image_path);
	sf::Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3),
		menu4(menuTexture4), menu5(menuTexture5), menuBg(menuBackground);
	bool isMult = false;
	int menuNum = 0;
	menu1.setPosition(70.f, 30.f);
	menu2.setPosition(70.f, 200.f);
	menu3.setPosition(70.f, 370.f);
	menuBg.setPosition(0.f, 0.f);

	while (true)
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();
		menu1.setColor(sf::Color::Yellow);
		menu2.setColor(sf::Color::Yellow);
		menu3.setColor(sf::Color::Yellow);
		menu4.setColor(sf::Color::Yellow);
		menu5.setColor(sf::Color::Yellow);
		menuNum = 0;
		window.clear();

		if (sf::IntRect(70.f, 30.f, 370.f, 130.f).contains(sf::Mouse::getPosition(window)))
		{
			if (isMult)
			{
				menu4.setColor(sf::Color::Red);
				menuNum = 4;
			}
			else
			{
				menu1.setColor(sf::Color::Red);
				menuNum = 1;
			}
		}
		if (sf::IntRect(70.f, 150.f, 370.f, 250.f).contains(sf::Mouse::getPosition(window)))
		{
			if (isMult)
			{
				menu5.setColor(sf::Color::Red);
				menuNum = 5;
			}
			else
			{
				menu2.setColor(sf::Color::Red);
				menuNum = 2;
			}
		}
		if (sf::IntRect(70.f, 280.f, 370.f, 370.f).contains(sf::Mouse::getPosition(window)))
		{
			menu3.setColor(sf::Color::Red);
			menuNum = 3;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			switch (menuNum)
			{
			case 1:
				return Type::Single;
			case 2:
				menu4.setPosition(70.f, 30.f);
				menu5.setPosition(70.f, 200.f);
				isMult = true;
				sf::sleep(sf::milliseconds(800));
				break;
			case 3:
				window.close();
				return Type::Exit;
			case 4:
				return Type::Server;
			case 5:
				return Type::Client;
			}
		}

		window.draw(menuBg);	
		if (isMult)
		{
			window.draw(menu4);
			window.draw(menu5);
		}
		else
		{
			window.draw(menu1);
			window.draw(menu2);
		}
		window.draw(menu3);

		window.display();
	}
}

void calculateBusinessLogic(Space& space, float& currentFrameZombie, sf::Clock& clock, sf::Time& time)
{
	Player& player = space.getPlayer();
	BulletController& bC = space.getBC();
	ZombieAI& AI = space.getAI();

	space.setElapsedTime(clock.getElapsedTime().asSeconds() - time.asSeconds());
	time = clock.getElapsedTime();

	bC.flyOfBullet(space.getElapsedTime());

	if (AI.timeToUpdateZombies())
	{
		AI.zombieGenerate(ZombieNS::amount - AI.getZombies().size());
		MapNS::heartsGenerate();
	}
	space.zombieDirection(currentFrameZombie);
	space.bulletCollisionProcessingZombie();
	space.bulletInteractionWithMap();
}

void draw(Space& space, sf::RenderWindow& window)
{
	Player& player = space.getPlayer();
	ZombieAI& AI = space.getAI();
	sf::View& camera = space.getCamera();
	camera.reset(sf::FloatRect(0, 0, 1200, 640));
	BulletController& bC = space.getBC();

	sf::Font font;
	font.loadFromFile(ExtraNS::font_path);
	sf::Text healthText("", font, 40), scoreText("", font, 40), 
		timeText("", font, 40), gameOverText("", font, 100);
	healthText.setColor(sf::Color::Red);
	healthText.setStyle(sf::Text::Bold);
	scoreText.setColor(sf::Color::Red);
	scoreText.setStyle(sf::Text::Bold);
	timeText.setColor(sf::Color::Red);
	timeText.setStyle(sf::Text::Bold);
	gameOverText.setColor(sf::Color::Red);
	gameOverText.setStyle(sf::Text::Bold);

	sf::Image mapImage;
	mapImage.loadFromFile(MapNS::image_path);
	sf::Texture mapTxt;
	mapTxt.loadFromImage(mapImage);
	sf::Sprite mapSprite(mapTxt);
	player.sprite().setPosition(player.box().getUppVertex().x(), player.box().getUppVertex().y());
	space.cameraMoving(player.box().getUppVertex().x(), player.box().getUppVertex().y());

	for (size_t i = 0; i < AI.getZombies().size(); ++i)
		AI.getZombies().at(i).sprite().setPosition(AI.getZombies().at(i).box().getUppVertex().x(), AI.getZombies().at(i).box().getUppVertex().y());

	healthText.setString("Здоровье: " + std::to_string((int)player.getHealth()));
	healthText.setPosition(camera.getCenter().x + 400.f, camera.getCenter().y - 280.f);

	scoreText.setString("Убито зомби: " + std::to_string(AI.getScore()));
	scoreText.setPosition(camera.getCenter().x - 550.f, camera.getCenter().y - 280.f);

	static sf::Time time;
	if (player.isAlive())
	{
		time = space.getGameTime();
		timeText.setString("Время игры: " + std::to_string(((int)time.asSeconds() / 60) / 10)
			+ std::to_string(((int)time.asSeconds() / 60) % 10) + ":" 
			+ std::to_string(((int)time.asSeconds() % 60) / 10) 
			+ std::to_string(((int)time.asSeconds() % 60) % 10));
		timeText.setPosition(camera.getCenter().x - 550.f, camera.getCenter().y + 200.f);
	}
	else
	{
		camera.rotate(5.f);
		gameOverText.setString("Игра окончена\nВремя игры: " + std::to_string(((int)time.asSeconds() / 60) / 10)
			+ std::to_string(((int)time.asSeconds() / 60) % 10) + ":"
			+ std::to_string(((int)time.asSeconds() % 60) / 10)
			+ std::to_string(((int)time.asSeconds() % 60) % 10));
		gameOverText.setPosition(camera.getCenter().x - 200.f, camera.getCenter().y - 50.f);
	}
	window.setView(camera);
	window.clear();
	for (size_t i = 0; i < MapNS::mapHeight; ++i)
		for (size_t j = 0; j < MapNS::mapWidth; ++j)
		{
			if (MapNS::map[i][j] == ' ')
				mapSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
			if (MapNS::map[i][j] == 'w')
				mapSprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
			if (MapNS::map[i][j] == 'h')
				mapSprite.setTextureRect(sf::IntRect(64, 0, 32, 32));
			mapSprite.setPosition(j * 32, i * 32);
			window.draw(mapSprite);
		}
	for (size_t i = 0; i < AI.getZombies().size(); ++i)
	{
		window.draw(AI.getZombies().at(i).getSprite());
		AI.getZombies().at(i).setMoved(false);
	}
	window.draw(player.sprite());
	for (size_t i = 0; i < bC.getBullets().size(); ++i)
		window.draw(bC.getBullets().at(i).getSprite());
	if (player.getStrikeCondition())
	{
		player.hurtSprite().setPosition(player.box().getUppVertex().x(), player.box().getUppVertex().y());
		window.draw(player.hurtSprite());
	}
	window.draw(healthText);
	window.draw(scoreText);
	window.draw(timeText);
	window.draw(gameOverText);
	window.display();
}


int main()
{
	sf::IpAddress IP;
	sf::TcpSocket clientsSocket;
	const size_t maxPlayers = 1;
	std::list<std::unique_ptr<sf::TcpSocket>> serversSockets;
	sf::Packet packet;

	sf::RenderWindow window(sf::VideoMode(1200, 640), "VectorGraphics");
	Space space(PlayerNS::size);

	int type = menu(window);

	if (type == Type::Exit)
		return 0;
	else if (type == Type::Server)
	{
		sf::TcpListener listener;
		listener.listen(2017);
		window.close();
		for (size_t i = 0; i < maxPlayers; ++i)
		{
			std::unique_ptr<sf::TcpSocket> socket = std::make_unique<sf::TcpSocket>();
			serversSockets.emplace_back(std::make_unique<sf::TcpSocket>());
			std::cout << "Waiting for connection from client..." << std::endl <<	
	//			serversSockets[i]->getLocalPort() << std::endl << 
				sf::IpAddress::getLocalAddress().toString() << std::endl;
	//		sf::TcpSocket socket;
			listener.accept(*(serversSockets.back()));
			std::cout << "Client connected, IP: " << serversSockets.back()->getRemoteAddress().toString() << std::endl;
			packet.append(&space, sizeof(space));
			serversSockets.back()->send(packet);
			packet.clear();
			packet.append(space.getAI().getZombies().data(), sizeof(*(space.getAI().getZombies().data())));
			std::cout << "Packing..." << std::endl << packet.getDataSize() << std::endl;
			serversSockets.back()->send(packet);
			packet.clear();
			packet.append(space.getBC().getBullets().data(), sizeof(*(space.getBC().getBullets().data())));
			std::cout << "Packing..." << std::endl << packet.getDataSize() << std::endl;
			serversSockets.back()->send(packet);
			packet.clear();
		}
	}
	else if (type == Type::Client)
	{
		window.setVisible(false);
		std::string ip;
		int port;
		std::cout << "Enter IP: ";
		std::cin >> ip;
		std::cout << std::endl << "Enter port: ";
		std::cin >> port;
		IP = sf::IpAddress(ip);
//		std::cout << IP.toString() << std::endl;
		clientsSocket.connect(IP, port);
		std::cout << "Receiving..." << std::endl;
		window.setVisible(true);
	}

	sf::Clock clock = space.getGameClock();
	sf::Time time = space.getGameTime();
	float elapsedTime = 0.f;
	float currentFramePlayer = 0.f;
	float currentFrameZombie = 0.f;
	short playerDirection = up_arrow;
	MapNS::heartsGenerate();
	space.getAI().zombieGenerate(ZombieNS::amount);

	/*
	Сервер:
	1. Получение данных о координатах игроков и фактов нажатий кнопки выстрела;
	2. Обработка данных бизнес-логики;
	3. Отправка данных класса space;

	Клиент:
	1. Получение данных об экземпляре класса space;
	2. Отрисовка объектов бизнес-логики;
	3. Отправка данных о координатах игрока и факте нажатия кнопки выстрела;
	*/
	if (type == Type::Server)
	{
		while (space.getPlayer().isAlive())
		{
			serversSockets.back()->receive(packet);
			Player player;
			bool fired;
			std::cout << "Unpacking..." << std::endl << packet.getDataSize() << std::endl;
			packet >> player >> fired;
			packet.clear();
			space.setPlayer(player);
			if (fired)
				space.getPlayer().fire();
			calculateBusinessLogic(space, currentFrameZombie, clock, time);
			packet.append(&space, sizeof(space));
			std::cout << "Packing..." << std::endl << packet.getDataSize() << std::endl;
			serversSockets.back()->send(packet);
			packet.clear();
			packet.append(space.getAI().getZombies().data(), sizeof(*(space.getAI().getZombies().data())));
			std::cout << "Packing..." << std::endl << packet.getDataSize() << std::endl;
			serversSockets.back()->send(packet);
			packet.clear();
			packet.append(space.getBC().getBullets().data(), sizeof(*(space.getBC().getBullets().data())));
			std::cout << "Packing..." << std::endl << packet.getDataSize() << std::endl;
			serversSockets.back()->send(packet);
			packet.clear();
		}
	}

	while (window.isOpen())
	{
		sf::Event event;
 		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();
		
		
		if (type == Type::Client)
		{
			sf::Packet packetBC, packetZ;
			clientsSocket.receive(packet);
			std::cout << "Unpacking..." << std::endl << packet.getDataSize() << std::endl;
			const Space * space1 = reinterpret_cast<const Space*>(packet.getData());
			std::cout << "Packet recieved" << std::endl;
			clientsSocket.receive(packetZ);
			std::cout << "Unpacking..." << std::endl << packetZ.getDataSize() << std::endl;
			const Zombie * zombie_ptr = reinterpret_cast<const Zombie*>(packetZ.getData());
			std::cout << "Packet recieved" << std::endl;
			clientsSocket.receive(packetBC);
			std::cout << "Unpacking..." << std::endl << packetBC.getDataSize() << std::endl;
			const Bullet * bullet_ptr = reinterpret_cast<const Bullet*>(packetBC.getData());
			std::cout << "Packet recieved" << std::endl;
			space = Space::getSpaceFromPtr(space1, zombie_ptr, bullet_ptr);
			packet.clear();
			draw(space, window);
			bool fired = space.inputProcessing(space, currentFramePlayer);
			packet << space.getPlayer() << fired;
			clientsSocket.send(packet);
			packet.clear();
		}
		if (type == Type::Single)
		{
			calculateBusinessLogic(space, currentFrameZombie, clock, time);
			draw(space, window);
			if (space.inputProcessing(space, currentFramePlayer))
				space.getPlayer().fire();
		}
	}
	return 0;
}
