#pragma once
#include "Event.h"
#include "GameScene.h"
#include "Fish.h"


class SwimmyEvent : public Event{
public: 
	int timer_ = 300;

	void Start(GameScene* scene) override;

	void Update(GameScene* scene) override;
	void End(GameScene* scene) override;



};
