#include "stdafx.h"
#include "GameObjects.h"

std::vector<GameObject*> gameObjects;

GameObject::GameObject(std::string texturePath, const sf::Vector2f & pos)
	: m_pos(pos)
	, m_destroyed(false)
{
	// Load the texture
	// Assign the sprite		
	m_texture.loadFromFile(texturePath);
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_sprite.getTextureRect().width * 0.5f, m_sprite.getTextureRect().height * 0.5f);
}

void GameObject::Destroy()
{
	m_destroyed = true;
}

bool GameObject::IsDestroyed()
{
	return m_destroyed;
}

void GameObject::Draw(sf::RenderWindow * window)
{
	sf::CircleShape collisionShape;
	collisionShape.setPosition(m_pos);
	collisionShape.setRadius(m_collisionRadius);
	collisionShape.setOutlineColor(sf::Color::Magenta);
	collisionShape.setOutlineThickness(3);
	collisionShape.setFillColor(sf::Color::Transparent);
	collisionShape.setOrigin(m_collisionRadius, m_collisionRadius);
	//window->draw(collisionShape);
	window->draw(m_sprite);
}

void GameObject::Update(float dt)
{
	float decelScalar = 0.5f;
	m_vel += m_accel * dt;
	
	LimitVelocity(dt);
	ApplyDrag(dt);

	m_pos += m_vel * dt;
	if (m_pos.x < 0)
	{
		m_pos.x = 1280;
	}
	else if (m_pos.x > 1280)
	{
		m_pos.x = 0;
	}

	if (m_pos.y < 0)
	{
		m_pos.y = 720;
	}
	else if (m_pos.y > 720)
	{
		m_pos.y = 0;
	}


	m_sprite.setRotation(m_angle);
	m_sprite.setPosition(m_pos);
}

void GameObject::LimitVelocity(float dt)
{
	float speed = sqrt(m_vel.x * m_vel.x + m_vel.y  *m_vel.y);	

	if (speed <= 0.1)
	{
		return;
	}

	sf::Vector2f normalizedVel = sf::Vector2f(m_vel.x / speed, m_vel.y / speed);

	if (speed > 500)
	{
		speed = 500;
	}

	m_vel.x = normalizedVel.x * speed;
	m_vel.y = normalizedVel.y * speed;
}

void GameObject::SetAngle(float angle)
{
	m_angle = angle;
}

float GameObject::GetAngle()
{
	return m_angle;
}

bool GameObject::IsCollidingWith(GameObject * other)
{
	sf::Vector2f vectorToOther = other->m_pos - m_pos;
	float dist = sqrt(vectorToOther.x * vectorToOther.x + vectorToOther.y * vectorToOther.y);

	if (dist < (m_collisionRadius + other->m_collisionRadius))
	{
		return true;
	}
	return false;
}

void GameObject::SetEngine(GameEngine* engine)
{
	m_engine = engine;
}

void GameObject::SetPos(const sf::Vector2f & pos)
{
	m_pos = pos;
}

void GameObject::SetAccel(float amount)
{	
	if (amount > 0)
	{
		float rotInRadians = DEG_TO_RAD * m_angle;
		m_accel = sf::Vector2f(amount * sin(rotInRadians), -amount * cos(rotInRadians));
	}
	else
	{
		m_accel = sf::Vector2f(0, 0);
	}
}

void GameObject::SetVelocity(float amount)
{
	if (amount > 0)
	{
		float rotInRadians = DEG_TO_RAD * m_angle;
		m_vel = sf::Vector2f(amount * sin(rotInRadians), -amount * cos(rotInRadians));
	}
	else
	{
		m_vel = sf::Vector2f(0, 0);
	}
}

void GameObject::SetVelocity(sf::Vector2f vel)
{
	m_vel = vel;
}

sf::Vector2f GameObject::GetPosition()
{
	return m_pos;
}

void GameObject::ApplyDrag(float dt)
{
	if (m_accel.x == 0 && m_accel.y == 0)
	{
		float dragAmount = dt * 0.9f;
		m_vel.x -= dragAmount* m_vel.x;
		m_vel.y -= dragAmount * m_vel.y;
	}
}

Bullet::Bullet(const sf::Vector2f & pos)
	: GameObject("Sprites/PNG/Lasers/laserBlue08.png", pos)
	, m_timeAlive(0.0f)
{
	SetCollisionRadius(10.0f);
	m_sprite.setScale(0.5, 0.5);
}

void Bullet::ApplyDrag(float dt)
{
	// do nothing
}

void Bullet::Update(float dt)
{
	GameObject::Update(dt);
	m_angle += dt * 360;
	m_timeAlive += dt;

	if (m_timeAlive > 3.0f)
	{
		Destroy();
	}
}

void Bullet::CollidedWith(GameObject * other)
{	
	Player* thePlayer = dynamic_cast<Player*>(other);
	if (thePlayer == NULL)
	{
		Destroy();
		other->Destroy();
	}
}

Player::Player(std::string texturePath, const sf::Vector2f & pos)
	: GameObject(texturePath, pos)	
	, weaponCooldown(0)
{
	SetCollisionRadius(35.0f);
}

void Player::Update(float dt)
{
	GameObject::Update(dt);	
	weaponCooldown -= dt;	
	SetAccel(0.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		SetAngle(GetAngle() + 180 * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		SetAngle(GetAngle() - 180 * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		SetAccel(400.0f);
	}

	if (weaponCooldown <= 0.0f)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{			
			weaponCooldown = .2f;
			Bullet* newBullet = new Bullet(m_pos);
			sf::Vector2f newVelocity;
			newVelocity.x = sin(m_angle * 2 * 3.1415 / 360.0f) * 300.0f;
			newVelocity.y = -cos(m_angle * 2 * 3.1415 / 360.0f) * 300.0f;
			newBullet->SetVelocity(newVelocity);
			m_engine->AddObject(newBullet);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			weaponCooldown = 5.f;
			SuperBullet* newBullet = new SuperBullet(m_pos);
			sf::Vector2f newVelocity;
			newVelocity.x = sin(m_angle * 2 * 3.1415 / 360.0f) * 150.0f;
			newVelocity.y = -cos(m_angle * 2 * 3.1415 / 360.0f) * 150.0f;
			newBullet->SetVelocity(newVelocity);
			m_engine->AddObject(newBullet);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void GameEngine::AddObject(GameObject* obj)
{
	obj->SetEngine(this);
	m_gameObjects.push_back(obj);
}

void GameEngine::Draw(sf::RenderWindow* window)
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		current->Draw(window);
	}
}

void GameEngine::Update(float dt)
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		current->Update(dt);
	}

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		for (int j = 0; j < m_gameObjects.size(); j++)
		{
			GameObject* other = m_gameObjects[j];
			if (other != current
				&& !other->IsDestroyed()
				&& !current->IsDestroyed()
				&& current->IsCollidingWith(other))
			{
				current->CollidedWith(other);
			}
		}
	}

	for (int i = m_gameObjects.size() - 1; i >= 0; i--)
	{
		GameObject* current = m_gameObjects[i];
		if (current->IsDestroyed())
		{
			delete current;
			m_gameObjects.erase(m_gameObjects.begin() + i);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
Asteroid::Asteroid(std::string texturePath, const sf::Vector2f & pos)
	: GameObject(texturePath, pos)
{
}

void Asteroid::Update(float dt)
{
	GameObject::Update(dt);
	m_angle += 180 * dt;
}

LargeAsteroid::LargeAsteroid(const sf::Vector2f & pos)
	: Asteroid("Sprites/PNG/Meteors/meteorBrown_big3.png", pos)
{
	SetCollisionRadius(50.0f);
}

SmallAsteroid::SmallAsteroid(const sf::Vector2f& pos)
	: Asteroid("Sprites/PNG/Meteors/meteorBrown_small2.png", pos)
{
	SetCollisionRadius(20.0f);

}

SuperBullet::SuperBullet(const sf::Vector2f & pos)
	: Bullet(pos)
{
	m_sprite.setScale(3, 3);
	m_sprite.setColor(sf::Color::Red);
	SetCollisionRadius(60);
}

void SuperBullet::CollidedWith(GameObject * other)
{
	Player* thePlayer = dynamic_cast<Player*>(other);
	if (thePlayer == NULL)
	{
		other->Destroy();
	}
}
