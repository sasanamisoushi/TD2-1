#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>
#include "Fish.h"
#include "BigFish.h"
#include "Rubbish.h"

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

	// 全ての当たり判定
	void CheckAllCollisions();

	//デスフラグのgetter
	bool IsFinished() const { return isFinish; }


	int getTimer_;

	void SpawnFish();


	
private:

	Player* player_ = nullptr;
	Model* model_ = nullptr;
	Model* playerModel_ = nullptr;
	Camera camera_;

	//魚
	//========================================
	// 魚モデル
	Model* fishModel_ = nullptr;

	std::list<Fish*> fishes_;


	//大きい魚のモデル
	Model* bigFishModel_ = nullptr;
	std::list<BigFish*> BigFishes_;

	//ゴミ
	Model* rubbishModel_ = nullptr;
	std::list<Rubbish*> rubbishes_;

	// 現在数をカウント
	int smallCount = 0;
	int bigCount = 0;
	int rubbishCount = 0;

	//終了フラグ
	bool isFinish = false;

	// タイマー
	uint32_t numTexHandles_[10];
	Sprite* numSprite_[3];
	int gameTimer_ = 18000;
	bool isGame_ = true; // ゲーム中か
};
