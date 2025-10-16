#pragma once
#include "KamataEngine.h"
#include "math.h"

using namespace KamataEngine;

class Player;

class BigFish {
public:
	// 初期化（座標と速度方向を指定）
	void Initialize(Model* model, Camera* camera, const Vector3& position, bool moveRight);

	// 更新
	void Update();

	// 描画
	void Draw();

	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }

	AABB GetAABB();

	// ルアーと当たっている
	void OnCollision(Player* player);

	// ルアーと当たってない
	void OutCollision();

	// フラグ
	bool isLureCheck_ = false;
	bool IsLureCheck() const { return isLureCheck_; }

	// 魚がゲットできる時間
	int fishGetTimer_ = 30;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 向き
	Vector3 direction_;

	// 速度
	Vector3 velocity_{};

	// 定数速度
	static inline const float speed_ = 0.05f;

	// 半径
	float radius_ = 1.0f;

	// 泳ぐ範囲
	float leftLimit_ = 0.0f;
	float rigdhtLimit_ = 0.0f;
};
