#pragma once
#include "Fish.h"
#include "KamataEngine.h"
#include "math.h"

class Fish;
class BigFish;

class bearEvent 
{
public:
	// 初期化
	void Initialize(
		KamataEngine::Model* model, KamataEngine::Model* bearModel, KamataEngine::Camera* camera,
		KamataEngine::Vector3& position, KamataEngine::Vector3& bearPosition);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
	// ルアーの動く処理
	void lureMove();
	// ルアーを投げるタイミングの時間をランダムにさせる
	void RandomLureThrow();
	// ワールドトランス処理
	void WorldTransformUpdate();
	// 釣ったらリセット
	void Reset();

	KamataEngine::Vector3 GetWorldPosition();

	AABB GetAABB();
	// 小さい魚
	void OnCollision(const Fish* fish);

	// 大きい魚
	void OnCollision(const BigFish* Bigfish);

	void lureUpDown();

private:
	
	// ルアー
	WorldTransform lureWorldTransform_;
	Model* model_ = nullptr;
	// 熊
	WorldTransform bearWorldTransform_;
	Model* bearModel_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;

	KamataEngine::Vector3 velocity_ = {};

	// タイマー
	float timer_ = 0.0f;
	// 持続時間
	float duration_ = 120.0f;

	// イベント発生フラグ
	int isBearEvent_;
	// ルアーを投げるタイミングのタイマー
	int lureThrowTimer_;
	// ルアーを投げるタイミングのタイマーが決定したときに止めるためのフラグ
	int isLureThrowTimer_;
	// ルアーを投げるフラグ
	int isLureThrow_;
	// ルアーが右に行った時に使う
	int isLeft; 
	// ルアーの移動
	static inline const float kLureMoveSpeedX = 0.001f;
	static inline const float kLureMoveSpeedY = 0.001f;
	static inline const float kAttenuation = 0.03f;
	// ルアーの位置のリセット
	Vector3 resetPos_;

	// ルアーを下げる
	// タイマー
	int downTimer_;
	// フラグ
	int isDown_;

	// ルアーを上げる
	// タイマー
	int upTimer_;
	// フラグ
	int isUp_;

};
