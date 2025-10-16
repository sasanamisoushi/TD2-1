#pragma once
#include <array>

namespace KamataEngine {
  class Sprite;
}

class Score
{
public:

	~Score();

	void Initialize();

	void AddScore(int score);

	void AddScoreCombo(int score);

	void SubtractScore(int score);

	void ResetScore();

	void Update();

	void Draw();

private:

	int score_;

	int combo_;

	std::array<KamataEngine::Sprite*, 10> sprites_ = {}; // 配列 描画
	std::array<int, 10> tex_ = {}; // 配列 数字
};
