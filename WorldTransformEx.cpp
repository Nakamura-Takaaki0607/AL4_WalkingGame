#include "MathUtilityForText.h"
#include "WorldTransform.h"

// WorldTransform.h に関数宣言を加える
// worldTransform_.UpdateMatrix();
// のように呼び出せば、親子構造を含めた行列変換と転送を行う

void WorldTransform::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	// 親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ *= parent_->matWorld_;
	}

	// 定数バッファに転送する
	TransferMatrix();
}
