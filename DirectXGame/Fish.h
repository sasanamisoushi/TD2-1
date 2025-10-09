#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Fish {
public:
	// 初期化（座標と速度方向を指定）
	void Initialize(Model* model, Camera* camera, const Vector3& position);

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

	//カメラ
	Camera* camera_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//Vector3 direction_;
	
	// 速度
	Vector3 velocity_ = {};

	static inline const float speed_ = 0.01f;
	float radius_ = 1.0f;

	float leftLimit_ = -100.0f;
	float rigdhtLimit_ = 100.0f;
};
