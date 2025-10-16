#include "EventFish.h"
#include "GameScene.h"

void EventFish::Initialize(Model* model, Camera* camera, const Vector3& pos, bool moveRight, int getTimer, Event* event) {
	Fish::Initialize(model, camera, pos, moveRight,getTimer);
	event_ = event;

}

void EventFish::Update() { 
	Fish::Update(); }

void EventFish::OnCollision(Player* player) {
	Fish::OnCollision(player);
	if (!hasTriggered_ && event_) {
		hasTriggered_ = true;
		// イベントを開始
		GameScene* scene = GameScene::GetInstance();
		scene->StartEvent(event_);
	}
}
