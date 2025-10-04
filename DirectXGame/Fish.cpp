#include "Fish.h"
#include <cstdlib>

void Fish::Initialize(const Vector2& startPos, bool moveRight) {
	pos_ = startPos;
	moveRight_ = moveRight;

	// 左右方向の速度を決定
	velocity_ = {moveRight ? speed_ : -speed_, 0.0f};

	// 魚のテクスチャ読み込み（仮）
	textureHandle_ = TextureManager::Load("fish.png");
}

void Fish::Update() {
	pos_ += velocity_;

	// 画面外に出たら反対側へ回り込む
	const float screenWidth = 1280.0f; // 画面サイズに合わせて変更
	if (pos_.x > screenWidth + radius_) {
		pos_.x = -radius_;
	} else if (pos_.x < -radius_) {
		pos_.x = screenWidth + radius_;
	}
}

void Fish::Draw() { 
	Sprite::Draw(textureHandle_, pos_, {moveRight_ ? 1.0f : -1.0f, 1.0f}); 
}
