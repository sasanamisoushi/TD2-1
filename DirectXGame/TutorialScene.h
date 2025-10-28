#pragma once
#include "Fade.h"
#include "KamataEngine.h"
#include <array>

namespace KamataEngine {
class Sprite;
}

class TutorialScene
{
public:

	// フェード
	enum class Phase
	{ 
		kFadeIn, 
		kMain, 
		kfadeOut 
	};

	~TutorialScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// デスフラグのgetter
	bool IsFinished() const { return isFinish; }

private:

	// 終了フラグ
	bool isFinish = false;

		// フェード
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;

	// フェード用
	int timer;

	// テクスチャハンドル
	std::array<uint32_t, 2> tutorialScoreGround_ = {}; // 配列 数字

	std::array<KamataEngine::Sprite*, 2> tutorialSprites= {};

	int page;

};
