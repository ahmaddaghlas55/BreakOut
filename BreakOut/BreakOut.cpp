// BreakOUT Clone - Cleaned and Unified Ball Logic
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class BreakOUT : public olc::PixelGameEngine
{
public:
	BreakOUT()
	{
		sAppName = "Break out wana be ";
	}

	


	void GenerateLevelLayout()
	{
		for (int y = 0; y < 30; y++)
			for (int x = 0; x < 24; x++)
			{
				if (x == 0 || y == 0 || x == 23)
					blocks[y * 24 + x] = 10; // Boundaries
				else
					blocks[y * 24 + x] = 0;  // Empty space
			}

		// Different tile layouts per level
		if (level == 1)
		{
			for (int x = 3; x <= 20; x++)
			{
				blocks[4 * 24 + x] = 1;
				blocks[6 * 24 + x] = 2;
				blocks[8 * 24 + x] = 3;
			}

			


		}
		else if (level == 2)
		{
			for (int x = 2; x <= 21; x++)
			{
				blocks[3 * 24 + x] = 3;
				blocks[5 * 24 + x] = 2;
				blocks[10 * 24 + x] = 1;
			}
		}
		else if (level == 3)
		{
			for (int x = 5; x <= 18; x++)
			{
				blocks[3 * 24 + x] = 2;
				blocks[6 * 24 + x] = 1;
				blocks[9 * 24 + x] = 3;
			}
		}
		else if (level == 4) {
			for (int y = 0; y < 30; y++)
				for (int x = 0; x < 24; x++) {
					if (x == 0 || y == 0 || x == 23)
						blocks[y * 24 + x] = 10;
					else
						blocks[y * 24 + x] = 0;

					if (x > 2 && x <= 20 && y > 3 && y <= 5)
						blocks[y * 24 + x] = 1;
					if (x > 2 && x <= 20 && y > 5 && y <= 7)
						blocks[y * 24 + x] = 2;
					if (x > 2 && x <= 20 && y > 7 && y <= 9)
						blocks[y * 24 + x] = 3;
				}
		}
	}




	void StartNewGame()
	{
		Trys = 3;
		vBallPos = { 12.5f, 15.5f };

		vBallDir = { cosf(-0.4f), sinf(-0.4f) };
		//vBallDir = { cosf(3.14159f / 2), sinf(3.14159f / 2) }; // Downwards direction
		vBallDir = vBallDir.norm();

		// Adjust game parameters based on the level
		switch (level)
		{
		case 1:
			fBallSpeed = 20.0f;
			fBatWidth = 50.0f;
			break;
		case 2:
			fBallSpeed = 30.0f;
			fBatWidth = 40.0f;
			break;
		case 3:
			fBallSpeed = 40.0f;
			fBatWidth = 30.0f;
			break;
		case 4:
			fBallSpeed = 45.0f;
			fBatWidth = 25.0f;
			break;
			

		default:
			fBallSpeed = 50.0f;
			fBatWidth = 20.0f;
			break;
		}

		GenerateLevelLayout(); // Call a function to set up tile layout

		Clear(olc::BLUE);
		
	}

	void UpdateGame(float fElapsedTime)
	{

		if (GetKey(olc::Key::LEFT).bHeld) fBatPos -= fBatSpeed * fElapsedTime;
		if (GetKey(olc::Key::RIGHT).bHeld) fBatPos += fBatSpeed * fElapsedTime;
		if (fBatPos < 11.0f) fBatPos = 11.0f;
		if (fBatPos + fBatWidth > float(ScreenWidth()) - 10.0f) fBatPos = float(ScreenWidth()) - 10.0f - fBatWidth;

		olc::vf2d screenBallPos = vBallPos * vBlockSize;
		if (screenBallPos.y <= 10.0f) vBallDir.y *= -1.0f;
		if (screenBallPos.x <= 10.0f || screenBallPos.x >= float(ScreenWidth()) - 10.0f) vBallDir.x *= -1.0f;

		if (screenBallPos.y >= (float(ScreenHeight()) - 20.0f) &&
			screenBallPos.x > fBatPos &&
			screenBallPos.x < fBatPos + fBatWidth)
		{
			vBallDir.y *= -1.0f;
			float batCenter = fBatPos + fBatWidth / 2.0f;
			float offset = (screenBallPos.x - batCenter) / (fBatWidth / 2.0f);
			vBallDir.x += offset * 0.5f;
			vBallDir = vBallDir.norm();
		}

		if (screenBallPos.y > float(ScreenHeight()+10.0f))
		{
			Trys--;
			if (Trys <= 0)
				eState = GameState::GameOver;
			else {
				vBallPos = { 12.5f, 15.5f };
				float fAngle = (float(rand()) / float(RAND_MAX)) * 3.14159f * 0.5f + 3.14159f * 0.75f;
				vBallDir = { cosf(fAngle), sinf(fAngle) };
				vBallDir = vBallDir.norm();
			}
		}

		olc::vf2d vPotentialBallPos = vBallPos + vBallDir * fBallSpeed * fElapsedTime;
		olc::vf2d vTileBallRadialDims = { fBallRadius / vBlockSize.x, fBallRadius / vBlockSize.y };

		auto TestResolveCollisionPoint = [&](const olc::vf2d& point, olc::vf2d& hitpos, int& id)
			{
				olc::vi2d vTestPoint = vPotentialBallPos + vTileBallRadialDims * point;

				// Ignore tiles at the bottom of the screen
				if (vTestPoint.y >= 29)
					return false;

				auto& tile = blocks[vTestPoint.y * 24 + vTestPoint.x];
				if (tile == 0) return false;

				bool bTileHit = tile < 10;
				if (bTileHit) {
					id = tile;
					hitpos = { float(vTestPoint.x), float(vTestPoint.y) };
					tile--;
					ScoreCounter += 10.0f;
				}
				if (point.x == 0.0f) vBallDir.y *= -1.0f;
				if (point.y == 0.0f) vBallDir.x *= -1.0f;

				return bTileHit;
			};

		olc::vf2d hitpos;
		int hitid = 0;
		bool bHasHitTile = false;
		bHasHitTile |= TestResolveCollisionPoint({ 0, -1 }, hitpos, hitid);
		bHasHitTile |= TestResolveCollisionPoint({ 0,  1 }, hitpos, hitid);
		bHasHitTile |= TestResolveCollisionPoint({ -1,  0 }, hitpos, hitid);
		bHasHitTile |= TestResolveCollisionPoint({ 1,  0 }, hitpos, hitid);

			// Fake Floor
			if (vBallPos.y > 20.0f) vBallDir.y *= -1.0f;
		
		if (bHasHitTile)
			for (int i = 0; i < 100; i++) {
				sFragment f;
				f.pos = hitpos + olc::vf2d(0.5f, 0.5f);
				float a = float(rand()) / float(RAND_MAX) * 2.0f * 3.14159f;
				float v = float(rand()) / float(RAND_MAX) * 10.0f;
				f.vel = { v * cosf(a), v * sinf(a) };
				f.fAngel = a; f.fTime = 3.0f;
				f.colour = (hitid == 1 ? olc::RED : hitid == 2 ? olc::GREEN : olc::YELLOW);
				listFragments.push_back(f);
			}

		
		



		vBallPos += vBallDir * fBallSpeed * fElapsedTime;

		for (auto& f : listFragments) {
			f.vel += olc::vf2d(0.0f, 20.0f) * fElapsedTime;
			f.pos += f.vel * fElapsedTime;
			f.fAngel += 5.0f * fElapsedTime;
			f.fTime -= fElapsedTime;
			f.colour.a = uint8_t((f.fTime / 3.0f) * 255);
		}
		listFragments.remove_if([](const sFragment& f) { return f.fTime < 0.0f; });


		




		bool bBlocksRemaining = false;
		for (int i = 0; i < 24 * 30; i++)
			if (blocks[i] >= 1 && blocks[i] <= 3) { bBlocksRemaining = true; break; }

		if (!bBlocksRemaining)
		{
			level++; // Move to the next level
			StartNewGame(); // Restart with the new level's settings
			
		}
		if (level == 5) {
			eState = GameState::Win;
		}

		Clear(olc::DARK_BLUE);
		SetPixelMode(olc::Pixel::MASK);
		
		for (int y = 0; y < 30; y++)
			for (int x = 0; x < 24; x++) {
				int t = blocks[y * 24 + x];
				if (t > 0)

					DrawPartialSprite(olc::vi2d(x, y) * vBlockSize, sprTile.get(), olc::vi2d(t, 0) * vBlockSize, vBlockSize);

				//	DrawPartialSprite({ x, y } *vBlockSize, sprTile.get(), { t, 0 } *vBlockSize, vBlockSize);
					switch (blocks[y * 24 + x])
					{
					case 0: // Do nothing
						break;
					case 10: // Draw Boundary
						FillRect(olc::vi2d(x, y) * vBlockSize, vBlockSize, olc::BLACK);
						break;
					}
					
					/*DrawPartialSprite(olc::vi2d(x, y) * vBlockSize, sprTile.get(),
						olc::vi2d(t, 0) * vBlockSize, vBlockSize);*/
				

				
			}
		
		SetPixelMode(olc::Pixel::NORMAL);
		FillCircle(vBallPos * vBlockSize, fBallRadius, olc::CYAN);
		for (auto& f : listFragments)
			DrawRotatedDecal(f.pos * vBlockSize, decFragment.get(), f.fAngel, { 4, 4 }, { 1, 1 }, f.colour);
		FillRect(int(fBatPos), ScreenHeight() - 20, int(fBatWidth), 10, olc::GREEN);
		DrawString(10, 30, "Score: " + std::to_string(int(ScoreCounter)), olc::WHITE);
		for (int i = 0; i < Trys; i++) {
			DrawSprite(4 + i * (heart->width + 2), 0.75, heart.get());
		}
		DrawString(200,30, "level:" + std::to_string(level), olc::WHITE);
	}

private:
	float fBatPos = 20.0f;
	float fBatWidth = 50.0f;
	float fBatSpeed = 250.0f;
	float fTargetFrameTime = 1.0f / 100.0f;
	float fAccumlatedTime = 0.0f;
	olc::vi2d vBlockSize = { 16, 16 };
	std::unique_ptr<int[]> blocks;
	olc::vf2d vBallPos = { 0.0f, 0.0f };
	olc::vf2d vBallDir = { 0.0f, 0.0f };
	float fBallRadius = 5.0f;
	float fBallSpeed = 20.0f;
	int Trys = 3;
	int previousLives = Trys;
	float ScoreCounter = 0.0f;
	std::unique_ptr<olc::Sprite> sprTile;
	std::unique_ptr<olc::Sprite> sprFragment;
	std::unique_ptr<olc::Decal> decFragment;
	std::unique_ptr<olc::Sprite> GrayTile;
	std::unique_ptr<olc::Sprite> heart;

	int level = 0; // creat new level 

	struct sFragment {
		olc::vf2d pos;
		olc::vf2d vel;
		float fAngel;
		float fTime;
		olc::Pixel colour;
		float life;
	};
	enum class GameState { Menu, Playing, GameOver, Win };
	GameState eState = GameState::Menu;
	std::list<sFragment> listFragments;

public:
	bool OnUserCreate() override
	{
		blocks = std::make_unique<int[]>(24 * 30);
		sprTile = std::make_unique<olc::Sprite>("C:\\Users\\ahmad\\OneDrive\\Pictures\\tut_tilesC.png");
		sprFragment = std::make_unique<olc::Sprite>("C:\\Users\\ahmad\\OneDrive\\Pictures\\tut_fragment.png");
		decFragment = std::make_unique<olc::Decal>(sprFragment.get());
		//"C:\\Users\\ahmad\\OneDrive\\Pictures\\tut_tile.png"
		//GrayTile = std::make_unique<olc::Sprite>("C:\\Users\\ahmad\\OneDrive\\Pictures\\tut_tile.png");
		heart = std::make_unique<olc::Sprite>("C:\\Users\\ahmad\\OneDrive\\Pictures\\heart pixel art 16x16.png");


		StartNewGame();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		switch (eState)
		{
		case GameState::Menu:
			Clear(olc::BLUE);
			DrawString(ScreenWidth() / 2 - 40, ScreenHeight() / 2, "PRESS SPACE TO START", olc::WHITE);
			if (GetKey(olc::Key::SPACE).bPressed) {
				StartNewGame();

				eState = GameState::Playing;
			}
			break;
		case GameState::Playing:
			UpdateGame(fElapsedTime);
			break;
		case GameState::GameOver:
			Clear(olc::DARK_RED);
			DrawString(ScreenWidth() / 2 - 40, ScreenHeight() / 2, "GAME OVER", olc::WHITE);
			DrawString(ScreenWidth() / 2 - 40, ScreenHeight() / 2 + 10, "PRESS R TO RESTART", olc::WHITE);
			//ScoreCounter
			DrawString(ScreenWidth() / 2 - 40, ScreenHeight() / 2 + 20, "Score:"+ std::to_string(ScoreCounter), olc::WHITE);
			

			if (GetKey(olc::Key::R).bPressed)
				eState = GameState::Menu;
			break;
		case GameState::Win:
			Clear(olc::DARK_GREEN);
			DrawString(ScreenWidth() / 2 - 40, ScreenHeight() / 2 - 10, "YOU WIN!", olc::YELLOW);
			DrawString(ScreenWidth() / 2 - 40, ScreenHeight() / 2 + 10, "PRESS R TO RESTART", olc::WHITE);
			DrawString(ScreenWidth() / 2 - 40, ScreenHeight() / 2 + 20, "Score:" + std::to_string(ScoreCounter), olc::WHITE);

			

			if (GetKey(olc::Key::R).bPressed)
				eState = GameState::Menu;
			break;
		}
		return true;
	}
};

int main()
{
	BreakOUT demo;
	if (demo.Construct(512, 480, 1, 1))
		demo.Start();
	return 0;
}