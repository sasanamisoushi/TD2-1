#include "Fish.h"
#include "math.h"
#include <DirectXMath.h>
using namespace DirectX;

#include <cstdlib>
#include <cassert>
#include <numbers>

void Fish::Initialize(Model* model, Camera* camera, const Vector3& position) {
	// NULL�|�C���^�`�F�b�N
	assert(model);
	// ���f��
	model_ = model;

	// �J����
	camera_ = camera;

	// �����̈ʒu�𒆐S�ɂT���͈̔͂ŉj��
	leftLimit_ = position.x - 5.0f;
	rigdhtLimit_ = position.x + 5.0f;

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	
	
	//�ړ�����
	//direction_ = moveRight ? Vector3(1.0f, 0.0f, 0.0f) : Vector3(-1.0f, 0.0f, 0.0f);

	// ���x�x�N�g��������Ɋ�Â��Đݒ�
	velocity_ += {-speed_,0.0f,0.0f};

	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	// �p�x����
	//if (moveRight) {
	//	//�E
	//	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	//} else {
	//	//��
	//	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	//}
}

void Fish::Update() {

	// �ړ�
	worldTransform_.translation_ +=  velocity_;

	// �[�Ŕ��]�i�q�X�e���V�X����������j
	//if (worldTransform_.translation_.x > rigdhtLimit_+0.1f) {
	//	direction_.x = -1.0f;                   // ����
	//	velocity_.x = direction_.x * speed_;
	//	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f; // �������ɉ�]
	//	worldTransform_.translation_.x = rigdhtLimit_;
	//} else if (worldTransform_.translation_.x < leftLimit_-0.1f) {
	//	direction_.x = 1.0f;                     // �E��
	//	velocity_.x = direction_.x * speed_;
	//	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f; // �E�����ɉ�]
	//	worldTransform_.translation_.x = leftLimit_;
	//}

	

	// �s��X�V
	WorldTransformUpdate(worldTransform_);
}

void Fish::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3D���f���`��O����
	Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_, * camera_);
	Model::PostDraw();
}
