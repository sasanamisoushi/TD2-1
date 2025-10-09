#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>
#include "Fish.h"

class GameScene 
{

	Player* player_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;
	KamataEngine::Camera camera_;

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

	//解放
	void Finalize();

private:



	// カメラ
	Camera camera_;


	// 魚モデル
	Model* fishModel_ = nullptr;

	std::vector<Fish*> fishes_;

	//終了フラグ
	bool isFinish = false;
};
