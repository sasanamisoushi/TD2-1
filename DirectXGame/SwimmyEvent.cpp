#include "SwimmyEvent.h"


void SwimmyEvent::Start(GameScene* scene) {
	//既存の魚を退場させる
	scene->ClearAllFish();

	//魚群を生成
	for (int i = 0; i < 10; i++) {
	
		Fish* fish = new Fish();
		fish->Initialize(scene->GetFishModel(), scene->GetCamera(), scene->GetRandomPos(), true, 90);
		scene->AddFish(fish);
	}

	//群れの中に1匹だけ赤い魚
	Fish* swmmy = new Fish();
	swmmy->SetEventType(FishEventType::swmmyFish);
	swmmy->Initialize(scene->GetSwimmyModel(), scene->GetCamera(), scene->GetRandomPos(), true, 90);
	scene->AddFish(swmmy);

	timer_ = 600;

}

void SwimmyEvent::Update(GameScene* scene) {

	if (timer_ > 0) {
	
		timer_--;
	} else {
	
		End(scene);
	}

}

void SwimmyEvent::End(GameScene* scene) {


	//群れを全て消す
	scene->ClearAllFish();

	for (int i = 0; i < 20; i++) {
		Fish* fish = new Fish();
		fish->Initialize(scene->GetFishModel(), scene->GetCamera(), scene->GetRandomPos(), true, 90);
		scene->AddFish(fish);
	}
}
