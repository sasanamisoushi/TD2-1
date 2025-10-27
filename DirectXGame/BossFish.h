#pragma once
#include "KamataEngine.h"
#include "math.h"
#include "Score.h"
#include "BGM.h"

using namespace KamataEngine;

class Player;

class Score;

class BossFish
{
public:

	// 初期化（座標と速度方向を指定）

	void Initialize(Model* model, Camera* camera, Score* score, KamataEngine::Vector3& position, int getTimer);

	// 更新
	virtual void Update();

	// 描画
	void Draw();

	KamataEngine::Vector3 GetWorldPosition();

	// ワールドトランス処理
	void WorldTransformUpdate();

	AABB GetAABB();

	void RandomPosY();

	// 魚がゲットできる時間
	int fishGetTimer_;

	// ルアーと当たっている
	virtual void OnCollision(Player* player);

	// フラグ
	// 魚がルアーの範囲内にいるか
	bool isLureCheck_ = false;
	bool IsLureCheck() const { return isLureCheck_; }

	// イベント発生フラグ
	int isBossEvent_;
	// スポーン
	int isBossSpoon_;

private:

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Score* score_;

	BGM* bgm_ = nullptr;

	uint32_t bossBgm_;

	// 向き
	Vector3 direction_;

	// 速度
	Vector3 velocity_ = {};

	 // 目的位置
	Vector3 spoonPos_;

	static inline const float speed_ = 0.01f;
	float radius_ = 1.0f;

	float leftLimit_ = 0.0f;
	float rightLimit_ = 0.0f;

	int isLeft_ = false;

	// 画面の範囲
	const float screenLeft = -14.0f;
	const float screenRight = 14.0f;

	static inline const float kLureMoveSpeedX = 0.001f;

	const int point_ = 100000;

};
