#include "BigFish.h"
#include "math.h"
#include <cstdlib>
#include <cassert>
#include <numbers>

void BigFish::Initialize(Model* model, Camera* camera, const Vector3& position, bool moveRight) {
	// NULLポインタチェック
	assert(model);
	// モデル
	model_ = model;

	// カメラ
	camera_ = camera;

	// 画面の範囲
	const float screenLeft = -12.0f;
	const float screenRight = 12.0f;

	// 泳ぐ範囲ランダム
	float swimRange = static_cast<float>(rand() % 5 + 4);

	// 出現ランダム
	float minPos = screenLeft + swimRange;
	float maxPos = screenRight - swimRange;
	float randomPos = minPos + static_cast<float>(rand()) / RAND_MAX * (maxPos - minPos);

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {0.35f, 0.35f, 0.35f};
	worldTransform_.translation_.x = randomPos;

	// 泳ぐ上限
	leftLimit_ = worldTransform_.translation_.x - swimRange;
	rigdhtLimit_ = worldTransform_.translation_.x + swimRange;

	// 移動方向
	direction_ = moveRight ? Vector3(1.0f, 0.0f, 0.0f) : Vector3(-1.0f, 0.0f, 0.0f);

	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	// 角度調整
	if (moveRight) {
		// 右
		worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	} else {
		// 左
		worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	}


}

void BigFish::Update() {
	// 移動
	worldTransform_.translation_.x += direction_.x * speed_;

	// 端で反転（ヒステリシスを持たせる）
	if (worldTransform_.translation_.x > rigdhtLimit_ + 0.1f) {
		direction_.x = -1.0f; // 左へ
		velocity_.x = direction_.x * speed_;
		worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f; // 左向きに回転
		worldTransform_.translation_.x = rigdhtLimit_;
	} else if (worldTransform_.translation_.x < leftLimit_ - 0.1f) {
		direction_.x = 1.0f; // 右へ
		velocity_.x = direction_.x * speed_;
		worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f; // 右向きに回転
		worldTransform_.translation_.x = leftLimit_;
	}

	// 行列更新
	WorldTransformUpdate(worldTransform_);

}

void BigFish::Draw() {
	// 3Dモデル描画前処理
	model_->Draw(worldTransform_, *camera_);

}
