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
	 Model* num0Model_=nullptr; 
	 Model* num1Model_=nullptr; 
	 Model* num2Model_=nullptr; 
	 Model* num3Model_=nullptr; 
	 Model* num4Model_=nullptr; 
	 Model* num5Model_=nullptr; 
	 Model* num6Model_=nullptr; 
	 Model* num7Model_=nullptr; 
	 Model* num8Model_=nullptr; 
	 Model* num9Model_=nullptr;

	static const int kGameTimer_ = 1800;
	int gameTimer_ = 0;
	WorldTransform numderTransform_;
	int currentNumber_ = 0;
	bool isGame_ = false;
};
