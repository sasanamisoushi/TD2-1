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

	// 全ての当たり判定
	void CheckAllCollisions();

	//デスフラグのgetter
	bool IsFinished() const { return isFinish; }

	int getTimer_;

	
private:

	Player* player_ = nullptr;
	Model* model_ = nullptr;
	Model* playerModel_ = nullptr;
	Camera camera_;

	// 魚モデル
	Model* fishModel_ = nullptr;

	std::list<Fish*> fishes_;

	//終了フラグ
	bool isFinish = false;
};
