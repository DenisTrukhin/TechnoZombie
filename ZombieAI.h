#pragma once
#include "CoreClasses.h"
#include "Constants.h"

class ZombieAI
{
public:
	ZombieAI() 
	{
		_zombieImage = sf::Image();
		_zombieImage.loadFromFile(ZombieNS::image_path);
		_zombieTxt = sf::Texture();
		_zombieTxt.loadFromImage(_zombieImage);
		_zombieScore = 0;
		_AITime = _AIClock.getElapsedTime();
	};

	ZombieAI(const ZombieAI& that)
	{
		_zombieImage = sf::Image();
		_zombieImage.loadFromFile(ZombieNS::image_path);
		_zombieTxt = sf::Texture();
		_zombieTxt.loadFromImage(_zombieImage);
		_zombieScore = that._zombieScore;
		_AIClock = that._AIClock;
		_AITime = that._AITime;
		for (size_t i = 0; i < that._zombies.size(); ++i)
			_zombies.emplace_back(Zombie(that._zombies[i].box().getUppVertex(), 
				that._zombies[i].box().getLowVertex(), _zombieTxt));
	};

	ZombieAI& operator= (const ZombieAI& that)
	{
		_zombieImage = sf::Image();
		_zombieImage.loadFromFile(ZombieNS::image_path);
		_zombieTxt = sf::Texture();
		_zombieTxt.loadFromImage(_zombieImage);
		_zombieScore = that._zombieScore;
		_AIClock = that._AIClock;
		_AITime = that._AITime;
		for (size_t i = 0; i < that._zombies.size(); ++i)
			_zombies.emplace_back(that._zombies[i].box().getUppVertex(),
				that._zombies[i].box().getLowVertex(), _zombieTxt, that._zombies[i].getTextureRect());
		return *this;
	}

	void pushZombie(const Zombie& zombie)
	{
		_zombies.emplace_back(zombie.box().getUppVertex(), 
			zombie.box().getLowVertex(), _zombieTxt, zombie.getTextureRect());
	}

	void removeZombie(size_t i)
	{
		_zombies.erase(_zombies.begin() + i);
		++_zombieScore;
	}

	void zombieGenerate(size_t amount) 
	{
		int i = 0, j = 0;
		srand(time(0));
		while (amount > 0) 
		{
			i = 1 + rand() % (MapNS::mapHeight - 1);
			j = 1 + rand() % (MapNS::mapWidth - 1);
			if (MapNS::map[i][j] == ' ' && 
				MapNS::map[(i * 32 + (int)ZombieNS::size.x()) / 32][(j * 32 + (int)ZombieNS::size.y()) / 32] == ' ' && 
				MapNS::map[(i * 32 + ((int)ZombieNS::size.x() / 2)) / 32][(j * 32 + ((int)ZombieNS::size.y() / 2)) / 32] == ' ')
			{ 
				_zombies.emplace_back(ZombieNS::position(j, i), ZombieNS::position(j, i) + ZombieNS::size, _zombieTxt);
				amount--;
			}
		}
		_AITime = _AIClock.getElapsedTime();
	}

	bool timeToUpdateZombies()
	{
		return _AIClock.getElapsedTime().asSeconds() >= _AITime.asSeconds() + ZombieNS::time_to_update_zombies;
	}

	const std::vector<Zombie>& getZombies() const { return _zombies; }
	std::vector<Zombie>& getZombies() { return _zombies; }
	size_t getScore() { return _zombieScore; }

	friend sf::Packet& operator >> (sf::Packet& packet, ZombieAI& AI)
	{
		size_t AISize;
		packet >> AISize;
		for (size_t i = 0; i < AISize; ++i)
		{
	//		Zombie zombie;
	//		packet >> zombie;
	//		AI._zombies.push_back(zombie);
		}
		return packet;
	}

	friend sf::Packet& operator << (sf::Packet& packet, const ZombieAI& AI)
	{
		packet << AI._zombies.size();
		for (size_t i = 0; i < AI._zombies.size(); ++i)
			packet << AI._zombies.at(i);
		return packet;
	}
private:
	std::vector<Zombie> _zombies;
	size_t _zombieScore;
	sf::Image _zombieImage;
	sf::Texture _zombieTxt;
	sf::Clock _AIClock;
	sf::Time _AITime;
};
