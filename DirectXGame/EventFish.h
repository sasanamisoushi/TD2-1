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

	void Initialize(Model* model, Camera* camera, Score* Score, const Vector3& pos, bool moveRight);
	void Update() override;
	void OnCollision(Player* player) override;

	void SetEventType(FishEventType type) { eventType_ = type;}

	 // イベント発火時に呼ばれるコールバック
	void SetOnTriggered(std::function<void(const Vector3&,FishEventType)> callback) { onTriggered_ = callback; }

	FishEventType GetEventType() const { return eventType_; }

	AABB GetAABB();

private:

	//スコア
	Score* score_;
	const int point_ = 1000;

	bool hasTriggered_ = false;

	FishEventType eventType_ = FishEventType::None;

	std::function<void(const Vector3&, FishEventType)> onTriggered_;

	// 生きてる or 釣られた
	bool isAlive_ = true;

	float triggerTimer_;
};
