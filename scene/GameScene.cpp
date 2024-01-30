#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include "MathUtilityForText.h"
#include <cassert>

#include <time.h>

#include "ImGuiManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete spriteTitle_;
	delete spriteSetumei_;
	delete spriteGameOver_;
	delete spriteGameClear_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// タイトル
	textureHandleTitle_ = TextureManager::Load("Title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

	// タイトル2
	textureHandleMove_ = TextureManager::Load("Move.png");
	spriteMove_ = Sprite::Create(textureHandleMove_, {0, 0});

	// 説明画面
	// ゲームオーバー（2Dスプライト）
	textureHandleSetumei_ = TextureManager::Load("Setumei.png");
	spriteSetumei_ = Sprite::Create(textureHandleSetumei_, {0, 0});

	// ゲームオーバー
	textureHandleGameOver_ = TextureManager::Load("Gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 0});

	// ゲームクリア
	textureHandleGameClear_ = TextureManager::Load("GameClear.png");
	spriteGameClear_ = Sprite::Create(textureHandleGameClear_, {0, 0});

	// フェードインフェードアウト
	textureHandleBlack_ = TextureManager::Load("Black.png");
	spriteBlack_ = Sprite::Create(textureHandleBlack_, {0, 0});

	// タイム
	textureHandleTime0_ = TextureManager::Load("0.png");
	spriteTime0_ = Sprite::Create(textureHandleTime0_, {50, 50});

	textureHandleTime1_ = TextureManager::Load("1.png");
	spriteTime1_ = Sprite::Create(textureHandleTime1_, {50, 50});

	textureHandleTime2_ = TextureManager::Load("2.png");
	spriteTime2_ = Sprite::Create(textureHandleTime2_, {50, 50});

	textureHandleTime3_ = TextureManager::Load("3.png");
	spriteTime3_ = Sprite::Create(textureHandleTime3_, {50, 50});

	textureHandleTime4_ = TextureManager::Load("4.png");
	spriteTime4_ = Sprite::Create(textureHandleTime4_, {50, 50});

	textureHandleTime5_ = TextureManager::Load("5.png");
	spriteTime5_ = Sprite::Create(textureHandleTime5_, {50, 50});

	textureHandleTime6_ = TextureManager::Load("6.png");
	spriteTime6_ = Sprite::Create(textureHandleTime6_, {50, 50});

	textureHandleTime7_ = TextureManager::Load("7.png");
	spriteTime7_ = Sprite::Create(textureHandleTime7_, {50, 50});

	textureHandleTime8_ = TextureManager::Load("8.png");
	spriteTime8_ = Sprite::Create(textureHandleTime8_, {50, 50});

	textureHandleTime9_ = TextureManager::Load("9.png");
	spriteTime9_ = Sprite::Create(textureHandleTime9_, {50, 50});

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.translation_ = {0.0f, 2.0f, -10.0f};
	viewProjection_.Initialize();

	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(2000.0f);

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクトを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	// 3Dモデルの生成
	modelCube_.reset(Model::Create());
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelEnemyBody_.reset(Model::CreateFromOBJ("needle_Body", true));
	modelEnemyL_arm_.reset(Model::CreateFromOBJ("needle_L_arm", true));
	modelEnemyR_arm_.reset(Model::CreateFromOBJ("needle_R_arm", true));

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get());

	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get()};
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	std::vector<Model*> enemyModels = {
	    modelEnemyBody_.get(), modelEnemyL_arm_.get(), modelEnemyR_arm_.get()};
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModels);
	enemy_->SetLocalPosition(Vector3(10, 0, 20));
	enemy_->SetLocalRotation(Vector3(0, PI, 0));
}

void GameScene::Update() {

	switch (sceneMode_) {
	case 1:
		TitleUpdate();
		titleTimer_++;
		break;
	case 2:
		SetumeiUpdate();
		break;
	case 3:
		gameTimer_--;

		timerSUB_ = gameTimer_ / 60;

		// 追従カメラの更新
		followCamera_->Update();

		// デバッグカメラの更新
		if (input_->TriggerKey(DIK_O)) {
			// フラグをトグル
			isDebugCameraActive_ = !isDebugCameraActive_;
		}

		if (isDebugCameraActive_ == true) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		} else {
			followCamera_->Update();
			viewProjection_.matView = followCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		}
		// ビュープロジェクションの転送
		viewProjection_.TransferMatrix();

		skydome_->Update();

		ground_->Update();

		player_->Update();

		enemy_->Update();

		dx = abs(player_->GetX() - enemy_->GetX());

		dz = abs(player_->GetZ() - enemy_->GetZ());

		 // 衝突判定
		if (dx < 2 && dz < 2) {
			playerLife_ -= 1;
		}

		if (playerLife_ == 0) {
			black_ = 1.0f;
			sceneMode_ = 4;
		}

		if (gameTimer_ == 0) {
			black_ = 1.0f;
			sceneMode_ = 5;
		}
		break;
	case 4:
		GameOverUpdate();
		break;
	case 5:
		GameClearUpdate();
		break;
	}
}

void GameScene::GamePlayStart() {
	playerLife_ = 1;
	gameTimer_ = 600;
	timerSUB_ = 0;
}

void GameScene::TitleUpdate() {
	if (input_->TriggerKey(DIK_RETURN)) {
		flag_ = true;

		black_ = 0;
	}
	if (flag_ == true && black_ >= 1.0f){
		flag_ = false;
		black_ = 1.0f;
		sceneMode_ = 2;
		GamePlayStart();
	}
}

void GameScene::TitleDraw2DNear() {
	spriteTitle_->Draw();
}

void GameScene::MoveDraw2DNear() {
	if (titleTimer_ % 40 >= 20) {
		spriteMove_->Draw();
	}
}

void GameScene::SetumeiUpdate() {
	if (input_->TriggerKey(DIK_RETURN)) {
		flag_ = true;
		black_ = 0;
	}
	if (flag_ == true && black_ >= 1.0f){
		flag_ = false;
		black_ = 1.0f;
		sceneMode_ = 3;
		GamePlayStart();
		player_->PlayerStart();
		enemy_->EnemyStart();
	}
}

void GameScene::SetumeiDraw2DNear() {
	spriteSetumei_->Draw();
}

void GameScene::GameOverUpdate() {
	if (sceneMode_ == 4) {
		if (input_->TriggerKey(DIK_RETURN)) {
			flag_ = true;
			black_ = 0;
		}
		if (flag_ == true && black_ >= 1.0f) {
			flag_ = false;
			black_ = 1.0f;
			sceneMode_ = 1;
		}
	}
}

void GameScene::GameOverDraw2DNear() {
	spriteGameOver_->Draw();
}

void GameScene::GameClearUpdate() {
	if (sceneMode_ == 5) {
		if (input_->TriggerKey(DIK_RETURN)) {
			flag_ = true;

			black_ = 0;
		}
		if (flag_ == true && black_ >= 1.0f) {
			flag_ = false;
			black_ = 1.0f;
			sceneMode_ = 1;
		}
	}
}

void GameScene::GameClearDraw2DNear() {
	spriteGameClear_->Draw();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	switch (sceneMode_) {
	case 3:
		// クラスの描画
		player_->Draw(viewProjection_);
		ground_->Draw(viewProjection_);
		skydome_->Draw(viewProjection_);
		enemy_->Draw(viewProjection_);
		break;
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	switch (sceneMode_) {
	case 1:
		TitleDraw2DNear();
		MoveDraw2DNear();
		break;
	case 2:
		SetumeiDraw2DNear();
		break;
	case 3:
		if (timerSUB_ == 10) {
			spriteTime1_->Draw();
			spriteTime0_->Draw();
		}

		if (timerSUB_ == 9) {
			spriteTime9_->Draw();
		}

		if (timerSUB_ == 8) {
			spriteTime8_->Draw();
		}

		if (timerSUB_ == 7) {
			spriteTime7_->Draw();
		}

		if (timerSUB_ == 6) {
			spriteTime6_->Draw();
		}

		if (timerSUB_ == 5) {
			spriteTime5_->Draw();
		}

		if (timerSUB_ == 4) {
			spriteTime4_->Draw();
		}

		if (timerSUB_ == 3) {
			spriteTime3_->Draw();
		}

		if (timerSUB_ == 2) {
			spriteTime2_->Draw();
		}

		if (timerSUB_ == 1) {
			spriteTime1_->Draw();
		}
		if (timerSUB_ == 0) {
			spriteTime0_->Draw();
		}
		break;
	case 4:
		GameOverDraw2DNear();
		break;
	case 5:
		GameClearDraw2DNear();
	}

	if (flag_ == false) {
		black_ -= 0.01f;
	} else {
		black_ += 0.01f;
	}

	spriteBlack_->SetColor({1, 1, 1, black_});
	spriteBlack_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}