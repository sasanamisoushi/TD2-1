#include "Fish.h"
#include "math.h"
#include <cstdlib>
#include <cassert>
#include <numbers>

void Fish::Initialize(Model* model, Camera* camera, const Vector3& position, bool moveRight) {
	// NULLポインタチェック
	assert(model);
	// モデル
	model_ = model;

	// カメラ
	camera_ = camera;

	//画面の範囲
	const float screenLeft = -14.0f;
	const float screenRight = 14.0f;

	// 泳ぐ範囲ランダム
	float swimRange = static_cast<float>(rand() % 5 + 4);

	//出現ランダム
	float minPos = screenLeft + swimRange;
	float maxPos = screenRight - swimRange;
	float randomPos = minPos + static_cast<float>(rand()) / RAND_MAX * (maxPos - minPos);

	// 魚がゲットできる時間
	fishGetTimer_ = 90;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {0.3f, 0.3f, 0.3f};
	worldTransform_.translation_.x = randomPos;

	
		// 泳ぐ上限
	leftLimit_ = worldTransform_.translation_.x - swimRange;
	rigdhtLimit_ = worldTransform_.translation_.x + swimRange;

	//移動方向
	direction_ = moveRight ? Vector3(1.0f, 0.0f, 0.0f) : Vector3(-1.0f, 0.0f, 0.0f);

	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	// 角度調整
	if (moveRight)
	{
		//右
		worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	} else {
		//左
		worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	}
}

void Fish::Update() {

	// 移動
	worldTransform_.translation_.x += direction_.x*speed_;

	// 端で反転（ヒステリシスを持たせる）
	if (worldTransform_.translation_.x > rigdhtLimit_+0.1f) {
		direction_.x = -1.0f;                   // 左へ
		velocity_.x = direction_.x * speed_;
		worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f; // 左向きに回転
		worldTransform_.translation_.x = rigdhtLimit_;
	} else if (worldTransform_.translation_.x < leftLimit_-0.1f) {
		direction_.x = 1.0f;                     // 右へ
		velocity_.x = direction_.x * speed_;
		worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f; // 右向きに回転
		worldTransform_.translation_.x = leftLimit_;
	}

	// 行列更新
	WorldTransformUpdate(worldTransform_);

	

}

void Fish::Draw() { 
	// 3Dモデル描画前処理
	model_->Draw(worldTransform_, * camera_);
}

KamataEngine::Vector3 Fish::GetWorldPosition() 
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Fish::GetAABB() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {(worldPos.x - 0.5f) / 2.0f, (worldPos.y - 0.5f) / 2.0f, (worldPos.z - 0.5f) / 2.0f};
	aabb.max = {(worldPos.x + 0.5f) / 2.0f, (worldPos.y + 0.5f) / 2.0f, (worldPos.z + 0.5f) / 2.0f};

	return aabb;
}

void Fish::OnCollision(const Player* player) 
{
	// ルアーと当たっているとき
	(void)player; 
	// ゲットタイマーを減らす
	fishGetTimer_--;
	// ゲットタイマーが0になったらゲット
	if (fishGetTimer_ < 0) 
	{
		isLureCheck_ = true;
	}
	
}

void Fish::OutCollision()
{ 
	// ルアーと当たっていないとき
	// ゲットタイマーをリセット
	fishGetTimer_ = 90; 
}
