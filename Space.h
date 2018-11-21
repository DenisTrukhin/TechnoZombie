#pragma once
#include "CoreClasses.h"
#include "ZombieAI.h"


Point2D player_initial_position = { 300.f, 400.f };

class Space
{
public:
	Space() = default;
	Space(const Point2D& player_size)
		: _bulletC(), _player(_bulletC, player_initial_position, player_initial_position + player_size)
	{};

	Space& operator= (const Space& that)
	{
		_bulletC = BulletController();
		for (size_t i = 0; i < that._bulletC.getBullets().size(); ++i)
			_bulletC.pushBullet(that._bulletC.getBullets()[i]);
		_AI = ZombieAI();
		for (size_t i = 0; i < that._AI.getZombies().size(); ++i)
			_AI.pushZombie(that._AI.getZombies()[i]);
		_player = Player(_bulletC, that._player.box().getUppVertex(),
			that._player.box().getLowVertex(), that._player.getTextureRect(),
			that._player.getPlayerDirection());
		_camera = that._camera;
		_gameClock = that._gameClock;
		return *this;
	};

	static Space getSpaceFromPtr(const Space* that, const Zombie* zombie_ptr, const Bullet* bullet_ptr)
	{
		Space space;
		space._bulletC = BulletController();
		for (size_t i = 0; i < that->_bulletC.getBullets().size(); ++i)
			space._bulletC.pushBullet(bullet_ptr[i]);
		space._AI = ZombieAI();
		for (size_t i = 0; i < that->_AI.getZombies().size(); ++i)
			space._AI.pushZombie(zombie_ptr[i]);
		space._player = Player(space._bulletC, that->_player.box().getUppVertex(), 
			that->_player.box().getLowVertex(), that->_player.getTextureRect(),
			that->_player.getPlayerDirection());
		space._camera = that->_camera;
		space._gameClock = that->_gameClock;
		return space;
	};

	void playerCollisionProcessing(SatArray& side_of_barrier, float& elapsedTime)
	{
		if (side_of_barrier->at(top))
			_player.move({ 0.f, _player.getSpeed().y() * elapsedTime + 1 });
		if (side_of_barrier->at(right))
			_player.move({ _player.getSpeed().x() * elapsedTime * -1 - 1, 0.f });
		if (side_of_barrier->at(bottom))
			_player.move({ 0.f, _player.getSpeed().y() * elapsedTime * -1 - 1 });
		if (side_of_barrier->at(left))
			_player.move({ _player.getSpeed().x() * elapsedTime + 1, 0.f });
	}

	void playerInteractionWithMap(DirectionsArray& pushed_moving_buttons)
	{

		for (size_t i = _player.box().getUppVertex().y() / 32; i < (_player.box().getLowVertex().y()) / 32; ++i)
			for (size_t j = _player.box().getUppVertex().x() / 32; j < (_player.box().getLowVertex().x()) / 32; ++j)
			{
				if (MapNS::map[i][j] == 'w')
				{
					Box2D wall = { j * 32.f, i * 32.f, j * 32.f + 32.f, i * 32.f + 32.f };
					SatArray& side_of_wall = _player.box().isBoxCrossesBox(wall);
					if (side_of_wall->at(top))
						_player.move({ 0.f, side_of_wall->at(top) });
					else if (side_of_wall->at(bottom))
						_player.move({ 0.f, side_of_wall->at(bottom) * -1.f });
					else if (side_of_wall->at(left))
						_player.move({ side_of_wall->at(left), 0.f });
					else if (side_of_wall->at(right))
						_player.move({ side_of_wall->at(right) * -1.f, 0.f });
				}

				if (MapNS::map[i][j] == 'h')
				{
					if (_player.getHealth() <= 80.f)
					{
						_player.setHealth(_player.getHealth() + 20.f);
						MapNS::map[i][j] = ' ';
						--MapNS::hearts_amount;
					}
					else if (_player.getHealth() < 100.f && _player.getHealth() > 80.f)
					{
						_player.setHealth(_player.getHealth() + (100.f - _player.getHealth()));
						MapNS::map[i][j] = ' ';
					}
				}
			}
	}

	void playerMoving(DirectionsArray& pushed_moving_buttons, float& currentFrame)
	{
		playerInteractionWithMap(pushed_moving_buttons);
		if (pushed_moving_buttons->at(up_arrow))
		{
			currentFrame += 0.1f;
			_player.setTextureRect(sf::IntRect((int)currentFrame * 55, 65, 55, 65));
			for (size_t i = 0; i < _AI.getZombies().size(); ++i)
				playerCollisionProcessing(_player.box().isBoxCrossesBox(_AI.getZombies().at(i).box()), _elapsedTime);
			_player.move({ 0.f, _player.getSpeed().y() * _elapsedTime * -1.f });
			if (currentFrame >= PlayerNS::next_frame)
				currentFrame = 0.f;
		}
		if (pushed_moving_buttons->at(right_arrow))
		{
			currentFrame += 0.1f;
			_player.setTextureRect(sf::IntRect((int)currentFrame * 55, 260, 55, 65));
			for (size_t i = 0; i < _AI.getZombies().size(); ++i)
				playerCollisionProcessing(_player.box().isBoxCrossesBox(_AI.getZombies().at(i).box()), _elapsedTime);
			_player.move({ _player.getSpeed().x() * _elapsedTime, 0.f });
			if (currentFrame >= PlayerNS::next_frame)
				currentFrame = 0.f;
		}
		if (pushed_moving_buttons->at(down_arrow))
		{
			currentFrame += 0.1f;
			_player.setTextureRect(sf::IntRect((int)currentFrame * 55, 390, 55, 65));
			for (size_t i = 0; i < _AI.getZombies().size(); ++i)
				playerCollisionProcessing(_player.box().isBoxCrossesBox(_AI.getZombies().at(i).box()), _elapsedTime);
			_player.move({ 0.f, _player.getSpeed().y() * _elapsedTime });
			if (currentFrame >= PlayerNS::next_frame)
				currentFrame = 0.f;
		}
		if (pushed_moving_buttons->at(left_arrow))
		{
			currentFrame += 0.1f;
			_player.setTextureRect(sf::IntRect((int)currentFrame * 55, 195, 55, 65));
			for (size_t i = 0; i < _AI.getZombies().size(); ++i)
				playerCollisionProcessing(_player.box().isBoxCrossesBox(_AI.getZombies().at(i).box()), _elapsedTime);
			_player.move({ _player.getSpeed().x() * _elapsedTime * -1.f, 0.f });
			if (currentFrame >= PlayerNS::next_frame)
				currentFrame = 0.f;
		}
		if (pushed_moving_buttons->at(up_left_arrows))
		{
			currentFrame += 0.1f;
			_player.setTextureRect(sf::IntRect((int)currentFrame * 55, 130, 55, 65));
			if (currentFrame >= PlayerNS::next_frame)
				currentFrame = 0.f;
		}
		if (pushed_moving_buttons->at(right_down_arrows))
		{
			currentFrame += 0.1f;
			_player.setTextureRect(sf::IntRect((int)currentFrame * 55, 325, 55, 65));
			if (currentFrame >= PlayerNS::next_frame)
				currentFrame = 0.f;
		}
		if (pushed_moving_buttons->at(left_down_arrows))
		{
			currentFrame += 0.1f;
			_player.setTextureRect(sf::IntRect((int)currentFrame * 55, 455, 55, 65));
			if (currentFrame >= PlayerNS::next_frame)
				currentFrame = 0.f;
		}
		if (pushed_moving_buttons->at(up_right_arrows))
		{
			currentFrame += 0.1f;
			_player.setTextureRect(sf::IntRect((int)currentFrame * 55, 0, 55, 65));
			if (currentFrame >= PlayerNS::next_frame)
				currentFrame = 0.f;
		}
		cameraMoving(_player.box().getUppVertex().x(), _player.box().getUppVertex().y());
	}

	void zombieCollisionProcessing(SatArray& side_of_barrier, Point2D& vector, float speed, size_t i)
	{
		if (side_of_barrier->at(top))
			_AI.getZombies().at(i).move({ 0.f, side_of_barrier->at(top) });
		else if (side_of_barrier->at(bottom))
			_AI.getZombies().at(i).move({ 0.f, side_of_barrier->at(bottom) * -1.f });
		if (side_of_barrier->at(right))
			_AI.getZombies().at(i).move({ side_of_barrier->at(right) * -1.f, 0.f });
		else if (side_of_barrier->at(left))
			_AI.getZombies().at(i).move({ side_of_barrier->at(left), 0.f });
		if (!_AI.getZombies().at(i).getMoved())
		{
			_AI.getZombies().at(i).move(vector * speed);
			_AI.getZombies().at(i).setMoved(true);
		}
	}

	void zombieInteractionWithMap(size_t n)
	{
		const Box2D& zBox = _AI.getZombies().at(n).box();
		for (size_t i = zBox.getUppVertex().y() / 32; i < (zBox.getLowVertex().y()) / 32; ++i)
			for (size_t j = zBox.getUppVertex().x() / 32; j < (zBox.getLowVertex().x()) / 32; ++j)
			{
				if (MapNS::map[i][j] == 'w')
				{
					Box2D wall = { j * 32.f, i * 32.f, j * 32.f + 32.f, i * 32.f + 32.f };
					SatArray& side_of_wall = zBox.isBoxCrossesBox(wall);
					if (side_of_wall->at(top))
						_AI.getZombies().at(n).move({ 0.f, side_of_wall->at(top) });
					else if (side_of_wall->at(bottom))
						_AI.getZombies().at(n).move({ 0.f, side_of_wall->at(bottom) * -1.f });
					else if (side_of_wall->at(left))
						_AI.getZombies().at(n).move({ side_of_wall->at(left), 0.f });
					else if (side_of_wall->at(right))
						_AI.getZombies().at(n).move({ side_of_wall->at(right) * -1.f, 0.f });
				}
			}
	}

	void zombieProcessing(Point2D& vector, float speed, float elapsedTime, size_t i, float& currentFrameZombie)
	{
		Zombie& zombie = _AI.getZombies().at(i);
		const Box2D& zBox = zombie.box();
		DirectionsArray directionToPlayer = zBox.isPointInSector(_player.box());
		if (zBox.isBoxNear(_player.box()))
		{
			zombieCollisionProcessing(zBox.isBoxCrossesBox(_player.box()), vector, speed, i);
			zombieInteractionWithMap(i);
			for (size_t j = i + 1; j < _AI.getZombies().size(); ++j)
				zombieCollisionProcessing(zBox.isBoxCrossesBox(_AI.getZombies().at(j).box()), vector, speed, i);
			if (isPlayerAtArmsLength(i) && zombie.strikeInterval() && _player.isAlive())
			{
				_AI.getZombies().at(i).strike();
				_player.hurt(_AI.getZombies().at(i).getPower());
			}
			else
			{
				if (_player.getStrikeCondition())
				{
					if (zombie.getClock().getElapsedTime().asSeconds() <=
						zombie.getTime().asSeconds() + ZombieNS::interval / 2.f)
						zombie.animationForZombieStrike(currentFrameZombie, directionToPlayer);
					else
						_player.setStrikeCondition(false);
				}
				else
				{
					currentFrameZombie += 0.07f;
					if (directionToPlayer->at(top) == true)
					{
						if (isPlayerAtArmsLength(i))
							currentFrameZombie = 0.f;
						zombie.sprite().setTextureRect(sf::IntRect((int)currentFrameZombie * 32, 144, 32, 48));
					}
					if (directionToPlayer->at(right) == true)
					{
						if (isPlayerAtArmsLength(i))
							currentFrameZombie = 0.f;
						zombie.sprite().setTextureRect(sf::IntRect((int)currentFrameZombie * 32, 96, 32, 48));
					}
					if (directionToPlayer->at(bottom) == true)
					{
						if (isPlayerAtArmsLength(i))
							currentFrameZombie = 0.f;
						zombie.sprite().setTextureRect(sf::IntRect((int)currentFrameZombie * 32, 0, 32, 48));
					}
					if (directionToPlayer->at(left) == true)
					{
						if (isPlayerAtArmsLength(i))
							currentFrameZombie = 0.f;
						zombie.sprite().setTextureRect(sf::IntRect((int)currentFrameZombie * 32, 48, 32, 48));
					}
					if (currentFrameZombie >= ZombieNS::next_frame)
						currentFrameZombie = 0.f;
				}
			}
		}
	}

	bool isPlayerAtArmsLength(size_t i)
	{
		return _AI.getZombies().at(i).box().distanceToOtherBox(_player.box()) <= 80.f;
	}

	void bulletInteractionWithMap()
	{
		for (size_t n = 0; n < _bulletC.getBullets().size(); ++n)
			for (size_t i = _bulletC.getBullets().at(n).box().getUppVertex().y() / 32; i < (_bulletC.getBullets().at(n).box().getLowVertex().y()) / 32; ++i)
			{
				for (size_t j = _bulletC.getBullets().at(n).box().getUppVertex().x() / 32; j < (_bulletC.getBullets().at(n).box().getLowVertex().x()) / 32; ++j)
				{
					if (MapNS::map[i][j] == 'w')
					{
						_bulletC.remove(n);
						break;
					}
				}
				break;
			}
	}

	void bulletCollisionProcessingZombie()
	{
		for (size_t i = 0; i < _bulletC.getBullets().size(); ++i)
		{
			for (size_t j = 0; j < _AI.getZombies().size(); ++j)
			{
				if (_bulletC.getBullets().at(i).box().isBoxCrossesBoxBool(_AI.getZombies().at(j).box()))
				{
					_bulletC.remove(i);
					_AI.getZombies().at(j).hurt(_player.getPower());
					if (_AI.getZombies().at(j).getHealth() <= 0.f)
					{
						_AI.removeZombie(j);
						_player.setStrikeCondition(false);
					}
					break;
				}
			}
		}
	}

	void zombieDirection(float& currentFrameZombie)
	{
		float speed = _elapsedTime * ZombieNS::speed_f;
 		for (size_t i = 0; i < _AI.getZombies().size(); ++i)
		{
			Point2D vectorX = { _AI.getZombies().at(i).box().otherBoxCenterWidth(_player.box()), 0.f };
			Point2D vectorY = { 0.f, _AI.getZombies().at(i).box().otherBoxCenterHeight(_player.box()) };
			Point2D vector = vectorX + vectorY;
			vector.normalize();
			zombieProcessing(vector, speed, _elapsedTime, i, currentFrameZombie);
		}
	}

	sf::View cameraMoving(float x, float y) 
	{ 
		_camera.setCenter(x + 290, y); 
		return _camera; 
	}

	bool inputProcessing(Space& space, float& currentFramePlayer)
	{
		bool fired = false;
		if (_player.isAlive())
		{
			DirectionsArray pushed_moving_buttons = std::make_unique<std::array<bool, 8>>();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				pushed_moving_buttons->at(up_arrow) = true;
				_player.setPlayerDirection(up_arrow);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				pushed_moving_buttons->at(right_arrow) = true;
				_player.setPlayerDirection(right_arrow);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				pushed_moving_buttons->at(down_arrow) = true;
				_player.setPlayerDirection(down_arrow);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				pushed_moving_buttons->at(left_arrow) = true;
				_player.setPlayerDirection(left_arrow);
			}
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				pushed_moving_buttons->at(up_left_arrows) = true;
				_player.setPlayerDirection(up_left_arrows);
			}
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				pushed_moving_buttons->at(up_right_arrows) = true;
				_player.setPlayerDirection(up_right_arrows);
			}
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				pushed_moving_buttons->at(left_down_arrows) = true;
				_player.setPlayerDirection(left_down_arrows);
			}
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				pushed_moving_buttons->at(right_down_arrows) = true;
				_player.setPlayerDirection(right_down_arrows);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				fired = true;
			}
			if (pushed_moving_buttons)
				space.playerMoving(pushed_moving_buttons, currentFramePlayer);
		}
		return fired;
	}


	Player& getPlayer() { return _player; }
	void setPlayer(Player& player) { _player = player; }
	ZombieAI& getAI() { return _AI; }
	BulletController& getBC() { return _bulletC; }
	sf::View& getCamera() { return _camera; }
	sf::Clock& getGameClock() { return _gameClock; }
	sf::Time getGameTime() { return _gameClock.getElapsedTime(); }
	float getElapsedTime() { return _elapsedTime; }
	void setElapsedTime(float elapsedTime) { _elapsedTime = elapsedTime; }

	friend sf::Packet& operator >> (sf::Packet& packet, Space& space)
	{
		return packet >> space._bulletC >> space._player >> space._AI >> space._elapsedTime;
	}
	friend sf::Packet& operator << (sf::Packet& packet, const Space& space)
	{
		return packet << space._bulletC << space._player << space._AI << space._elapsedTime;
	}
private:
	BulletController _bulletC;
	Player _player;
	sf::View _camera;
	ZombieAI _AI;
	sf::Clock _gameClock;
	float _elapsedTime;
};
