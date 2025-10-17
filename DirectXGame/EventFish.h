#pragma once
#include "Fish.h"
#include <functional>

class EventFish : public Fish {
public:
	void Initialize(Model* model, Camera* camera, Score* Score, const Vector3& pos, bool moveRight, int getTimer);
	void Update() override;
	void OnCollision(Player* player) override;

	 // イベント発火時に呼ばれるコールバック
	void SetOnTriggered(std::function<void(const Vector3&)> callback) { onTriggered_ = callback; }


private:

	bool hasTriggered_ = false;
	std::function<void(const Vector3&)> onTriggered_;
};
