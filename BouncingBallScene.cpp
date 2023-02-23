#include "stdafx.h"
#include "BouncingBallScene.h"

#include "SpherePosColorNorm.h"
#include "Logger.h"

void BouncingBallScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pBouncyMaterial = pPhysX->createMaterial(0.5f, 0.5f, 1.f);
	const PxMaterial* pHalfBouncyMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.5f);
	const PxMaterial* pNotBouncyMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.f);

	//Cube
	const float radius{1};
	const int parts{ 10 };
	m_pSphere1 = new SpherePosColorNorm(radius, parts, parts, XMFLOAT4( Colors::ForestGreen ));
	AddGameObject(m_pSphere1);

	m_pSphere2 = new SpherePosColorNorm(radius, parts, parts, XMFLOAT4(Colors::ForestGreen));
	AddGameObject(m_pSphere2);

	m_pSphere3 = new SpherePosColorNorm(radius, parts, parts, XMFLOAT4(Colors::ForestGreen));
	AddGameObject(m_pSphere3);

	//Cube actor
	PxRigidDynamic* pSphere1Actor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pSphere2Actor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pSphere3Actor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxSphereGeometry sphereGeometry = PxSphereGeometry{ radius/2.f };

	PxRigidActorExt::createExclusiveShape(*pSphere1Actor, sphereGeometry, *pBouncyMaterial);
	PxRigidActorExt::createExclusiveShape(*pSphere2Actor, sphereGeometry, *pHalfBouncyMaterial);
	PxRigidActorExt::createExclusiveShape(*pSphere3Actor, sphereGeometry, *pNotBouncyMaterial);

	//Link sphere with sphere actor
	m_pSphere1->AttachRigidActor(pSphere1Actor);
	m_pSphere2->AttachRigidActor(pSphere2Actor);
	m_pSphere3->AttachRigidActor(pSphere3Actor);

	m_pSphere1->Translate(-10.f, 10.f, 0.f);
	m_pSphere2->Translate(0.f, 10.f, 0.f);
	m_pSphere3->Translate(10.f, 10.f, 0.f);
	
	//Groundplane
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pBouncyMaterial);
	m_pPhysxScene->addActor(*pGroundActor);
}

void BouncingBallScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pSphere1->Translate(-10.f, 10.f, 0.f);
		m_pSphere2->Translate(0.f, 10.f, 0.f);
		m_pSphere3->Translate(10.f, 10.f, 0.f);
	}
}

void BouncingBallScene::Draw() const
{

}

void BouncingBallScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void BouncingBallScene::OnSceneDeactivated()
{
}
