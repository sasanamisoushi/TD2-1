#include "Time.h"

using namespace KamataEngine;

void Time::Initialize() {
	//worldTransform_.Initialize();
	//worldTransform_.translation_ = {0};


	gameTimer_ = kGameTime;
	isGame = true;
}

void Time::Update() 
{

	if (isGame)
	{
		isGame = true;
		gameTimer_--;
		if (gameTimer_ <= 0) 
		{
			isGame = false;
		}
	}
}

void Time::Draw() 
{

	 // 秒に変換
	//int seconds = gameTimer_ / 60;

	// ImGuiで画面に描画
	ImGui::Begin("Timer");
	ImGui::Text("Time Remaining: %d", seconds);
	ImGui::End();
	//if (isGame)
	//{
	//	model_->Draw(worldTransform_,camera_);
	//}
}
