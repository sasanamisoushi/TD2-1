#pragma once
#include "Fish.h"
#include "Event.h"

class EventFish : public Fish {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& pos, bool moveRight, int getTimer, Event* event);
	void Update() override;
	void OnCollision(Player* player) override;

private:
	Event* event_ = nullptr;
	bool hasTriggered_ = false;
};
