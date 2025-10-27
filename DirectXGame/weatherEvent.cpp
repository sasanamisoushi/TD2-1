#include "weatherEvent.h"
#include <imgui.h>

void weatherEvent::Initialize() {

	// 初期状態
	currentWeather_ = WeatherType::Clear;
	timer_ = 0.0f;
	isActive_ = false;
	duration_ = 10.0f;
  
	// 最大数の初期化
	maxFishCount_ = 0;
	maxBigFishCount_ = 0;
	maxRubbishCount_ = 0;
  
	bgm_ = new BGM();
	bgm_->Initialize();
	bgmBox_ = 0;

	rainBgm_ = Audio::GetInstance()->LoadWave("./BGM/In the Sweet By and By.mp3");
	rainbowBgm_ = Audio::GetInstance()->LoadWave("./BGM/In the Sweet By and By.mp3");
	cloudBgm_ = Audio::GetInstance()->LoadWave("./BGM/In the Sweet By and By.mp3");
	meteorBgm_ = Audio::GetInstance()->LoadWave("./BGM/In the Sweet By and By.mp3");
}

void weatherEvent::Update(int fishCount, int bigFishCount, int rubbishCount) {

	// 天気イベントが発動していないなら何もしない
	if (!isActive_) {
		return;
	}

	bgm_->BGMPlay(bgmBox_);
	// タイマーを継続時間分増やす
	timer_ += 1.0f / 60.0f;

	// --- 最大数更新 ---
	if (fishCount > maxFishCount_) {
		maxFishCount_ = fishCount;
	}
	if (bigFishCount > maxBigFishCount_) {
		maxBigFishCount_ = bigFishCount;
	}
	if (rubbishCount > maxRubbishCount_) {
		maxRubbishCount_ = rubbishCount;
	}

	// 一定時間経過したら天気をリセット
	if (timer_ >= duration_) {

		ResetWeather();
	}

	bool wasRainBefore = wasRaining_;
	wasRaining_ = (currentWeather_ == WeatherType::Rain);
	rainJustEnded_ = (wasRainBefore && !wasRaining_);

#ifdef _DEBUG
	ImGui::Begin("Weather Debug");
	ImGui::Text("Weather: %d", static_cast<int>(currentWeather_));
	ImGui::Text("Timer: %.2f", timer_);
	ImGui::Separator();
	ImGui::Text("Fish Count: %d (Max: %d)", fishCount, maxFishCount_);
	ImGui::Text("BigFish Count: %d (Max: %d)", bigFishCount, maxBigFishCount_);
	ImGui::Text("Rubbish Count: %d (Max: %d)", rubbishCount, maxRubbishCount_);
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
		bgmBox_ = rainBgm_;
		break;
	case 1:
		ChangeWeather(WeatherType::Cloud);
		bgmBox_ = cloudBgm_;
		break;
	case 2:
		ChangeWeather(WeatherType::Rainbow);
		bgmBox_ = rainbowBgm_;
		break;
	case 3:
		ChangeWeather(WeatherType::Meteor);
		bgmBox_ = meteorBgm_;
		break;
	}
}

float weatherEvent::GetFishSpeedMultiplier() const {
	// イベントが発動していないなら常に等速
	if (!isActive_) {
		return 0.01f;
	}

	switch (currentWeather_) {
	case WeatherType::Cloud:
		return 0.03f; // 雲で早い
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

float weatherEvent::GetFishMaxCountMultiplier() const {
	if (!isActive_) {
		return 1.0f; // 通常時（倍率1倍）
	}

	switch (currentWeather_) {
	case WeatherType::Rain:
		return 1.5f; // 雨で魚が多く出る
	default:
		return 1.0f;
	}
}

bool weatherEvent::WasRainJustEnded() const {
	
	return rainJustEnded_; 
}

void weatherEvent::ResetWeather() {
	maxFishCount_ = 0;
	maxBigFishCount_ = 0;
	maxRubbishCount_ = 0;

	rainJustEnded_ = (currentWeather_ == WeatherType::Rain);

	currentWeather_ = WeatherType::Clear;
	timer_ = 0.0f;
	isActive_ = false;
	bgm_->BGMStop();
}

void weatherEvent::ChangeWeather(WeatherType newWeather) {
	currentWeather_ = newWeather;
	timer_ = 0.0f;
	isActive_ = true;
}
