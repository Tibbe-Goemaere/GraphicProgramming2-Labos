#include "stdafx.h"
#include "TriggerTestScene.h"

#include "MeshObject.h"
#include "ContentManager.h"
#include "SpherePosColorNorm.h"
#include "Logger.h"

enum InputIds
{
	Up,
	Down,
	Left,
	Right
};

void TriggerTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = pPhysX->createMaterial(0.5, 0.5f, 0.2f);

	//Ground plane
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo, {0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Sphere
	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Moccasin });
	AddGameObject(m_pSphere);

	const auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterial);

	//Link sphere with sphere actor
	m_pSphere->AttachRigidActor(pSphereActor);
	
	m_pSphere->Translate(0.f, 2.f, 0.f);

	//Trigger left
	m_pTriggerLeft = pPhysX->createRigidStatic(PxTransform{ -8,0.5f,0 });
	auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, PxBoxGeometry{ 0.5f,0.5f,0.5f },*pDefaultMaterial);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pPhysxScene->addActor(*m_pTriggerLeft);

	//Trigger right
	m_pTriggerRight = pPhysX->createRigidStatic(PxTransform{ 8,0.5f,0 });
	pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, PxBoxGeometry{ 0.5f,0.5f,0.5f }, *pDefaultMaterial);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pPhysxScene->addActor(*m_pTriggerRight);
	
	//Input
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Down, InputTriggerState::down, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Up, InputTriggerState::down, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Right, InputTriggerState::down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Left, InputTriggerState::down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT });
}

void TriggerTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
	
	}

	const float force{ 5.f };
	if (m_SceneContext.GetInput()->IsActionTriggered(Left))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({0,0,force});
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Right))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,-force });
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Up))
	{

	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Down))
	{

	}

	//Trigger check
	if (m_IsTriggeredLeft)
	{
		
		m_IsTriggeredLeft = false;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0.f,10.f,0.f }, PxForceMode::eIMPULSE);
		
	}

	if (m_IsTriggeredRight)
	{
		
		m_IsTriggeredRight = false;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0.f,10.f,0.f }, PxForceMode::eIMPULSE);
		
	}
}

void TriggerTestScene::Draw() const
{

}

void TriggerTestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void TriggerTestScene::OnSceneDeactivated()
{
}

void TriggerTestScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i{ 0 }; i < count; ++i)
	{
		//ignore deleted shapes
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		const PxTriggerPair& pair = pairs[i];
		
		if (pair.triggerActor == m_pTriggerLeft)
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //entered
			{
				Logger::GetInstance()->LogInfo(L"left trigger activated");
				
				m_IsTriggeredLeft = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) //Left
			{
				Logger::GetInstance()->LogInfo(L"left trigger left");
			}
		}

		if (pair.triggerActor == m_pTriggerRight)
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //entered
			{
				m_IsTriggeredRight = true;
				Logger::GetInstance()->LogInfo(L"right trigger activated");
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) //Left
			{
				Logger::GetInstance()->LogInfo(L"right trigger left");
			}
		}
	}
}
