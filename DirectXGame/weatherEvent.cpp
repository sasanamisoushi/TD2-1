#include "weatherEvent.h"
#include <imgui.h>

void weatherEvent::Initialize() {

	// 初期状態
	currentWeather_ = WeatherType::Clear;
	timer_ = 0.0f;
	isActive_ = false;
	duration_ = 10.0f;
}

void weatherEvent::Update() {

	// 天気イベントが発動していないなら何もしない
	if (!isActive_) {
		return;
	}

	// タイマーを継続時間分増やす
	timer_ += 1.0f / 60.0f;

	// 一定時間経過したら天気をリセット
	if (timer_ >= duration_) {

		ResetWeather();
	}

#ifdef _DEBUG
	ImGui::Begin("Weather Debug");
	ImGui::Text("Weather: %d", static_cast<int>(currentWeather_));
	ImGui::Text("Timer: %.2f", timer_);
	ImGui::End();
#endif
}

void weatherEvent::Draw() {}

void weatherEvent::TriggerRandomWeather() {

	// すでにイベント中ならスキップ
	if (isActive_) {

		return;
	}

	int random = rand() % 4;
	switch (random) {
	case 0:
		ChangeWeather(WeatherType::Rain);
		break;
	case 1:
		ChangeWeather(WeatherType::Cloud);
		break;
	case 2:
		ChangeWeather(WeatherType::Rainbow);
		break;
	case 3:
		ChangeWeather(WeatherType::Meteor);
		break;
	}
}

float weatherEvent::GetFishSpeedMultiplier() const {
	// イベントが発動していないなら常に等速
	if (!isActive_) {
		return 0.01f;
	}

	switch (currentWeather_) {
	case WeatherType::Rain:
		return 0.05f; // 雨で速い
	case WeatherType::Cloud:
		return 0.01f; // 雲で遅い
	default:
		return 0.01f;
	}
}

float weatherEvent::GetBigFishSpawnRate() const {

	if (!isActive_) {
		return 0.1f;
	}

	switch (currentWeather_) {
	case WeatherType::Rainbow:
		return 0.5f; // 虹で大魚出やすい
	default:
		return 0.1f;
	}
}

float weatherEvent::GetRubbishSpawnRate() const {

	if (!isActive_) {
		return 0.1f;
	}

	switch (currentWeather_) {
	case WeatherType::Meteor:
		return 0.5f; // 隕石でゴミ出やすい
	default:
		return 0.1f;
	}
}

void weatherEvent::ResetWeather() {
	currentWeather_ = WeatherType::Clear;
	timer_ = 0.0f;
	isActive_ = false;
}

void weatherEvent::ChangeWeather(WeatherType newWeather) {
	currentWeather_ = newWeather;
	timer_ = 0.0f;
	isActive_ = true;
}
