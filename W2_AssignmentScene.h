#pragma once
#include "GameScene.h"
class W2_AssignmentScene final : public GameScene
{
public:
	W2_AssignmentScene() :GameScene(L"W2_AssignmentScene") {}
	~W2_AssignmentScene() override = default;
	W2_AssignmentScene(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene(W2_AssignmentScene&& other) noexcept = delete;
	W2_AssignmentScene& operator=(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene& operator=(W2_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
private:
	GameObject* m_pLevel{ nullptr };
	GameObject* m_pSphere{ nullptr };

	PxRigidStatic* m_pTriggerLeft{ nullptr };
	PxRigidStatic* m_pTriggerRight{ nullptr };

	bool m_IsTriggeredLeft{};
	bool m_IsTriggeredRight{};

	GameObject* m_pCubeLeft{ nullptr };
	GameObject* m_pCubeRight{ nullptr };

	FMOD::Channel* m_pChannel2D{ nullptr };
	FMOD::Sound* m_pBellSound{ nullptr };

	GameObject* m_pHatchLeft{ nullptr };
	GameObject* m_pHatchRight{ nullptr };

	float m_LeftRotateAngle{};
	float m_RightRotateAngle{};

	FMOD_VECTOR m_prevCamPos{};

	enum InputIds
	{
		Play2D,
		Play3D,
		IncreaseVolume,
		DecreaseVolume
	};
};

