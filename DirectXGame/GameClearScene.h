#pragma once
#include "KamataEngine.h"
#include "Score.h"
#include "BGM.h"
#include "Fade.h"

class GameClearScene {
public:

	// フェード
	enum class Phase { 
		kFadeIn, 
		kMain, 
		kfadeOut 
	};

	~GameClearScene();

	// 初期化
	void Initialize(Score* score);

	// 更新
	void Update();

	// 描画
	void Draw();

	// デスフラグのgetter
	bool IsFinished() const { return isFinish; }

private:
	// 終了フラグ
	bool isFinish = false;

	//スコア
	Score* score_ = nullptr;
	BGM* bgm_ = nullptr;

	// フェード
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;

	// フェード用
	int timer;

	uint32_t gameClearBgmHandle_; // クリアしたときのBGM
};
