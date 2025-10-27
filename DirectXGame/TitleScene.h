#pragma once
#include "KamataEngine.h"
#include "Fade.h"
#include "BGM.h"

class TitleScene {
public:

	//フェード
	enum class Phase {
		kFadeIn,
		kMain,
		kfadeOut
	};

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();


	//デスフラグのgetter
	bool IsFinished() const { return isFinish; }

	~TitleScene();

private:

	//終了フラグ
	bool isFinish = false;

	//フェード
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;

	BGM* bgm_ = nullptr;

	//フェード用
	int timer;

	uint32_t titleBgmHandle_; // タイトルのBGM

	//タイトル画像
	//KamataEngine::Sprite* titleSprite_ =nullptr;


};
