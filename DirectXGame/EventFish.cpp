#include "EventFish.h"
#include "GameScene.h"

void EventFish::Initialize(Model* model, Camera* camera, const Vector3& pos, bool moveRight, int getTimer) {
	Fish::Initialize(model, camera, pos, moveRight,getTimer);
}

void EventFish::Update() { 
	Fish::Update(); }

void EventFish::OnCollision(Player* player) {
	Fish::OnCollision(player);

	if (!hasTriggered_) {
		hasTriggered_ = true;
		isLureCheck_ = true;

		// イベント発生（群れを出すなど）
		if (onTriggered_) {
			onTriggered_(GetWorldPosition());
		}
	}
}
