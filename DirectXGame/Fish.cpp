#include "Fish.h"
#include "math.h"
#include <DirectXMath.h>
using namespace DirectX;

#include <cstdlib>
#include <cassert>
#include <numbers>

void Fish::Initialize(Model* model, Camera* camera, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	// モデル
	model_ = model;

	// カメラ
	camera_ = camera;

	// 自分の位置を中心に５ずつの範囲で泳ぐ
	leftLimit_ = position.x - 5.0f;
	rigdhtLimit_ = position.x + 5.0f;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	
	
	//移動方向
	//direction_ = moveRight ? Vector3(1.0f, 0.0f, 0.0f) : Vector3(-1.0f, 0.0f, 0.0f);

	// 速度ベクトルを方向に基づいて設定
	velocity_ += {-speed_,0.0f,0.0f};

	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	// 角度調整
	//if (moveRight) {
	//	//右
	//	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	//} else {
	//	//左
	//	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	//}
}

void Fish::Update() {

	// 移動
	worldTransform_.translation_ +=  velocity_;

	// 端で反転（ヒステリシスを持たせる）
	//if (worldTransform_.translation_.x > rigdhtLimit_+0.1f) {
	//	direction_.x = -1.0f;                   // 左へ
	//	velocity_.x = direction_.x * speed_;
	//	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f; // 左向きに回転
	//	worldTransform_.translation_.x = rigdhtLimit_;
	//} else if (worldTransform_.translation_.x < leftLimit_-0.1f) {
	//	direction_.x = 1.0f;                     // 右へ
	//	velocity_.x = direction_.x * speed_;
	//	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f; // 右向きに回転
	//	worldTransform_.translation_.x = leftLimit_;
	//}

	

	// 行列更新
	WorldTransformUpdate(worldTransform_);
}

void Fish::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_, * camera_);
	Model::PostDraw();
}
