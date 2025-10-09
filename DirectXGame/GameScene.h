#pragma once
#include "KamataEngine.h"
#include "Fish.h"

class GameScene {
public:

	//初期化
	void Initialize();

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
