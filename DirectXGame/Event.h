#pragma once
#include "KamataEngine.h"
#include "math.h"

using namespace KamataEngine;

class Event {
public:
	
	virtual ~Event() = default;
	virtual void Start(class GameScene* scene) { (void)scene; };
	virtual void Update(class GameScene* scene) { (void)scene; }
	virtual void End(class GameScene* scene) { (void)scene; }
	bool isActive_ = false;

private:
};
