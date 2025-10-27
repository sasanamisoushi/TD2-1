#pragma once
#include "KamataEngine.h"
#include "Score.h"
#include "BGM.h"

class GameClearScene {
public:

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

	Score* score_ = nullptr;
	BGM* bgm_ = nullptr;
};
