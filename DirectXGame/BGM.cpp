#include "BGM.h"
#include "KamataEngine.h"
#include <stdio.h>

using namespace KamataEngine;

void BGM::Initialize()
{
	bgmHandle_ = 0;
}

void BGM::BGMPlay(uint32_t bgmHandle) 
{
	if (!Audio::GetInstance()->IsPlaying(bgmHandle_)) {
		bgmHandle_ = Audio::GetInstance()->PlayWave(bgmHandle, true);
	}
}

void BGM::BGMStop()
{
	if (Audio::GetInstance()->IsPlaying(bgmHandle_))
	{
		Audio::GetInstance()->StopWave(bgmHandle_);
	}
}