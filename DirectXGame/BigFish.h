#pragma once
#include "KamataEngine.h"
#include "math.h"
#include "Fish.h"

using namespace KamataEngine;

class Player;

class bearEvent;

class Score;

enum class BigFishState {
	Appear, // 登場中
	Normal, // 通常行動
};

class BigFish : public Fish{
public:
	// 初期化（座標と速度方向を指定）
	void Initialize(Model* model, Camera* camera, Score* score, const Vector3& targetPos, bool moveRight);

	// 更新
	void Update() override;

	
	AABB GetAABB();

	// ルアーと当たっている
	void OnCollision(Player* player) override;

	// ルアーと当たっている
	void OnCollisionBear(bearEvent* bearEvent) override;

	// ルアーと当たってない
	void OutCollision() override;

	void OutCollisionBear() override;

private:
	BigFishState State_ = BigFishState::Appear;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	Score* score_;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	 bool isAlive_ = true;

	 // 出現アニメ用
	Vector3 targetPos_{};
	Vector3 finalScale_{};
	float appearTimer_ = 0.0f;
	float appearDuration_ = 60.0f;

	const int point_ = 600;
};
