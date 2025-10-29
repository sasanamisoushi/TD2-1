#include "EventFish.h"
#include "GameScene.h"

void EventFish::Initialize(Model* model, Camera* camera, Score* Score, const Vector3& pos, bool moveRight) {

	score_ = Score;
	Fish::Initialize(model, camera, Score, pos, moveRight, 90);
}

void EventFish::Update() {
	Fish::Update();

	// 行列更新
	WorldTransformUpdate(GetWorldTransform());
}

void EventFish::OnCollision(Player* player) {
	if (!isAlive_)
		return; // すでに死んでたら無視

	// ゲット判定
	fishGetTimer_--;

	if (fishGetTimer_ < 0 && !isLureCheck_) {
		isLureCheck_ = true;
		score_->AddScore(point_);

		// ★ 重複実行を防ぐため即座に無効化！
		isAlive_ = false;

		// プレイヤーをリセット
		player->Reset();
	}

	
}

AABB EventFish::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {(worldPos.x - 0.5f) / 2.0f, (worldPos.y - 0.5f) / 2.0f, (worldPos.z - 2.0f) / 2.0f};
	aabb.max = {(worldPos.x + 0.5f) / 2.0f, (worldPos.y + 0.5f) / 2.0f, (worldPos.z + 2.0f) / 2.0f};

	return aabb;
}
