#include "BigFish.h"
#include <cstdlib>
#include <cassert>
#include <numbers>
#include "Player.h"
#include "Score.h"



void BigFish::Initialize(Model* model, Camera* camera, Score* score, const Vector3& targetPos, bool moveRight) {
	

	score_ = score;
	targetPos_ = targetPos;
	State_ = BigFishState::Appear;

	// Fishの基本初期化（共通部分）
	Fish::Initialize(model, camera, score, targetPos, moveRight, 30);

	// BigFish専用パラメータ
	finalScale_ = {0.35f, 0.35f, 0.35f};
	GetWorldTransform().scale_ = {0.01f, 0.01f, 0.01f}; // 出現時は小さい

	WorldTransformUpdate(GetWorldTransform());
}

void BigFish::Update() {

	switch (State_) {
	case BigFishState::Appear:
		appearTimer_++;

		// 線形補間して少しずつ目的地へ
		GetWorldTransform().translation_ = Lerp(GetWorldTransform().translation_, targetPos_, 0.05f);
		GetWorldTransform().scale_ = Lerp(GetWorldTransform().scale_, finalScale_, 0.05f);

		// 一定時間経過で通常行動へ
		if (appearTimer_ > appearDuration_) {
			State_ = BigFishState::Normal;
		}
		break;

	case BigFishState::Normal:
		//通常の魚の移動を継続
		Fish::Update();
		// 常にスケールを固定
		GetWorldTransform().scale_ = finalScale_;
		break;
	}

	// 行列更新
	WorldTransformUpdate(GetWorldTransform());

}





AABB BigFish::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	float sizeScale = 1.5f; // 通常魚より大きめ
	AABB aabb;
	aabb.min = {worldPos.x - 0.5f * sizeScale, worldPos.y - 0.5f * sizeScale, worldPos.z - 0.5f * sizeScale};
	aabb.max = {worldPos.x + 0.5f * sizeScale, worldPos.y + 0.5f * sizeScale, worldPos.z + 0.5f * sizeScale};

	return aabb;
}

void BigFish::OnCollision(Player* player)
{
	// Fishの共通当たり処理（タイマー処理など）
	Fish::OnCollision(player);

	if (isLureCheck_) {
		// BigFishはスコア加算
		score_->AddScore(point_);
	}
	
}

void BigFish::OnCollision(bearEvent* bearEvent)
{
	
	// Fishの共通当たり処理（タイマー処理など）
	Fish::OnCollision(bearEvent);

	if (isLureCheck_) {
		// BigFishはスコア加算
		score_->AddScore(point_);
	}
}

void BigFish::OutCollision() {

	 Fish::OutCollision();


}