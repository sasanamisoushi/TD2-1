#pragma once
#include "KamataEngine.h"
#include "math.h"

using namespace KamataEngine;

class Rubbish {
public:

	//初期化
	void Initialize(Model* model, Camera* camera, const Vector3& targetPos, bool moveRight);

	// 更新
	void Update();

	// 描画
	void Draw();

	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }

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

	// 速度
	Vector3 velocity_ = {};

	static inline const float speed_ = 0.01f;
	float radius_ = 1.0f;

	float leftLimit_ = 0.0f;
	float rigdhtLimit_ = 0.0f;


};
