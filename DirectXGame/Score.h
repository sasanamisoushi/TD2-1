#pragma once
#include <array>

namespace KamataEngine {
  class Sprite;
}

class Score
{

private:

	int score_;

	std::array<KamataEngine::Sprite*, 10> sprites_ = {}; // 配列 描画
	std::array<int, 10> tex_ = {}; // 配列 数字

public:

	void Initialize();

	void Update();

	void Draw();
};
