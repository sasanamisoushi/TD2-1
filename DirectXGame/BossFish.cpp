#include "BossFish.h"
#include "Player.h"
#include "Score.h"
#include "bearEvent.h"
#include <cassert>
#include <cstdlib>
#include <numbers>

void BossFish::Initialize(Model* model, Camera* camera, Score* score, KamataEngine::Vector3& position, int getTimer) 
{

	// スポーンの範囲を５～-２にする

	// NULLポインタチェック
	assert(model);
	// モデル
	model_ = model;
	// カメラ
	camera_ = camera;

	score_ = score;

	// 魚がゲットできる時間
	fishGetTimer_ = getTimer;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.scale_ = {0.8f, 0.6f, 0.8f}; // 最初は小さめ
	isLeft_ = false;	

	isBossEvent_ = false;
	isBossSpoon_ = false;

	bossBgm_ = Audio::GetInstance()->LoadWave("./BGM/In the Sweet By and By.mp3");
}

void BossFish::Update() 
{
	if (!isBossSpoon_) 
	{
		return;
	}
	bgm_->BGMPlay(bossBgm_);
	KamataEngine::Vector3 acceleration = {};
	if (!isLeft_) {
		acceleration.x += kLureMoveSpeedX;
	} else {
		acceleration.x -= kLureMoveSpeedX;
	}
	if (worldTransform_.translation_.x >screenRight  && !isLeft_) {
		velocity_.x = 0;
		acceleration.x = 0;
		RandomPosY();
		worldTransform_.translation_.y = spoonPos_.y;
		worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
		isLeft_ = true;
	}
	if (worldTransform_.translation_.x <screenLeft && isLeft_) {
		velocity_.x = 0;
		acceleration.x = 0;
		RandomPosY();
		worldTransform_.translation_.y = spoonPos_.y;
		worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
		isLeft_ = false;
	}
	velocity_.x += acceleration.x;
	WorldTransformUpdate();
}

void BossFish::Draw()
{
	if (isBossSpoon_)
	{
		model_->Draw(worldTransform_, *camera_);
	}
	
}

KamataEngine::Vector3 BossFish::GetWorldPosition()
{ // ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void BossFish::WorldTransformUpdate() 
{
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

AABB BossFish::GetAABB() {
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {(worldPos.x - 0.5f) / 2.0f, (worldPos.y - 0.5f) / 2.0f, (worldPos.z - 2.0f) / 2.0f};
	aabb.max = {(worldPos.x + 0.5f) / 2.0f, (worldPos.y + 0.5f) / 2.0f, (worldPos.z + 2.0f) / 2.0f};

	return aabb;
}

void BossFish::RandomPosY() 
{ 
	spoonPos_.y =float (rand() % 7 - 2);
}

void BossFish::OnCollision(Player* player)
{
	if (!isBossSpoon_) {
		return;
	}

	// ルアーと当たっているとき
	(void)player;
	// ゲットタイマーを減らす
	fishGetTimer_--;
	// ゲットタイマーが0になったらゲット
	if (fishGetTimer_ < 0) {
		isLureCheck_ = true;
		score_->AddScoreCombo(point_);
		isBossSpoon_ = false;
		bgm_->BGMStop();
	}
}
