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
	Fish::OnCollision(player);

	if (!hasTriggered_) {
		hasTriggered_ = true;
		isLureCheck_ = true;
		
		if (isLureCheck_) {
			// BigFishはスコア加算
			score_->AddScore(point_);
		}

		// イベント発生（群れを出すなど）
		if (onTriggered_) {
			onTriggered_(GetWorldPosition(), eventType_);
		}
	}
}

AABB EventFish::GetAABB() { 
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {(worldPos.x - 0.5f) / 2.0f, (worldPos.y - 0.5f) / 2.0f, (worldPos.z - 2.0f) / 2.0f};
	aabb.max = {(worldPos.x + 0.5f) / 2.0f, (worldPos.y + 0.5f) / 2.0f, (worldPos.z + 2.0f) / 2.0f};

	return aabb;
}
