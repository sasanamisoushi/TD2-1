#pragma once
#include "KamataEngine.h"
#include "math.h"

using namespace KamataEngine;

class Player;

class Fish {
public:
	// 初期化（座標と速度方向を指定）
	void Initialize(Model* model, Camera* camera, const Vector3& position, bool moveRight);

	// 更新
	void Update();

	// 描画
	void Draw();

	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }

	KamataEngine::Vector3 GetWorldPosition();

	AABB GetAABB();

	// ルアーと当たっている
	void OnCollision(const Player* player);
	// ルアーと当たってない
	void OutCollision();

	// フラグ
	// 魚がルアーの範囲内にいるか
	bool isLureCheck_ = false;
	bool IsLureCheck() const { return isLureCheck_; }

	// 魚がゲットできる時間
	int fishGetTimer_;

	
private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	//カメラ
	Camera* camera_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//向き
	Vector3 direction_;
	
	// 速度
	Vector3 velocity_ = {};

	static inline const float speed_ = 0.01f;
	float radius_ = 1.0f;

	float leftLimit_ = 0.0f;
	float rigdhtLimit_ = 0.0f;

	
	
};
