#include "Score.h"
#include "KamataEngine.h"

using namespace KamataEngine;

Score::~Score() 
{
	for (int i = 0; i < 10; i++) 
	{
		delete sprites_[i];
		sprites_[i] = nullptr;
	}
}

void Score::Initialize()
{
	
	Vector4 collar = {1, 1, 1, 1};
	tex_[0] = TextureManager::Load("num/0.png");
	tex_[1] = TextureManager::Load("num/1.png");
	tex_[2] = TextureManager::Load("num/2.png");
	tex_[3] = TextureManager::Load("num/3.png");
	tex_[4] = TextureManager::Load("num/4.png");
	tex_[5] = TextureManager::Load("num/5.png");
	tex_[6] = TextureManager::Load("num/6.png");
	tex_[7] = TextureManager::Load("num/7.png");
	tex_[8] = TextureManager::Load("num/8.png");
	tex_[9] = TextureManager::Load("num/9.png");

	sprites_[0] = Sprite::Create(tex_[0], {20, 0}, collar, {0.0f, 0.0f}, false, false);
	sprites_[1] = Sprite::Create(tex_[0], {40, 0}, collar, {0.0f, 0.0f}, false, false);
	sprites_[2] = Sprite::Create(tex_[0], {60, 0}, collar, {0.0f, 0.0f}, false, false);
	sprites_[3] = Sprite::Create(tex_[0], {80, 0}, collar, {0.0f, 0.0f}, false, false);
	sprites_[4] = Sprite::Create(tex_[0], {100, 0}, collar, {0.0f, 0.0f}, false, false);
	sprites_[5] = Sprite::Create(tex_[0], {120, 0}, collar, {0.0f, 0.0f}, false, false);
	sprites_[6] = Sprite::Create(tex_[0], {140, 0}, collar, {0.0f, 0.0f}, false, false);
	sprites_[7] = Sprite::Create(tex_[0], {160, 0}, collar, {0.0f, 0.0f}, false, false);
	sprites_[8] = Sprite::Create(tex_[0], {180, 0}, collar, {0.0f, 0.0f}, false, false);
	sprites_[9] = Sprite::Create(tex_[0], {200, 0}, collar, {0.0f, 0.0f}, false, false);

	score_ = 0;
}

void Score::AddScore(int score)
{
	score_ += score; 
}

void Score::ResetScore()
{ score_ = 0; }


void Score::Update()
{
	for (int i = 0; i < 10; i++) 
	{
		delete sprites_[i];
		sprites_[i] = nullptr;
	}
}

void Score::Draw()
{
	int ketaNum = 10; // 桁数
	std::array<int, 10> keta = {0};
	int viewScore = score_;
	for (int i = 0; i < ketaNum; i++) 
	{
		keta[ketaNum - i - 1] = viewScore % 10;
		viewScore /= 10;
	}
	for (int i = 0; i < ketaNum; i++)
	{
		sprites_[i]->SetTextureHandle(tex_[keta[i]]);
		sprites_[i]->Draw();
	}
}
