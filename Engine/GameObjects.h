#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#define DEG_TO_RAD (0.0174532925f)

class GameObject;

class GameEngine
{
public:
	void AddObject(GameObject* obj);
	void Draw(sf::RenderWindow* window);
	void Update(float dt);

private:
	std::vector<GameObject*> m_gameObjects;
};

class GameObject
{
public:
	GameObject(std::string texturePath, const sf::Vector2f& pos);
	virtual void Draw(sf::RenderWindow* window);;
	virtual void Update(float dt);
	virtual void LimitVelocity(float dt);
	virtual void ApplyDrag(float dt);

	void SetAngle(float angle);
	float GetAngle();
	bool IsCollidingWith(GameObject* other);
	virtual void CollidedWith(GameObject* other) {};

	void SetEngine(GameEngine* engine);
	void SetPos(const sf::Vector2f& pos);
	void SetAccel(float amount);
	void SetVelocity(float amount);
	void SetVelocity(sf::Vector2f vel);
	
	void SetCollisionRadius(float radius) 
	{ 
		m_collisionRadius = radius; 
	}

	sf::Vector2f GetPosition();

	virtual void Destroy();
	bool IsDestroyed();
protected:
	sf::Vector2f m_pos;
	sf::Vector2f m_vel;
	sf::Vector2f m_accel;

	sf::Sprite m_sprite;
	sf::Texture m_texture;
	float m_collisionRadius;
	float m_angle;
	bool m_destroyed;
	GameEngine* m_engine;
};

class Bullet : public GameObject
{
public:
	Bullet(const sf::Vector2f& pos);
	void ApplyDrag(float dt);
	virtual void Update(float dt);
	virtual void CollidedWith(GameObject* other);
	float m_timeAlive;
	
};

class SuperBullet : public Bullet
{
public:
	SuperBullet(const sf::Vector2f& pos);
	virtual void CollidedWith(GameObject* other);
};

class Player : public GameObject
{
public:
	Player(std::string texturePath, const sf::Vector2f& pos);
	
	virtual void Update(float dt);	

private:	
	float weaponCooldown;
};


class Asteroid : public GameObject
{
public:
	void ApplyDrag(float dt) {};
	Asteroid(std::string texturePath, const sf::Vector2f& pos);	
	virtual void Update(float dt);
};

class LargeAsteroid : public Asteroid
{
public:
	LargeAsteroid(const sf::Vector2f& pos);	
};

class SmallAsteroid : public Asteroid
{
public:
	SmallAsteroid(const sf::Vector2f& pos);
};

