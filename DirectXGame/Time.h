#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Time 
{
public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 残り時間（フレーム）
	int GetGameTimer() const { return gameTimer_; }

	// 最大寿命（定数）
	int GetMaxGameTime() const { return kGameTime; }

	//時間切れか？
	bool isTimeUp() const { return isGame; }

	int GetSecondTime() const { return seconds; }

private:
	//WorldTransform worldTransform_;
	//タイムモデル
	//Model* model_=nullptr;
	//最大時間
	static const int kGameTime = 18000;
	//制限時間
	int gameTimer_ = 0;
	//カメラ
	//Camera camera_;
	//ゲーム中か
	bool isGame = false;

	 // 秒に変換
	int seconds = gameTimer_ / 60;
};
