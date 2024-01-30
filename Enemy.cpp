#include "Enemy.h"
#include "MathUtilityForText.h"

void Enemy::EnemyStart() { worldTransform_.translation_ = {0, 0, 20}; }

void Enemy::Initialize(const std::vector<Model*>& models) {
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	// ワールド変換に初期化
	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;

	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.translation_.x = -0.87337f;
	worldTransformL_arm_.translation_.y = 1.01077f;

	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.translation_.x = +0.87337f;
	worldTransformR_arm_.translation_.y = 1.01077f;
}

void Enemy::Update() {
	// ギミックの更新
	UpdateGimmick();

	const float speed = 0.1f;

	worldTransform_.rotation_.y += 0.01f;

	Vector3 move = {0.0f, 0.0f, speed};

	Matrix4x4 matRotY = MakeRotateYMatrix(worldTransform_.rotation_.y);

	move = TransformNormal(move, matRotY);

	worldTransform_.translation_ += move;

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
}

void Enemy::UpdateGimmick() {
	const float idleArmAngleMax = 30.0f;

	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter) * idleArmAngleMax / 180.0f * PI;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter) * idleArmAngleMax / 180.0f * PI;
}