#include "bearEvent.h"
#include "cassert"
#include <algorithm>
#include <numbers>

void bearEvent::Initialize(
	KamataEngine::Model* model, KamataEngine::Model* bearModel, KamataEngine::Camera* camera,
	KamataEngine::Vector3& position, KamataEngine::Vector3& bearPosition) 
{
	assert(model);
	assert(bearModel);
	// ルアー
	model_ = model;
	lureWorldTransform_.Initialize();
	lureWorldTransform_.translation_ = position;
	lureWorldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	// 熊
	bearModel_ = bearModel;
	bearWorldTransform_.Initialize();
	bearWorldTransform_.translation_ = bearPosition;
	bearWorldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	camera_ = camera;

	// イベント発生フラグ
	isBearEvent_ = false;
	isLureThrowTimer_ = false;
	isLureThrow_ = false;
	// 位置リセット
	resetPos_ = position;

	isLeft = false;
	isDown_ = false;
	isUp_ = false;
	upTimer_ = 0;
	downTimer_ = 0;

	// タイマー
	timer_ = 0.0f;
	// 持続時間
	duration_ = 30.0f;
}

void bearEvent::Update() 
{
	if (!isBearEvent_)
	{
		return;
	}

	// タイマーを継続時間分増やす
	timer_ += 1.0f / 60.0f;
	// 一定時間経過したら天気をリセット
	if (timer_ >= duration_)
	{
		isBearEvent_ = false;
		timer_ = 0.0f;
		Reset();
		return;
	}

	// 処理
	// 投げるタイマーを決定
	RandomLureThrow();
	// ルアーの動き
	lureMove();
	// ワールドトランス更新
	WorldTransformUpdate();

}

void bearEvent::Draw() 
{
	if (isBearEvent_) 
	{
		model_->Draw(lureWorldTransform_, *camera_);
		bearModel_->Draw(bearWorldTransform_, *camera_);
	}
}

void bearEvent::lureMove() 
{
	KamataEngine::Vector3 acceleration = {};
	if (!isLureThrow_)
	{
		lureThrowTimer_--;
		if (lureThrowTimer_ < 0)
		{
			isLureThrow_ = true;
			isDown_ = true;
			velocity_.x = 0;
			acceleration.x = 0;
			downTimer_ = 90;
		}
		if (!isLeft) {
			acceleration.x += kLureMoveSpeedX;
		} else {
			acceleration.x -= kLureMoveSpeedX;
		}
		if (lureWorldTransform_.translation_.x > 10.0f && !isLeft) {
			velocity_.x = 0;
			acceleration.x = 0;
			isLeft = true;
		}
		if (lureWorldTransform_.translation_.x < -10.0f && isLeft) {
			velocity_.x = 0;
			acceleration.x = 0;
			isLeft = false;
		}
		velocity_.x += acceleration.x;
	}
	if (isLureThrow_)
	{
		lureUpDown();
		if (isDown_)
		{
			if (velocity_.y > 0.0f) {
				velocity_.y *= (1.0f - kAttenuation);
			}
			acceleration.y -= kLureMoveSpeedY;
		} 
		if (isUp_)
		{
			if (velocity_.y < 0.0f) {
				velocity_.y *= (1.0f - kAttenuation);
			}
			acceleration.y += kLureMoveSpeedY;
		}
		if (lureWorldTransform_.translation_.y < -2.0f) {
			// ルアーが下端に行ったときに
			velocity_.y = 0;
			acceleration.y = 0;
			lureWorldTransform_.translation_.y = -2.0f;
		}
		if (lureWorldTransform_.translation_.y > 9.5f)
		{
			Reset();

		}
		velocity_.y += acceleration.y;
	}	
}

// ルアーを投げるタイミングのタイマー
void bearEvent::RandomLureThrow()
{
	if (!isLureThrowTimer_)
	{
		lureThrowTimer_ = rand() % 30 + (rand() % 60 + 60);
		isLureThrowTimer_ = true;
	}
}

void bearEvent::WorldTransformUpdate()
{
	lureWorldTransform_.translation_.x += velocity_.x;
	lureWorldTransform_.translation_.y += velocity_.y;
	lureWorldTransform_.translation_.z += velocity_.z;
	lureWorldTransform_.matWorld_ = MakeAffineMatrix(lureWorldTransform_.scale_, lureWorldTransform_.rotation_, lureWorldTransform_.translation_);
	lureWorldTransform_.TransferMatrix();

	bearWorldTransform_.matWorld_ = MakeAffineMatrix(bearWorldTransform_.scale_, bearWorldTransform_.rotation_, bearWorldTransform_.translation_);
	bearWorldTransform_.TransferMatrix();
}

void bearEvent::Reset()
{
	KamataEngine::Vector3 acceleration = {};
	lureWorldTransform_.translation_ = resetPos_;
	isLureThrowTimer_ = false;
	isLureThrow_ = false;
	RandomLeftRight();
	isDown_ = false;
	isUp_ = false;
	upTimer_ = 0;
	downTimer_ = 0;
	RandomLureThrow();
	velocity_.y = 0;
	acceleration.y = 0;
}

KamataEngine::Vector3 bearEvent::GetWorldPosition() 
{ // ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分取得
	worldPos.x = lureWorldTransform_.matWorld_.m[3][0];
	worldPos.y = lureWorldTransform_.matWorld_.m[3][1];
	worldPos.z = lureWorldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB bearEvent::GetAABB()
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {(worldPos.x - 0.3f) / 2.0f, (worldPos.y - 0.5f) / 2.0f, (worldPos.z - 2.0f) / 2.0f};
	aabb.max = {(worldPos.x + 0.3f) / 2.0f, (worldPos.y + 0.5f) / 2.0f, (worldPos.z + 2.0f) / 2.0f};

	return aabb;
}

void bearEvent::OnCollision(const Fish* fish)
{ (void)fish; }

void bearEvent::OnCollision(const BigFish* Bigfish) 
{ (void)Bigfish; }

// ルアーの上下のタイマー
void bearEvent::lureUpDown()
{
	// 上がる下がるときのタイマー減算処理
	if (isDown_ && downTimer_ > 0) 
	{
		downTimer_--;
		if (downTimer_ <= 0) 
		{
			isDown_ = false;
			isUp_ = true;
		}
	}
	if (isUp_ && upTimer_ > 0)
	{
		upTimer_--;
		if (upTimer_ <= 0)
		{
			isUp_ = false;
			isDown_ = true;
		}
	}
	// タイマーの設定
	if (isDown_ && downTimer_ <= 0)
	{
		downTimer_ = (rand() % 30 + 20) + (rand() % 30 + 20);
	}
	if (isUp_ && upTimer_ <= 0) 
	{
		upTimer_ = (rand() % 30 + 20) + (rand() % 30 + 20);
	}

}

void bearEvent::RandomLeftRight() 
{
	int random = rand() % 2;
	if (!random)
	{
		isLeft = false;
	}
	else 
	{
		isLeft = true;
	}
}
