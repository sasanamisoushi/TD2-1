#include "Player.h"
#include "cassert"
#include <algorithm>
#include <numbers>

using namespace KamataEngine;
using namespace MathUtility;

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Model* playerModel, KamataEngine::Camera* camera, KamataEngine::Vector3& position, KamataEngine::Vector3& playerPosition) {
	assert(model);
	assert(playerModel);
	model_ = model;
	playerModel_ = playerModel;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	playerWorldTransform_.Initialize();
	playerWorldTransform_.translation_ = playerPosition;
	playerWorldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	camera_ = camera;

	isLureThrow = false;
	isKeyPush = false;
	isLeft = false;

}

void Player::Update() {
	InputMove();
	WorldTransformUpdate();
}

void Player::InputMove() {

	// ルアーの投げる場所決定の処理
	if (!isLureThrow) 
	{
		KamataEngine::Vector3 acceleration = {};
		if (!isLeft)
		{
			acceleration.x += kLureMoveSpeedX;
		}
		else
		{
			acceleration.x -= kLureMoveSpeedX;
		}
		if (worldTransform_.translation_.x > 10.0f && !isLeft)
		{
			velocity_.x = 0;
			acceleration.x = 0;
			isLeft = true;
		}
		if (worldTransform_.translation_.x < -10.0f && isLeft)
		{
			velocity_.x = 0;
			acceleration.x = 0;
			isLeft = false;
		}
		if (KamataEngine::Input::GetInstance()->PushKey(DIK_P)) 
		{
			velocity_.x = 0;
			acceleration.x = 0;
		    isLureThrow = true;
		}
		velocity_.x += acceleration.x;
	}
	// 魚を釣る
	if (isLureThrow)
	{

		KamataEngine::Vector3 acceleration = {};
		if (!isKeyPush) {
			acceleration.y -= kLureMoveSpeedY;
		} else {
			acceleration.y += kLureMoveSpeedY;
		}
		if (worldTransform_.translation_.y < -2.5f && !isKeyPush) {
			// ルアーが下端に行ったときに
			velocity_.y = 0;
			acceleration.y = 0;
		}
		if (KamataEngine::Input::GetInstance()->PushKey(DIK_SPACE)) {
			isKeyPush = true;
		} else {
			isKeyPush = false;
		}
		velocity_.y += acceleration.y;
	}
}

void Player::WorldTransformUpdate()
{
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	model_->Draw(worldTransform_, *camera_);
	playerModel_->Draw(playerWorldTransform_, *camera_);
}

KamataEngine::Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() {
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {(worldPos.x - 0.5f) / 2.0f, (worldPos.y - 0.5f) / 2.0f, (worldPos.z - 0.5f) / 2.0f};
	aabb.max = {(worldPos.x + 0.5f) / 2.0f, (worldPos.y + 0.5f) / 2.0f, (worldPos.z + 0.5f) / 2.0f};

	return aabb;
}

void Player::OnCollision(const Fish* fish) 
{
	(void)fish; 
}

void Player::OnCollision(const BigFish* Bigfish) 
{

	(void)Bigfish;

}
