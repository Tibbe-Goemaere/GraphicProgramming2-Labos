#include "stdafx.h"
#include "BoxTorqueScene.h"

#include "CubePosColorNorm.h"
#include "Logger.h"

enum InputIds
{
	Up,
	Down,
	Left,
	Right
};

void BoxTorqueScene::Initialize()
{

	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.f);

	//Cube
	const XMFLOAT3 actorDimensions{ 1.f,1.f,1.f };
	m_pCube = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	AddGameObject(m_pCube);

	//Cube actor
	m_pCubeActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxBoxGeometry boxGeometry = PxBoxGeometry{ actorDimensions.x / 2.f,actorDimensions.y / 2.f,actorDimensions.z / 2.f };

	PxRigidActorExt::createExclusiveShape(*m_pCubeActor, boxGeometry, *pDefaultMaterial);

	//Link cube with cube actor
	m_pCube->AttachRigidActor(m_pCubeActor);

	m_pCube->Translate(0.f, 00.f, 0.f);
	//Groundplane
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Input
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Down, InputTriggerState::down, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Up, InputTriggerState::down, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Right, InputTriggerState::down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Left, InputTriggerState::down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT });

	//Camera
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ 0.f,20.f,-10.f });
	m_SceneContext.GetCamera()->SetForward({ 0.f,-2.f,1.f });
}

void BoxTorqueScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pCube->Translate(0.f, 10.f, 0.f);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(Left))
	{
		m_pCubeActor->addTorque(PxVec3{ 0,0,5 });
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Right))
	{
		m_pCubeActor->addTorque(PxVec3{ 0,0,-5 });
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Up))
	{
		m_pCubeActor->addTorque(PxVec3{ 5,0,0 });
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Down))
	{
		m_pCubeActor->addTorque(PxVec3{ -5,0,0 });
	}
}

void BoxTorqueScene::Draw() const
{

}

void BoxTorqueScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void BoxTorqueScene::OnSceneDeactivated()
{
}
