#include "Player.h"
#include "ImGuiManager.h"
#include "MathUtilityForText.h"
#include "Vector3.h" 
#include <cassert>

void Player::PlayerStart() { worldTransform_.translation_ = {0, 0, 0}; }

void Player::Initialize(const std::vector<Model*>& models) {
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;

	worldTransformHead_.Initialize();
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformHead_.translation_.y = 1.5038f;

	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.translation_.x = -0.5275f;
	worldTransformL_arm_.translation_.y = 1.2619f;

	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.translation_.x = +0.5275f;
	worldTransformR_arm_.translation_.y = 1.2619f;

	// 浮遊ギミックの初期化
	InitializeFloatingGimmick();

	// インプット
	input_ = Input::GetInstance();
}

void Player::Update() {

	// 浮遊ギミックの更新
	UpdateFloatingGimmick();

	const float speed = 0.3f;

	Vector3 move = {0.0f, 0.0f, 0.0f};

	if (input_->PushKey(DIK_RIGHT)) {
		move.x = 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		move.x = -1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		move.z = 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		move.z = -1.0f;
	}

	move = Normalize(move) * speed;

	if (viewProjection_) {
		// カメラの回転行列
		Matrix4x4 matRotX = MakeRotateXMatrix(viewProjection_->rotation_.x);
		Matrix4x4 matRotY = MakeRotateYMatrix(viewProjection_->rotation_.y);
		Matrix4x4 matRotZ = MakeRotateZMatrix(viewProjection_->rotation_.z);
		// 回転行列の合成
		Matrix4x4 matRot = matRotZ * matRotX * matRotY;

		// 移動量をカメラの回転に合わせて回転させる
		move = TransformNormal(move, matRot);
	}

	worldTransform_.translation_ += move;

	// 移動ベクトルのY軸周り角度
	worldTransform_.rotation_.y = std::atan2(move.x, move.z);

	// 変換行列を更新
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	// 3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {

	floatingParameter_ += 0.1f;
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * 0.2f;
	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * 0.5f;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * 0.5f;
}