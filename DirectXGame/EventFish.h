#pragma once
#include "Fish.h"
#include <functional>



class EventFish : public Fish {
public:

	enum class FishEventType {
		None,
		SwimmyGroup,  // 魚の群れイベント
		BearHelp,     // クマが釣りに参加するイベント
		WeatherChange // 天候が変わるイベント
	};

	void Initialize(Model* model, Camera* camera, Score* Score, const Vector3& pos, bool moveRight, int getTimer);
	void Update() override;
	void OnCollision(Player* player) override;

	void SetEventType(FishEventType type) { eventType_ = type;}

	 // イベント発火時に呼ばれるコールバック
	void SetOnTriggered(std::function<void(const Vector3&,FishEventType)> callback) { onTriggered_ = callback; }

	FishEventType GetEventType() const { return eventType_; }

private:

	bool hasTriggered_ = false;

	FishEventType eventType_ = FishEventType::None;

	std::function<void(const Vector3&, FishEventType)> onTriggered_;
};
