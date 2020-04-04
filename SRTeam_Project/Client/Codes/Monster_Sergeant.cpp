#include "stdafx.h"
#include "..\Headers\Monster_Sergeant.h"


#include "Component_Manager.h"
#include "Object_Manager.h"

#include "Monster_Sergeant_Bullet_Collision.h"

CMonster_Sergeant::CMonster_Sergeant(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * vStartPos, SCENETYPE eStage)
	:CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTargetCom(nullptr)
	, m_pWalkTextureCom(nullptr)
	, m_pAttackTextureCom(nullptr)
	, m_pDeathTextureCom(nullptr)
	, m_pHitTextureCom(nullptr)
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_fFrameMax(0.f)
	, m_fFrame(0.f)
	, m_iCount(0)
	, m_bSetUp_Hit(true)
	, m_bDieing(false)
	, m_bSetUp_Die(true)
	, m_eState(WALK)
{

	m_pObject_Manager->AddRef();
	m_pComponent_Manager->AddRef();
	m_vStartPos = *vStartPos;

	m_vRendomMovePos = m_vStartPos;
	m_vRendomMovePos.x += 1;
	m_vRendomMovePos.z += 1;

	m_tInfo.fCollisionSize = 0.5f;
	m_tInfo.fCollisionSearchSize = 6.f;
	m_tInfo.fCollisionAtkSearchSize = 3.f;
	m_tInfo.iHp = 15;

	m_eStage = eStage;

}

HRESULT CMonster_Sergeant::Ready_GameObject(void)
{
	if (Ready_Component())
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &m_vStartPos);
	return NOERROR;

}

_int CMonster_Sergeant::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALHPATEST, this);

	return 0;
}

_int CMonster_Sergeant::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (0 >= m_tInfo.iHp)
		m_bDieing = true;

	if (true == m_tInfo.bDie)
	{
		const _matrix* pmatView = m_pObject_Manager->Get_pCamera()->Get_TempView();
		_matrix matView = *pmatView;

		m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_RIGHT, (_vec3*)&matView.m[Engine::CTransform::INFO_RIGHT][0]);
		m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_UP, (_vec3*)&matView.m[Engine::CTransform::INFO_UP][0]);
		m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, (_vec3*)&matView.m[Engine::CTransform::INFO_LOOK][0]);

		m_pTransformCom->Set_Scale(&_vec3(m_vScale.x, m_vScale.y, m_vScale.z));

		return 0;
	}




	if (true == m_bDieing)
	{
		m_eState = DIE;
		
		if (true == m_bSetUp_Die)
		{
			CSoundMgr::GetInstance()->PlaySound(L"PlayerDeath.wav", CSoundMgr::Channel_Monster);
			m_bSetUp_Die = false;
			m_fFrame = 0.f;
		}
	}
	else
	{

		if (true == m_tInfo.bHit)
		{
			m_eState = HIT;
			if (true == m_bSetUp_Hit)
			{
				m_bSetUp_Hit = false;
				m_fFrame = 0.f;
			}

		}
		else
		{
			if (true == m_tInfo.bSearch)
			{
				if (true == m_tInfo.bAtkSearch)
				{
					m_tInfo.bAtk = true;
					m_eState = ATTACK;
				}
				else
				{
					m_tInfo.bAtk = false;
					m_eState = WALK;
					PlayerPos_Check(fTimeDelta);
				}
			}

			if (true == m_tInfo.bAtk)
			{

				if (20 == m_iCount)
				{
					Engine::CGameObject*		pGameObject = nullptr;


					CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
					_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);

					_vec3 vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);

					vLook.y = 0.f;


					pGameObject = CMonster_Sergeant_Bullet_Collision::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vLook, m_eStage);
					if (nullptr == pGameObject)
						return 0;

					if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, L"Monster_Bullet", pGameObject)))
						return 0;

					m_iCount = 0;
				}
				else
				{
					++m_iCount;
				}


			}
		}
	}


	const _matrix* pmatView = m_pObject_Manager->Get_pCamera()->Get_TempView();
	_matrix matView = *pmatView;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_RIGHT, (_vec3*)&matView.m[Engine::CTransform::INFO_RIGHT][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_UP, (_vec3*)&matView.m[Engine::CTransform::INFO_UP][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, (_vec3*)&matView.m[Engine::CTransform::INFO_LOOK][0]);

	m_pTransformCom->Set_Scale(&_vec3(m_vScale.x, m_vScale.y, m_vScale.z));


	Move_OnTerrain();

	switch (m_eState)
	{
	case WALK:
		m_fFrameMax = 4.f;
		break;
	case ATTACK:
		m_fFrameMax = 2.f;
		break;
	case HIT:
		m_fFrameMax = 1.f;
		break;
	case DIE:
		m_fFrameMax = 8.f;
		break;
	/*case WALK_LEFT:
		m_fFrameMax = 4.f;
		break;
	case WALK_RIGHT:
		m_fFrameMax = 4.f;
		break;
	
	
	*/
	}


	m_fFrame += 5.f * fTimeDelta;

	if (m_fFrame >= m_fFrameMax)
	{
		m_fFrame = 0.f;

		if (HIT == m_eState)
		{
			m_eState = WALK;
			m_tInfo.bHit = false;
			m_bSetUp_Hit = true;
		}
		/*
		if (ATTACK == m_eState)
		{
			Create_Bullet(L"Monster_Bullet", fTimeDelta);
			m_bAtkTF = false;
		}
		*/

		if (DIE == m_eState)
		{
			m_fFrame = 8.f;
			m_tInfo.bDie = true;
		}
	}

	return 0;

}

void CMonster_Sergeant::Render_GameObject(void)
{
	if (nullptr == m_pBufferCom)
		return;

	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pTransformCom->SetUp_OnGraphicDev();
	switch (m_eState)
	{
	case WALK:
		m_pWalkTextureCom->SetUp_OnGraphicDev(0, _uint(m_fFrame));
		break;
		/*case WALK_LEFT:
			m_pTextureCom_Walk_Left->SetUp_OnGraphicDev(0, _uint(m_fFrame));
			break;
		case WALK_RIGHT:
			m_pTextureCom_Walk_Right->SetUp_OnGraphicDev(0, _uint(m_fFrame));
			break;*/
	case HIT:
		m_pHitTextureCom->SetUp_OnGraphicDev(0);
		break;
	case ATTACK:
		m_pAttackTextureCom->SetUp_OnGraphicDev(0, _uint(m_fFrame));
		break;
	case DIE:
		m_pDeathTextureCom->SetUp_OnGraphicDev(0, _uint(m_fFrame));
		break;
	}
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


}

HRESULT CMonster_Sergeant::Ready_Component(void)
{

	Engine::CComponent*			pComponent = nullptr;

	// Transform
	pComponent = m_pTransformCom = (Engine::CTransform*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Transform_com", pComponent));
	m_pTransformCom->AddRef();

	// Buffers
	pComponent = m_pBufferCom = (Engine::CRect_Texture*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectTex");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer_Imp", pComponent));
	m_pBufferCom->AddRef();

	// Texture_Walk
	pComponent = m_pWalkTextureCom = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Sergeant_Walk");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Walk", pComponent));
	m_pWalkTextureCom->AddRef();

	// Texture_Attack
	pComponent = m_pAttackTextureCom = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Sergeant_Attack");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Attack", pComponent));
	m_pAttackTextureCom->AddRef();

	// Texture_Death
	pComponent = m_pDeathTextureCom = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Sergeant_Death");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Death", pComponent));
	m_pDeathTextureCom->AddRef();

	// Texture_Hit
	pComponent = m_pHitTextureCom = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Sergeant_Hit");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Hit", pComponent));
	m_pHitTextureCom->AddRef();

	// Renderer
	pComponent = m_pRendererCom = (Engine::CRenderer*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));
	m_pRendererCom->AddRef();


	return NOERROR;
}

void CMonster_Sergeant::Move_OnTerrain(void)
{
	if (nullptr == m_pTransformCom)
		return;

	if (nullptr == m_pTargetCom)
	{
		m_pTargetCom = (CVIBuffer*)m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Terrain", 0, L"Com_Buffer");
		m_pTargetCom->AddRef();
	}

	m_pTransformCom->Move_OnBuffer(m_pTargetCom, 0.46f);
}

void CMonster_Sergeant::PlayerPos_Check(float fTimeDelta)
{
	CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
	_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);

	_vec3 vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);

	m_pTransformCom->Set_Infomation(CTransform::INFO_LOOK, &vLook);

	m_pTransformCom->Go_Straight(1.f, fTimeDelta);
}

HRESULT CMonster_Sergeant::Create_Bullet(const _tchar * pLayertag, float fTimeDelta)
{
	return NOERROR;
}

CMonster_Sergeant * CMonster_Sergeant::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * vStartPos, SCENETYPE eStage)
{
	CMonster_Sergeant *	pInstance = new CMonster_Sergeant(pGraphicDev, vStartPos, eStage);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CMonster_Sergeant Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CMonster_Sergeant::Free(void)
{
	_ulong		dwRefcnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pWalkTextureCom);
	Engine::Safe_Release(m_pAttackTextureCom);
	Engine::Safe_Release(m_pDeathTextureCom);
	Engine::Safe_Release(m_pHitTextureCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTargetCom);
	Engine::Safe_Release(m_pComponent_Manager);
	Engine::Safe_Release(m_pObject_Manager);


	dwRefcnt = Engine::CGameObject::Free();

	return dwRefcnt;
}
