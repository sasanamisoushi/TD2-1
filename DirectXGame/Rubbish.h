#pragma once
#include "KamataEngine.h"
#include "math.h"

using namespace KamataEngine;

class Player;

class Score;

enum class RubbishState {
	Appear, // 登場中
	Normal, // 通常行動
};


class Rubbish {
public:

	//初期化
	void Initialize(Model* model, Camera* camera, Score* score, const Vector3& targetPos, bool moveRight);

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
	int fishGetTimer_;
	// タイマーのリセット
	int resetTimer_;

	void SetSpeedMultiplier(float multiplier);
	float GetSpeedMultiplier() const { return speedMultiplier_; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 向き
	Vector3 direction_;

	Score* score_;

	// 速度
	Vector3 velocity_ = {};

	static inline const float speed_ = 0.01f;
	float radius_ = 1.0f;

	float leftLimit_ = 0.0f;
	float rigdhtLimit_ = 0.0f;

	RubbishState state_ = RubbishState::Appear;

	// 目的位置
	Vector3 targetPos_;

	// 登場演出タイマー
	float appearTimer_ = 0.0f;

	// 約1秒で登場
	float appearDuration_ = 60.0f;

	Vector3 finalScale_;

	const int point_ = 100;

	float speedMultiplier_ = 0.01f; // 天気などによる速度補正

};
