#pragma once
#include "Fish.h"
#include "KamataEngine.h"
#include "math.h"

class weatherEvent {
public:
	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:
	// カメラ
	Camera* camera_ = nullptr;
};
