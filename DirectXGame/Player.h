#pragma once
#include "KamataEngine.h"
#include "math.h"

#include <vector>

class Player 
{

private:

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform playerWorldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0u;

	KamataEngine::Vector3 velocity_ = {};

	
	
	// ルアーの移動
	static inline const float kLureMoveSpeedX = 0.001f;

	static inline const float kLureMoveSpeedY = 5;
	// プレイヤーの描画座標
	float playerDrawPosX;
	float playerDrawPosY;

	// フラグ
	int isLureThrow; // ルアーが投げてある
	int isKeyPush; //スペースを押してる

	int isLeft; // ルアーが右に行った時に使う

public:

	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Model* playerModel, KamataEngine::Camera* camera, 
		KamataEngine::Vector3& position, KamataEngine::Vector3& playerPosition);
	// 更新
	void Update();
	// ボタン処理
	void InputMove();
	// ワールドトランス処理
	void WorldTransformUpdate();
	// 描画
	void Draw();

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	Vector3 GetPlayerPos() const { return playerWorldTransform_.translation_; }

	Vector3 GetLurePos() const { return worldTransform_.translation_; }

	KamataEngine::Vector3 GetWorldPosition();

	AABB GetAABB();

};
