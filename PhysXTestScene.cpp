#include "stdafx.h"
#include "PhysXTestScene.h"

#include "CubePosColorNorm.h"
#include "TorusPosColorNorm.h"
#include "CylinderPosColorNorm.h"
#include "Logger.h"

void PhysXTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.5f,0.5f,1.f);
	
	//Cube
	const XMFLOAT3 actorDimensions{ 1.f,1.f,1.f };
	m_pCube = new CubePosColorNorm(actorDimensions.x,actorDimensions.y,actorDimensions.z);
	AddGameObject(m_pCube);

	//Cube actor
	PxRigidDynamic* pCubeActor = pPhysX->createRigidDynamic(PxTransform{PxIdentity});
	PxBoxGeometry boxGeometry = PxBoxGeometry{actorDimensions.x/2.f,actorDimensions.y/2.f,actorDimensions.z/2.f};

	//PxShape* pCubeShape = pPhysX->createShape(boxGeometry, *pDefaultMaterial, true);
	//pCubeActor->attachShape(*pCubeShape);
	//m_pPhysxScene->addActor(*pCubeActor);
	//OR
	PxRigidActorExt::createExclusiveShape(*pCubeActor, boxGeometry, *pDefaultMaterial);

	//Link cube with cube actor
	m_pCube->AttachRigidActor(pCubeActor);
	
	m_pCube->Translate(0.f, 10.f, 0.f);
	m_pCube->RotateDegrees(30.f,45.f,25.f);
	//Groundplane
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);
}

void PhysXTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed,'R'))
	{
		m_pCube->Translate(0.f, 10.f, 0.f);
	}
}

void PhysXTestScene::Draw() const
{

}

void PhysXTestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void PhysXTestScene::OnSceneDeactivated()
{
}
