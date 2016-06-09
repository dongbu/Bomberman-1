#include "Player.h"



Player::Player()
	:m_bomb(nullptr)
{

}


Player::~Player()
{
}


void Player::SetAnimator(Animator& animator, size_t width, size_t height)
{
	float w = static_cast<float>(width);
	float h = static_cast<float>(height);

	float tileSize = 64; // HACK tmp only

	m_sprite.setOrigin(w / 2.f, h / 2.f);
	SetSize(w, h);
	
	m_animator = &animator;
	m_animator->SetSprite(m_sprite);

	SetPositionX(3 * tileSize + w); 
	SetPositionY(5 * tileSize + h);
	m_sprite.setPosition(sf::Vector2f(3 * tileSize + w, 5 * tileSize + h));
}


void Player::OnMoveKeyPressed(int x, int y)
{
	const float speed = 120;
	SetVelocity(speed*x, speed*y);
}


void Player::OnActionKeyPressed()
{
	if(m_bomb==nullptr)
	{
		m_bomb = new Bomb();
		m_bombAnimator = new Animator();
		m_bombAnimator->AddAnimationState("waitingForExplosion", *m_bombTextureAtlas, 0, m_bombTextureAtlas->GetCount() - 1);
		m_bomb->SetAnimator(*m_bombAnimator, m_bombTextureAtlas->GetCellSizeX(), m_bombTextureAtlas->GetCellSizeY());
		m_bomb->SetUpRay(m_bombRayTextureAtlas);
		m_bomb->SetDetonationTime(sf::seconds(3.f));
		m_bomb->SetRayOnScreenTime(sf::seconds(1));
		m_bomb->SetPosition(static_cast<int>(GetPositionX()), static_cast<int>(GetPositionY()));
		m_bomb->SetLevelPointer(level);
	}
}

void Player::SetUpBomb(TextureAtlas* atlasBomb, TextureAtlas* atlasRay)
{
	m_bombTextureAtlas = atlasBomb;
	m_bombRayTextureAtlas = atlasRay;
}


void Player::SetLevelPointer(Level * level)
{
	if (level == nullptr)
	{
		std::cerr << "Level pointer is a null!" << std::endl;
		exit(5);
	}
	this->level = level;
}


std::vector<sf::FloatRect> Player::GetBombRaysColliders()
{
	if (m_bomb != nullptr && m_bomb->GetState() == Bomb::exploding)
		return m_bomb->GetBombRaysColliders();
	else return std::vector<sf::FloatRect>();//this will be changed when whole collisions system will be rewritten - don't worry Szymon :)
}


void Player::CheckIsPlayerInBombRay(std::vector<sf::FloatRect>* bombRays)
{
	if (m_bomb != nullptr)
	{
		if (m_bomb->IsObjectInRay(m_sprite.getGlobalBounds()))
		{
			reactWhenIsInBombRay();
			//return;
		}
		if (bombRays != nullptr)
		{
			for (unsigned int i = 0; i < bombRays->size(); i++)
			{
				if (bombRays[i][i].intersects(m_sprite.getGlobalBounds()))
				{
					reactWhenIsInBombRay();
					//return;
				}
			}
		}
	}
}



void Player::Update(const float & dt)
{
	m_animator->Animate(dt);

	if (movementX != 0 && movementY != 0)
	{
		movementX /= 1.41f;
		movementY /= 1.41f;
	}
	
	SetPositionX(GetPositionX() + movementX);
	SetPositionY(GetPositionY() + movementY);
	m_sprite.setPosition(GetPositionX(),GetPositionY());

	if (m_bomb != nullptr)
	{
		m_bomb->Update(dt);

		if (m_bomb->GetState() == Bomb::exploded)
		{
			delete m_bomb;
			m_bomb = nullptr;
		}
	}
}


void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (m_bomb != nullptr)
		target.draw(*m_bomb);

	target.draw(m_sprite);
}


void Player::reactWhenIsInBombRay()
{
	//Animation etc
	std::cout << "\r[DEBUG] Player is colliding with bomb ray";
}
