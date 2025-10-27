#pragma once
#include <array>

namespace KamataEngine 
{
	class Audio;
}

class BGM
{
public:

	void Initialize();

	void BGMPlay(uint32_t bgmHandle);

	void BGMStop();

private:

	uint32_t bgmHandle_; // BGMを入れる箱
};
