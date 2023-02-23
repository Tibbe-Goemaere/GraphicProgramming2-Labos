#include "stdafx.h"
#include "FrictionTestScene.h"

#include "CubePosColorNorm.h"
#include "Logger.h"

void FrictionTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pNoFrictionMaterial = pPhysX->createMaterial(0.f, 0.f, 0.f);
	const PxMaterial* pHalfFrictionMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.f);
	const PxMaterial* pFullFrictionMaterial = pPhysX->createMaterial(1.f, 1.f, 0.f);

	//Cube
	const XMFLOAT3 actorDimensions{ 1.f,1.f,1.f };
	m_pCube1 = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	AddGameObject(m_pCube1);

	m_pCube2 = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	AddGameObject(m_pCube2);

	m_pCube3 = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	AddGameObject(m_pCube3);

	//Cube actor
	PxRigidDynamic* pCube1Actor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pCube2Actor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pCube3Actor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxBoxGeometry boxGeometry = PxBoxGeometry{ actorDimensions.x / 2.f,actorDimensions.y / 2.f,actorDimensions.z / 2.f };

	PxRigidActorExt::createExclusiveShape(*pCube1Actor, boxGeometry, *pNoFrictionMaterial);
	PxRigidActorExt::createExclusiveShape(*pCube2Actor, boxGeometry, *pHalfFrictionMaterial);
	PxRigidActorExt::createExclusiveShape(*pCube3Actor, boxGeometry, *pFullFrictionMaterial);

	//Link cube with cube actor
	m_pCube1->AttachRigidActor(pCube1Actor);
	m_pCube2->AttachRigidActor(pCube2Actor);
	m_pCube3->AttachRigidActor(pCube3Actor);

	m_pCube1->Translate(-10.f, 5.f, 0.f);
	m_pCube2->Translate(0.f, 5.f, 0.f);
	m_pCube3->Translate(10.f, 5.f, 0.f);

	m_pCube1->RotateDegrees(0.f, 0.f, -30.f);
	m_pCube2->RotateDegrees(0.f, 0.f, -30.f);
	m_pCube3->RotateDegrees(0.f, 0.f, -30.f);
	
	//Groundplane
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pNoFrictionMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Ramps
	const XMFLOAT3 rampDimensions{ 5.f,1.f,2.f };
	auto pRamp1 = new CubePosColorNorm(rampDimensions.x, rampDimensions.y, rampDimensions.z);
	AddGameObject(pRamp1);

	auto pRamp2 = new CubePosColorNorm(rampDimensions.x, rampDimensions.y, rampDimensions.z);
	AddGameObject(pRamp2);

	auto pRamp3 = new CubePosColorNorm(rampDimensions.x, rampDimensions.y, rampDimensions.z);
	AddGameObject(pRamp3);

	//Ramp actor
	auto pRamp1Actor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	auto pRamp2Actor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	auto pRamp3Actor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	PxBoxGeometry rampGeometry = PxBoxGeometry{ rampDimensions.x / 2.f,rampDimensions.y / 2.f,rampDimensions.z / 2.f };

	PxRigidActorExt::createExclusiveShape(*pRamp1Actor, rampGeometry, *pNoFrictionMaterial);
	PxRigidActorExt::createExclusiveShape(*pRamp2Actor, rampGeometry, *pNoFrictionMaterial);
	PxRigidActorExt::createExclusiveShape(*pRamp3Actor, rampGeometry, *pNoFrictionMaterial);

	//Link ramp with ramp actor
	pRamp1->AttachRigidActor(pRamp1Actor);
	pRamp2->AttachRigidActor(pRamp2Actor);
	pRamp3->AttachRigidActor(pRamp3Actor);

	pRamp1->Translate(-10.f, 2.f, 0.f);
	pRamp2->Translate(0.f, 2.f, 0.f);
	pRamp3->Translate(10.f, 2.f, 0.f);

	pRamp1->RotateDegrees(0, 0.f, -30.f);
	pRamp2->RotateDegrees(0.f, 0.f, -30.f);
	pRamp3->RotateDegrees(0.f, 0.f, -30.f);
}

void FrictionTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pCube1->Translate(-10.f, 5.f, 0.f);
		m_pCube2->Translate(0.f, 5.f, 0.f);
		m_pCube3->Translate(10.f, 5.f, 0.f);

		m_pCube1->RotateDegrees(0.f, 0.f, -30.f);
		m_pCube2->RotateDegrees(0.f, 0.f, -30.f);
		m_pCube3->RotateDegrees(0.f, 0.f, -30.f);
	}
}

void FrictionTestScene::Draw() const
{

}

void FrictionTestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void FrictionTestScene::OnSceneDeactivated()
{
}
