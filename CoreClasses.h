#pragma once
#include "Box2D.h"
#include "Constants.h"
short up_arrow = 0;
short right_arrow = 1;
short down_arrow = 2;
short left_arrow = 3;
short up_right_arrows = 4;
short right_down_arrows = 5;
short left_down_arrows = 6;
short up_left_arrows = 7;

class GameObject
{
public:
	void move(const Point2D& offset) { _box += offset; }
	const Box2D& box() const { return _box; }
	const Point2D& getSpeed() const { return _speed; }
	void setBox(Box2D box) { _box = box; }
protected:
	GameObject() = default;
	GameObject(const Point2D& upp, const Point2D& low, const Point2D& speed)
		: _speed(speed), _box(low, upp)
	{};
	GameObject(const Box2D& box, const Point2D& speed)
		: _speed(speed), _box(box) 
	{};
	Point2D _speed;
	Box2D _box;
};

class Bullet : public GameObject
{
public:
	Bullet() = default;
	Bullet(const Box2D& box, const Point2D& speed, const sf::Texture& bulletTxt)
		: GameObject(box, speed)
	{
		_bulletSprite = sf::Sprite(bulletTxt);
	}
	Bullet(const Point2D& center, short playerDirection, sf::Texture& bulletTxt)
		: GameObject(Point2D(), Point2D(), Point2D())
	{
		if (playerDirection == up_arrow)
		{
			_speed = BulletNS::speed_up;
			_box = { Point2D{ center.x(), center.y() - PlayerNS::size.y() / 2.f }, 
				Point2D{ center.x() + BulletNS::size.x() , (center.y() - PlayerNS::size.y() / 2.f) + BulletNS::size.y() } };
		}
		if (playerDirection == right_arrow)
		{
			_speed = BulletNS::speed_right;
			_box = { Point2D{ center.x(), center.y() }, Point2D{ center.x(), center.y() } };
		}
		if (playerDirection == down_arrow)
		{
			_speed = BulletNS::speed_down;
			_box = { Point2D{ center.x(), center.y() }, Point2D{ center.x(), center.y() } };
		}
		if (playerDirection == left_arrow)
		{
			_speed = BulletNS::speed_left;
			_box = { Point2D{ center.x(), center.y() }, Point2D{ center.x(),  center.y() } };
		}
		if (playerDirection == up_left_arrows)
		{
			_speed = BulletNS::speed_up_left;
			_box = { Point2D{ center.x(), center.y() }, Point2D{ center.x(),  center.y() } };
		}
		if (playerDirection == up_right_arrows)
		{
			_speed = BulletNS::speed_up_right;
			_box = { Point2D{ center.x(), center.y() }, Point2D{ center.x(),  center.y() } };
		}
		if (playerDirection == right_down_arrows)
		{
			_speed = BulletNS::speed_right_down;
			_box = { Point2D{ center.x(), center.y() }, Point2D{ center.x(),  center.y() } };
		}
		if (playerDirection == left_down_arrows)
		{
			_speed = BulletNS::speed_left_down;
			_box = { Point2D{ center.x(), center.y() }, Point2D{ center.x(),  center.y() } };
		}
		_bulletSprite = sf::Sprite(bulletTxt);
	};

	Bullet(const Bullet& that)
		: GameObject(that._box.getLowVertex(), that._box.getUppVertex(), that._speed)
	{
	//	std::cout << "I'm bullet's copy constructor! I've been called!" << std::endl;
		_bulletSprite = sf::Sprite(*(that.getSprite().getTexture()));
		_bulletSprite.setPosition(_box.getUppVertex().x(), _box.getUppVertex().y());
	};

	const Bullet& operator= (const Bullet& that)
	{
	//	std::cout << "I'm bullet's operator =! I've been called!" << std::endl;
		_box = that._box;
		_speed = that._speed;
		_bulletSprite = sf::Sprite(*(that.getSprite().getTexture()));
		_bulletSprite.setPosition(_box.getUppVertex().x(), _box.getUppVertex().y());
		return *this;
	}

	const sf::Sprite& getSprite() const { return _bulletSprite; }
	sf::Sprite& setSprite() { return _bulletSprite; }

	friend sf::Packet& operator >> (sf::Packet& packet, Bullet& bullet)
	{
		return packet >> bullet._box >> bullet._speed;
	}
	friend sf::Packet& operator << (sf::Packet& packet, const Bullet& bullet)
	{
		return packet << bullet._box << bullet._speed;
	}
private:
	sf::Sprite _bulletSprite;
};

class BulletController
{
public:
	BulletController()
	{
		_bulletImage = sf::Image();
		_bulletImage.loadFromFile(BulletNS::image_path);
		_bulletTxt = sf::Texture();
		_bulletTxt.loadFromImage(_bulletImage);
		_bullets = std::vector<Bullet>();
	};
	BulletController(const BulletController& that)
	{
		_bulletImage = sf::Image();
		_bulletImage.loadFromFile(PlayerNS::image_path);
		_bulletTxt = sf::Texture();
		_bulletTxt.loadFromImage(_bulletImage);
		for (size_t i = 0; i < that._bullets.size(); ++i)
			_bullets.emplace_back(that._bullets[i].box(), that._bullets[i].getSpeed(), _bulletTxt);
	};
	BulletController& operator= (const BulletController& that)
	{
	//	_bulletImage = sf::Image();
	//	_bulletImage.loadFromFile(PlayerNS::image_path);
		_bulletTxt = sf::Texture();
	//	_bulletTxt.loadFromImage(_bulletImage);
		for (size_t i = 0; i < that._bullets.size(); ++i)
			_bullets.emplace_back(that._bullets[i].box(), that._bullets[i].getSpeed(), _bulletTxt);
		return *this;
	}
	void pushBullet(const Point2D& center, short playerDirection)
	{
		_bullets.emplace_back(center, playerDirection, _bulletTxt);
	}
	void pushBullet(const Bullet& bullet)
	{
		_bullets.emplace_back(bullet.box(), bullet.getSpeed(), _bulletTxt);
	}
	void flyOfBullet(float elapsedTime)
	{
		for (size_t n = 0; n < _bullets.size(); ++n)
		{
			Point2D speed = _bullets.at(n).getSpeed();
			_bullets.at(n).move(speed * elapsedTime);
			_bullets.at(n).setSprite().setPosition(_bullets.at(n).box().getUppVertex().x(), _bullets.at(n).box().getUppVertex().y());
			if (_bullets.at(n).box().getUppVertex().x() < 0 ||
				_bullets.at(n).box().getUppVertex().y() < 0 ||
				_bullets.at(n).box().getLowVertex().x() > MapNS::mapSize.x() ||
				_bullets.at(n).box().getLowVertex().y() > MapNS::mapSize.y())
					_bullets.erase(_bullets.begin() + n);
		}
	}

	void remove(size_t i)
	{
		
		_bullets.erase(_bullets.begin() + i);
		std::cout << "Bullet deleted" << std::endl;
	}

	const std::vector<Bullet>& getBullets() const
	{
		return _bullets;
	}

	friend sf::Packet& operator >> (sf::Packet& packet, BulletController& bC)
	{
		size_t bCsize;
		packet >> bCsize;
		for (size_t i = 0; i < bCsize; ++i)
		{
	//		Bullet bullet;
	//		packet >> bullet;
	//		bC._bullets.push_back(bullet);
		}
		return packet;
	}
	friend sf::Packet& operator << (sf::Packet& packet, const BulletController& bC)
	{
		packet << bC._bullets.size();
		for (size_t i = 0; i < bC._bullets.size(); ++i)
			packet << bC._bullets.at(i);
		return packet;
	}

private:
	std::vector<Bullet> _bullets;
	sf::Image _bulletImage;
	sf::Texture _bulletTxt;
};

class Zombie : public GameObject
{
public:

	void strike()
	{
		if (strikeInterval())
			_time = _zombieClock.getElapsedTime();
	}

	void animationForZombieStrike(float& currentFrame, DirectionsArray& directionToPlayer)
	{
		currentFrame += 0.005f;
	if (directionToPlayer->at(top) == true)
		_zombieSprite.setTextureRect(sf::IntRect((int)currentFrame * 32, 336, 32, 48));
	if (directionToPlayer->at(right) == true)
		_zombieSprite.setTextureRect(sf::IntRect((int)currentFrame * 32, 288, 32, 48));
	if (directionToPlayer->at(bottom) == true)
		_zombieSprite.setTextureRect(sf::IntRect((int)currentFrame * 32, 192, 32, 48));
	if (directionToPlayer->at(left) == true)
		_zombieSprite.setTextureRect(sf::IntRect((int)currentFrame * 32, 240, 32, 48));
	if (currentFrame >= ZombieNS::next_strike_frame)
		currentFrame = 0.f;
	}

	void hurt(float playerPower) { _health -= playerPower / 100.f * (100.f - _armor); }

	Zombie() = default;

	Zombie(const Point2D& upp, const Point2D& low, sf::Texture& zombieTxt, 
		sf::IntRect textureRect = sf::IntRect(0, 0, 32, 48))
		: GameObject(low, upp, ZombieNS::speed), _armor(15.f), _power(20.f), _health(120.f)
	{
		_zombieSprite = sf::Sprite(zombieTxt);
		_textureRect = textureRect;
		setTextureRect(_textureRect);
		sf::Vector2f scale = _zombieSprite.getScale();
		_zombieSprite.setScale(scale.x * 1.8f, scale.y * 1.8f);
		_box = _box * 1.8f;
		_time = _zombieClock.getElapsedTime();
		_moved = false;
	};

	Zombie(const Zombie& that)
		: GameObject(that._box.getLowVertex(), that._box.getUppVertex(), ZombieNS::speed), _armor(that._armor), _power(that._power), _health(that._health)
	{
		//std::cout << "I'm zombie's copy constructor ! I've been called!" << std::endl;
		_zombieSprite = sf::Sprite(*(that.getSprite().getTexture()));
		setTextureRect(that._textureRect);
		sf::Vector2f scale = _zombieSprite.getScale();
		_zombieSprite.setScale(scale.x * 1.8f, scale.y * 1.8f);
	};

	const Zombie& operator= (const Zombie& that)
	{
	//	std::cout << "I'm zombie's operator = ! I've been called!" << std::endl;
		_box = that._box;
		_speed = ZombieNS::speed;
		_armor = that._armor;
		_power = that._power;
		_health = that._health;
		_time = that._time;
		_zombieSprite = sf::Sprite(*(that.getSprite().getTexture()));
		setTextureRect(that._textureRect);
		sf::Vector2f scale = _zombieSprite.getScale();
		_zombieSprite.setScale(scale.x * 1.8f, scale.y * 1.8f);
		return *this;
	}

	~Zombie() {};

	bool strikeInterval() 
	{ 
		return _zombieClock.getElapsedTime().asSeconds() >= _time.asSeconds() + ZombieNS::interval; 
	}

	void setTime(sf::Time time) { _time = time; }
	const sf::Time& getTime() const { return _time; }
	const sf::Clock& getClock() const { return _zombieClock; }
	float getArmor() { return _armor; }
	float getPower() { return _power; }
	float getHealth() { return _health; }
	const sf::Sprite& getSprite() const { return _zombieSprite; } 
	void setSprite(sf::Sprite zombieSprite) { _zombieSprite = zombieSprite; }
	sf::Sprite& sprite() { return _zombieSprite; }
	bool getMoved() { return _moved; }
	void setMoved(bool moved) { _moved = moved; }

	friend sf::Packet& operator >> (sf::Packet& packet, Zombie& zombie)
	{
		return packet >> zombie._armor >> zombie._box >> zombie._health >>
			zombie._moved >> zombie._power >> zombie._speed;
	}
	friend sf::Packet& operator << (sf::Packet& packet, const Zombie& zombie)
	{
		return packet << zombie._armor << zombie._box << zombie._health <<
			zombie._moved << zombie._power << zombie._speed;
	}

	const sf::IntRect& getTextureRect() const { return _textureRect; }
	void setTextureRect(sf::IntRect textureRect)
	{
		_textureRect = textureRect;
		_zombieSprite.setTextureRect(_textureRect);
	}
private:
	float _armor;
	float _power;
	float _health;
	sf::Sprite _zombieSprite;
	sf::Clock _zombieClock;
	sf::Time _time;
	bool _moved;
	sf::IntRect _textureRect;
};

class Gun
{
public:
	Gun()
		:_bulletC(BulletController())
	{};
	Gun(BulletController& bulletC)
		: _bulletC(bulletC)
	{
		_time = _bulletClock.getElapsedTime();
	};
	Gun& operator= (const Gun& that)
	{
		_bulletC = BulletController();
		for (size_t i = 0; i < that._bulletC.getBullets().size(); ++i)
			_bulletC.pushBullet(that._bulletC.getBullets()[i]);
		_bulletClock = that._bulletClock;
		_time = that._time;
		return *this;
	}

	void shot(const Point2D& center, short playerDirection)
	{
		if (_bulletClock.getElapsedTime().asSeconds() >= _time.asSeconds() + GunNS::interval)
		{
			_bulletC.pushBullet(center, playerDirection);
			_time = _bulletClock.getElapsedTime();
		}
	}

	BulletController& getBC() const { return _bulletC; }

	friend sf::Packet& operator >> (sf::Packet& packet, Gun& gun)
	{
		return packet >> gun._bulletC;
	}
	friend sf::Packet& operator << (sf::Packet& packet, const Gun& gun)
	{
		return packet << gun._bulletC;
	}
private:
	BulletController& _bulletC;
	sf::Clock _bulletClock;
	sf::Time _time;
};

class Player : public GameObject
{
public:
	Player() = default;
	void fire()
	{
		_gun.shot(_box.getCenter(), _playerDirection);
	}

	void hurt(float zombiePower) 
	{ 
		_health -= zombiePower / 100.f * (100.f - _armor);
		_strike = true;
		if (_health <= 0.f)
			_isAlive = false;
	}

	Player(BulletController& bulletC, const Point2D& upp, const Point2D& low, 
		const sf::IntRect textureRect = sf::IntRect(0, 0, 55, 65), short playerDirection = up_arrow)
		: GameObject(low, upp, PlayerNS::speed), _armor(20.f), _power(60.f), 
		_health(100.f), _gun(bulletC), _strike(false), _isAlive(true), _playerDirection(playerDirection)
	{
		_playerImage = sf::Image();
		_playerImage.loadFromFile(PlayerNS::image_path);
		_playerTxt = sf::Texture();
		_playerTxt.loadFromImage(_playerImage);
		//_textureRect = textureRect;
		_playerSprite = sf::Sprite(_playerTxt);
		setTextureRect(textureRect);
		_hurtImage = sf::Image();
		_hurtImage.loadFromFile(PlayerNS::image_hurt_path);
		_hurtTxt = sf::Texture();
		_hurtTxt.loadFromImage(_hurtImage);
		_hurtSprite = sf::Sprite(_hurtTxt);
		_hurtSprite.setTextureRect(sf::IntRect(0, 0, 55, 65));
	};

	Player& operator= (const Player& that)
	{
		_armor = that._armor;
		_power = that._power;
		_health = that._health;
		_playerImage = sf::Image();
		_playerImage.loadFromFile(PlayerNS::image_path);
		_playerTxt = sf::Texture();
		_playerTxt.loadFromImage(_playerImage);
		_playerSprite = sf::Sprite(_playerTxt);
		_hurtImage = sf::Image();
		_hurtImage.loadFromFile(PlayerNS::image_hurt_path);
		_hurtTxt = sf::Texture();
		_hurtTxt.loadFromImage(_hurtImage);
		_hurtSprite = sf::Sprite(_hurtSprite);
		_strike = that._strike;
		setTextureRect(that._textureRect);
		_playerDirection = that._playerDirection;
		_gun = { that._gun.getBC() };
		return *this;
	}

	friend sf::Packet& operator >> (sf::Packet& packet, Player& player)
	{
		return packet >> player._armor >> player._box >> player._gun >>
			player._health >> player._isAlive >> player._power >> player._speed >> player._strike;
	}
	friend sf::Packet& operator << (sf::Packet& packet, const Player& player)
	{
		return packet << player._armor << player._box << player._gun <<
			player._health << player._isAlive << player._power << player._speed << player._strike;
	}

	float getPower() { return _power; }
	float getArmor() { return _armor; }
	float getHealth() { return _health; }
	void setHealth(float health) { _health = health; }
	sf::Sprite& sprite() { return _playerSprite; }
	sf::Sprite& hurtSprite() { return _hurtSprite; }
	bool getStrikeCondition() { return _strike; }
	void setStrikeCondition(bool condition) { _strike = condition; }
	bool isAlive() { return _isAlive; }
	void setIsAlive(bool condition) { _isAlive = condition; }
	const sf::IntRect& getTextureRect() const { return _textureRect; }
	void setTextureRect(sf::IntRect textureRect) 
	{ 
		_textureRect = textureRect; 
		_playerSprite.setTextureRect(_textureRect);
	}
	short getPlayerDirection() const { return _playerDirection; }
	void setPlayerDirection(short playerDirection) { _playerDirection = playerDirection; }
private:
	float _armor;
	float _power;
	float _health;
	short _playerDirection;
	Gun _gun;
	sf::Image _playerImage;
	sf::Texture _playerTxt;
	sf::Sprite _playerSprite;
	sf::Image _hurtImage;
	sf::Texture _hurtTxt;
	sf::Sprite _hurtSprite;
	bool _strike;
	bool _isAlive;
	sf::IntRect _textureRect;
};



