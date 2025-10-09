#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>
#include "Fish.h"
#include "BigFish.h"

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

	//========================================
	//魚
	//========================================
	// 魚モデル
	Model* fishModel_ = nullptr;

	std::vector<Fish*> fishes_;


	//大きい魚のモデル
	Model* bigFishModel_ = nullptr;
	std::vector<BigFish*> BigFishes_;

	// 現在数をカウント
	int smallCount = 0;
	int bigCount = 0;

	//終了フラグ
	bool isFinish = false;
};
