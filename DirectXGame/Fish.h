#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Fish {
public:
	// 初期化（座標と速度方向を指定）
	void Initialize(const Vector2& startPos, bool moveRight);

	// 更新
	void Update();

	// 描画
	void Draw();

	// 位置取得
	const Vector2& GetPosition() const { return pos_; }

	// 当たり判定用
	float GetRadius() const { return radius_; }

private:
	Vector2 pos_;
	Vector2 velocity_;
	float speed_ = 2.0f;
	float radius_ = 16.0f;
	int textureHandle_ = -1;

	bool moveRight_ = true;
};
