#include "Fish.h"
#include <cstdlib>
#include <cassert>
#include <numbers>
#include "Player.h"
#include "bearEvent.h"
#include "Score.h"


void Fish::Initialize(Model* model, Camera* camera, Score* score, const Vector3& targetPos, bool moveRight, int getTimer) {
  
	// NULLポインタチェック
	assert(model);
	// モデル
	model_ = model;

	// カメラ
	camera_ = camera;

	state_ = FishState::Appear;

	targetPos_ = targetPos;


	moveRight_ = moveRight;
  
	score_ = score;

	
	Vector3 spawnPos;
	//出現開始位置
	if (rand() % 2 == 0) {
		spawnPos = {moveRight ? -15.0f : 15.0f, targetPos_.y, targetPos_.z};
	} else {
		spawnPos = {0.0f, targetPos_.y, targetPos_.z + 20.0f}; // 奥から
	}

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = spawnPos;
	worldTransform_.scale_ = {0.01f, 0.01f, 0.01f}; // 最初は小さめ

	// 出現完了後の最終サイズ
	finalScale_ = {0.2f, 0.2f, 0.2f};

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
	fishGetTimer_ = getTimer;
	resetTimer_ = getTimer;

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

	WorldTransformUpdate(worldTransform_);
}

void Fish::Update() {
	switch (state_) {
	case FishState::Appear:
		appearTimer_++;

		// 線形補間して少しずつ目的地へ
		worldTransform_.translation_ = Lerp(worldTransform_.translation_, targetPos_, 0.05f);
		worldTransform_.scale_ = Lerp(worldTransform_.scale_, finalScale_, 0.05f);

		// 一定時間経過で通常行動へ
		if (appearTimer_ > appearDuration_) {
			state_ = FishState::Normal;
		}
		break;

	case FishState::Normal:
		// 移動
		worldTransform_.translation_.x += direction_.x * speedMultiplier_;

		// 端で反転（ヒステリシスを持たせる）
		if (!inEvent_) {
			if (worldTransform_.translation_.x > rigdhtLimit_ + 0.1f) {
				direction_.x = -1.0f; // 左へ
				velocity_.x = direction_.x * speedMultiplier_;
				worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f; // 左向きに回転
				worldTransform_.translation_.x = rigdhtLimit_;
			} else if (worldTransform_.translation_.x < leftLimit_ - 0.1f) {
				direction_.x = 1.0f; // 右へ
				velocity_.x = direction_.x * speedMultiplier_;
				worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f; // 右向きに回転
				worldTransform_.translation_.x = leftLimit_;
			}
		}
		break;
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

	aabb.min = {worldPos.x - 0.5f / 2.0f, worldPos.y - 0.5f / 2.0f, worldPos.z - 0.5f / 2.0f};
	aabb.max = {worldPos.x + 0.5f / 2.0f, worldPos.y + 0.5f / 2.0f, worldPos.z + 0.5f / 2.0f};

	return aabb;
}

void Fish::OnCollision(Player* player) 
{
	// ルアーと当たっているとき
	(void)player;
	// ゲットタイマーを減らす
	fishGetTimer_--;
	// ゲットタイマーが0になったらゲット
	if (fishGetTimer_ < 0) 
	{
		isLureCheck_ = true;
		score_->AddScoreCombo(point_);
		player->Reset();
	}
}

void Fish::OnCollision(bearEvent* bearEvent)
{
	// ルアーと当たっているとき
	(void)bearEvent;
	// ゲットタイマーを減らす
	fishGetTimer_-= 9;
	// ゲットタイマーが0になったらゲット
	if (fishGetTimer_ < 0) {
		isLureCheck_ = true;
		score_->AddScore((point_ + 50));
		bearEvent->Reset();
	}
}

void Fish::OutCollision()
{ 
	// ルアーと当たっていないとき
	// ゲットタイマーをリセット
	fishGetTimer_ = resetTimer_; 
}

void Fish::SetTexture(const std::string& filePath) { 
	
	textureHandle_ = TextureManager::Load(filePath);

}

float Fish::GetMoveDirectionY() const { 
	return moveRight_ ? 1.0f : -1.0f; 
}

void Fish::SetWorldPosition(const Vector3& pos) {
	worldTransform_.translation_ = pos; 
}

void Fish::SetSpeedMultiplier(float multiplier) { speedMultiplier_ = multiplier; }
