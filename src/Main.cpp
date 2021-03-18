#define OLC_PGE_APPLICATION
#define OLC_PGEX_GRAPHICS2D
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"


#define WIDTH 600
#define HEIGHT 900

namespace Math
{
	constexpr float Pi { 3.1415926535f };
	constexpr auto Abs = std::fabsf;
};

struct Player
{
	union
	{
		olc::vf2d Pos {};
		struct { float X, Y; };
	};

	float Speed { 69 };
	float Drag { 0 };
	olc::vf2d Velocity {};

	float Radius { 25 };
};

class Game: public olc::PixelGameEngine
{
private:
	Player m_Player {};
	olc::vf2d Gravity { 0, Math::Pi };
	float WallBounce { 0.42f };
public:
	Game()
	{ sAppName = "Bally"; }

	virtual bool OnUserCreate() override
	{
		m_Player.Pos = { (float)WIDTH / 2, (float)HEIGHT / 2 };
		return true;
	}

	virtual bool OnUserUpdate(float deltaTime) override
	{
		if (m_Player.Y + m_Player.Radius > HEIGHT)
		{
			m_Player.Y = HEIGHT - m_Player.Radius;
			m_Player.Velocity.y = -Math::Abs(m_Player.Velocity.y);
		}
		
		if (m_Player.X + m_Player.Radius > WIDTH)
		{
			m_Player.X = WIDTH - m_Player.Radius;
			m_Player.Velocity.x = -Math::Abs(m_Player.Velocity.x) * WallBounce;
		}
		else if (m_Player.X - m_Player.Radius < 0)
		{
			m_Player.X = m_Player.Radius;
			m_Player.Velocity.x = Math::Abs(m_Player.Velocity.x) * WallBounce;
		}
		
		m_Player.Velocity += Gravity * deltaTime;
		m_Player.Pos += m_Player.Velocity;
		m_Player.Velocity -= m_Player.Velocity * m_Player.Drag * deltaTime;

		Clear(olc::PixelF(0.25f, 0.25f, 0.25f));
		FillCircle(m_Player.Pos, m_Player.Radius, olc::PixelF(0, 0.5f, 0.6f));
		return true;
	}
};


int main()
{
	Game game;
	if (game.Construct(WIDTH, HEIGHT, 1, 1))
		game.Start();

	return 0;
}