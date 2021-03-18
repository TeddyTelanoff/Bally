#define OLC_PGE_APPLICATION
#define OLC_PGEX_GRAPHICS2D
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"


#define WIDTH 600
#define HEIGHT 900

#define POS_UNION(x, y) union { Vec2f Pos { x, y }; struct { float X, Y; }; }
#define SIZE_UNION(w, h) union { Vec2f Size { w, h }; struct { float Width, Height; }; }

namespace
{
	olc::PixelGameEngine *Instance;

	using Vec2i = olc::vi2d;
	using Vec2f = olc::vf2d;
	using olc::PixelF;

	using Transform = olc::GFX2D::Transform2D;

	namespace Math
	{
		constexpr float Pi { 3.1415926535f };
		constexpr auto Abs = std::fabsf;
	};
}

struct Player
{
	POS_UNION(0, 0);
	float Speed { 69 };
	float Drag { 0 };
	Vec2f Velocity {};

	float Radius { 25 };
};

struct Pad
{
	POS_UNION(0, 0);
	SIZE_UNION(100, 30);
	float Rot {};

	virtual void Draw()
	{ Instance->FillRect(Pos, Size, PixelF(0, 1, 0)); }

	virtual void Update(float in, float deltaTime) = 0;
};

struct TurnPad: public Pad
{
	float RotSpeed { 5 };

	virtual void Update(float in, float deltaTime) override
	{
		Rot += RotSpeed * deltaTime;
	}
};

class Game: public olc::PixelGameEngine
{
private:
	Player m_Player {};
	Vec2f Gravity { 0, Math::Pi };
	float WallBounce { 0.42f };
	float GroundBounce { 0.88f };

	std::vector<Pad *> m_Pads {};
public:
	Game()
	{
		Instance = this;
		sAppName = "Bally";
	}

	virtual bool OnUserCreate() override
	{
		olc::GFX2D s;

		TurnPad *pad = new TurnPad;
		pad->Pos = { (float)WIDTH / 2, (float)HEIGHT / 2 };
		m_Pads.push_back(pad);
		m_Player.Pos = { (float)WIDTH / 2, (float)HEIGHT / 2 };
		return true;
	}

	virtual bool OnUserUpdate(float deltaTime) override
	{
		if (m_Player.Y + m_Player.Radius > HEIGHT)
		{
			m_Player.Y = HEIGHT - m_Player.Radius;
			m_Player.Velocity.y = -Math::Abs(m_Player.Velocity.y) * GroundBounce;
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

		Clear(PixelF(0.25f, 0.25f, 0.25f));
		FillCircle(m_Player.Pos, m_Player.Radius, PixelF(0, 0.5f, 0.6f));

		for (Pad *pad : m_Pads)
			pad->Draw();
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