#include <Windows.h>
#include "KamataEngine.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	//エンジンの初期化
	KamataEngine::Initialize(L"TD2");

	//DirectXCommmonインスタンスの取得
	DirectXCommon* dxCommmon = DirectXCommon::GetInstance();

	//メインループ
	while (true) {
		//エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}


		//描画開始
		dxCommmon->PreDraw();


		//描画終了
		dxCommmon->PostDraw();
	
	}


	//エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}
