#define OLC_PGE_APPLICATION
#define OLC_PGEX_GRAPHICS2D
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

struct Player
{
	union
	{
		olc::vf2d Pos {};
		struct { float X, Y; };
	};

	float Speed { 69 };
	float Drag { 1 };
	olc::vf2d Velocity {};

	float Radius { 25 };
};

class Game: public olc::PixelGameEngine
{
private:
	Player m_Player {};

	__declspec(property(get = ScreenWidth)) int m_Width;
	__declspec(property(get = ScreenHeight)) int m_Height;
public:
	Game()
	{ sAppName = "Bally"; }

	virtual bool OnUserCreate() override
	{ return true; }

	virtual bool OnUserUpdate(float deltaTime) override
	{
		if (GetKey(olc::Key::UP).bHeld)
			m_Player.Velocity.y -= m_Player.Speed * deltaTime;
		if (GetKey(olc::Key::DOWN).bHeld)
			m_Player.Velocity.y += m_Player.Speed * deltaTime;
		if (GetKey(olc::Key::LEFT).bHeld)
			m_Player.Velocity.x -= m_Player.Speed * deltaTime;
		if (GetKey(olc::Key::RIGHT).bHeld)
			m_Player.Velocity.x += m_Player.Speed * deltaTime;

		m_Player.Pos += m_Player.Velocity;
		m_Player.Velocity -= m_Player.Velocity * m_Player.Drag * deltaTime;

		FillRect({ 0, 0 }, olc::vf2d(m_Width, m_Height), olc::PixelF(0.25f, 0.25f, 0.25f));
		FillCircle(m_Player.Pos, m_Player.Radius, olc::PixelF(0, 0.5f, 0.6f));
		return true;
	}
};


int main()
{
	Game game;
	if (game.Construct(600, 900, 1, 1))
		game.Start();

	return 0;
}