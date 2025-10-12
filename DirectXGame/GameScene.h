#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>
#include "Fish.h"

class GameScene 
{
public:

	//初期化
	void Initialize();
	~GameScene();
	//更新
	void Update();

	//描画
	void Draw();

	//デスフラグのgetter
	bool IsFinished() const { return isFinish; }


private:

	Player* player_ = nullptr;
	Model* model_ = nullptr;
	Model* playerModel_ = nullptr;
	Camera camera_;

	// 魚モデル
	Model* fishModel_ = nullptr;

	std::vector<Fish*> fishes_;

	//終了フラグ
	bool isFinish = false;

	//タイマー
	uint32_t numTexHandles_[10];
	Sprite* numSprite_ = nullptr;
	int gameTimer_ = 1800;
	bool isGame_ = true;
};
