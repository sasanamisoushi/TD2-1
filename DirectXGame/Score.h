#pragma once
#include <array>

namespace KamataEngine {
  class Sprite;
}

class Score
{
public:

	 enum class DisplayMode { 
		 Normal,
		 Result
	 };

	~Score();

	void Initialize();

	void AddScore(int score);

	void AddScoreBear(int score);

	void AddScoreCombo(int score);

	void SubtractScore(int score);

	void ResetScore();

	void Update();

	void Draw();

	void RankingDraw();

	void FileOpen();

	void FileWrite();

	void scoreBossClear();

	int isScoreBossClear = false;

	void SetScoreDisplayMode(DisplayMode mode);

private:

	int score_;

	int combo_;

	std::array<int, 3> rankingScore_;

	std::array<KamataEngine::Sprite*, 10> sprites_ = {}; // 配列 描画
	std::array<std::array<KamataEngine::Sprite*, 10>,3> rankingSprites = {}; // 配列 描画 １位
	std::array<int, 10> tex_ = {}; // 配列 数字

	DisplayMode displayMode_ = DisplayMode::Normal;
};
