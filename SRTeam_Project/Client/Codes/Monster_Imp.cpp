#include "stdafx.h"
#include "..\Headers\Monster_Imp.h"

#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Client_Player.h"
#include "Camera.h"
#include "Monster_Imp_Bullet_Collision.h"

CMonster_Imp::CMonster_Imp(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vStartPos, SCENETYPE eStage)
	: Engine::CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom_Walk(nullptr)
	, m_pTextureCom_Walk_Left(nullptr)
	, m_pTextureCom_Walk_Right(nullptr)
	, m_pTextureCom_Hit(nullptr)
	, m_pTextureCom_Atk(nullptr)
	, m_pTextureCom_Die(nullptr)
	, m_pTargetCom(nullptr)
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_fFrameMax(0.f)
	, m_fFrame(0.f)
	, m_eState(WALK)
	, m_bSetUp_Hit(true)
	, m_bSetUp_Die(true)
	, m_bSetUp_Atk(true)
	, m_bDieing(false)
	, m_bAtkTF(false)
	, m_bSearch_Change(true)
	, m_iCount(100)
	, m_iSearch_Change_Count(0)
	, m_eStage(SCENE_END)
{
	m_pObject_Manager->AddRef();
	m_pComponent_Manager->AddRef();
	m_vStartPos = *vStartPos;

	m_vRendomMovePos = m_vStartPos;
	m_vRendomMovePos.x += 1;
	m_vRendomMovePos.z += 1;

	m_tInfo.fCollisionSize = 0.5f;
	m_tInfo.fCollisionSearchSize = 5.f;
	m_tInfo.fCollisionAtkSearchSize = 5.f;
	m_tInfo.iHp = 15;

	m_eStage = eStage;

}

HRESULT CMonster_Imp::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &m_vStartPos);
	return NOERROR;
}

_int CMonster_Imp::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALHPATEST, this);

	return 0;
}

_int CMonster_Imp::LastUpdate_GameObject(const _float & fTimeDelta)
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
			CSoundMgr::GetInstance()->PlaySound(L"ImpDeath2.wav", CSoundMgr::Channel_Monster);
			m_bSetUp_Die = false;
			m_fFrame = 0.f;
		}
	}
	else
	{
		if (true == m_tInfo.bAtkSearch)
			m_tInfo.bAtk = true;
		else if (false == m_tInfo.bAtkSearch)
		{
			m_eState = WALK;
			m_tInfo.bAtk = false;
		}

		if (true == m_tInfo.bAtk)
		{
			if (100 <= m_iCount)
			{
				m_bSetUp_Atk = true;
				m_bAtkTF = true;
				m_iCount = 0;
			}
			else
			{
				m_eState = WALK;
				++m_iCount;
			}

			if (true == m_bAtkTF)
			{
				m_eState = ATTACK;

				if (true == m_bSetUp_Atk)
				{
					m_fFrame = 0;
					m_bSetUp_Atk = false;
				}
			}
		}


		if (true == m_tInfo.bHit)
		{
			m_eState = HIT;
			if (true == m_bSetUp_Hit)
			{
				m_bSetUp_Hit = false;
				m_fFrame = 0.f;
			}
		}
	}


	const _matrix* pmatView = m_pObject_Manager->Get_pCamera()->Get_TempView();
	_matrix matView = *pmatView;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_RIGHT, (_vec3*)&matView.m[Engine::CTransform::INFO_RIGHT][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_UP, (_vec3*)&matView.m[Engine::CTransform::INFO_UP][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, (_vec3*)&matView.m[Engine::CTransform::INFO_LOOK][0]);

	m_pTransformCom->Set_Scale(&_vec3(m_vScale.x, m_vScale.y, m_vScale.z));

	if (true == m_tInfo.bSearch)
	{
		if (false == m_bDieing)
		{
			if (false == m_tInfo.bAtk)
				PlayerPos_Check(fTimeDelta);
		}
	}
	else
	{
		if(false == m_bDieing)
			SearchMove_Check(fTimeDelta);
	}
	//else 로 돌아다니는거 만들수 있음..

	Move_OnTerrain();

	switch (m_eState)
	{
	case WALK:
		m_fFrameMax = 4.f;
		break;
	case WALK_LEFT:
		m_fFrameMax = 4.f;
		break;
	case WALK_RIGHT:
		m_fFrameMax = 4.f;
		break;
	case HIT:
		m_fFrameMax = 1.f;
		break;
	case ATTACK:
		m_fFrameMax = 3.f;
		break;
	case DIE:
		m_fFrameMax = 7.f;
		break;
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

		if (ATTACK == m_eState)
		{
			Create_Bullet(L"Monster_Bullet", fTimeDelta);
			m_bAtkTF = false;
		}


		if (DIE == m_eState)
		{
			
			m_fFrame = 6.f;
			m_tInfo.bDie = true;
		}
	}


	return 0;
}

void CMonster_Imp::Render_GameObject(void)
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
		m_pTextureCom_Walk->SetUp_OnGraphicDev(0, _uint(m_fFrame));
		break;
	case WALK_LEFT:
		m_pTextureCom_Walk_Left->SetUp_OnGraphicDev(0, _uint(m_fFrame));
		break;
	case WALK_RIGHT:
		m_pTextureCom_Walk_Right->SetUp_OnGraphicDev(0, _uint(m_fFrame));
		break;
	case HIT:
		m_pTextureCom_Hit->SetUp_OnGraphicDev(0, _uint(m_fFrame));
		break;
	case ATTACK:
		m_pTextureCom_Atk->SetUp_OnGraphicDev(0, _uint(m_fFrame));
		break;
	case DIE:
		m_pTextureCom_Die->SetUp_OnGraphicDev(0, _uint(m_fFrame));
		break;
	}
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);



}

HRESULT CMonster_Imp::Ready_Component(void)
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
	pComponent = m_pTextureCom_Walk = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Imp_Walk");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Imp_Walk", pComponent));
	m_pTextureCom_Walk->AddRef();

	// Texture_Walk_Left
	pComponent = m_pTextureCom_Walk_Left = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Imp_Walk_Left");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Imp_Walk_Left", pComponent));
	m_pTextureCom_Walk_Left->AddRef();

	// Texture_Walk_Right
	pComponent = m_pTextureCom_Walk_Right = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Imp_Walk_Right");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Imp_Walk_Right", pComponent));
	m_pTextureCom_Walk_Right->AddRef();


	// Texture_Hit
	pComponent = m_pTextureCom_Hit = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Imp_Hit");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Imp_Hit", pComponent));
	m_pTextureCom_Hit->AddRef();

	// Texture_Atk
	pComponent = m_pTextureCom_Atk = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Imp_Atk");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Imp_Atk", pComponent));
	m_pTextureCom_Atk->AddRef();

	// Texture_Die
	pComponent = m_pTextureCom_Die = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Imp_Die");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Imp_Die", pComponent));
	m_pTextureCom_Die->AddRef();

	// Renderer
	pComponent = m_pRendererCom = (Engine::CRenderer*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));
	m_pRendererCom->AddRef();

	return NOERROR;
}

void CMonster_Imp::Move_OnTerrain(void)
{
	if (nullptr == m_pTransformCom)
		return;

	if (nullptr == m_pTargetCom)
	{
		m_pTargetCom = (CVIBuffer*)m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Terrain", 0, L"Com_Buffer");
		m_pTargetCom->AddRef();
	}

	m_pTransformCom->Move_OnBuffer(m_pTargetCom, 0.38f);
}

void CMonster_Imp::PlayerPos_Check(float fTimeDelta)
{
	CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
	_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);

	_vec3 vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);

	m_pTransformCom->Set_Infomation(CTransform::INFO_LOOK, &vLook);

	m_pTransformCom->Go_Straight(1.f, fTimeDelta);



}

void CMonster_Imp::SearchMove_Check(float fTimeDelta)
{
	if (100 <= m_iSearch_Change_Count)
	{
		if (true == m_bSearch_Change)
		{
			m_bSearch_Change = false;
			m_iSearch_Change_Count = 0;
		}
		else
		{
			m_bSearch_Change = true;
			m_iSearch_Change_Count = 0;
		}
	}

	if (true == m_bSearch_Change)
	{
		_vec3 vLook = m_vRendomMovePos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);

		vLook *= 0.5f * fTimeDelta;
		vLook += *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
		m_pTransformCom->Set_Infomation(CTransform::INFO_POSITION, &vLook);

		m_eState = WALK_RIGHT;
	}
	else if (false == m_bSearch_Change)
	{
		_vec3 vLook = m_vStartPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);

		vLook *= 0.5f * fTimeDelta;
		vLook += *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
		m_pTransformCom->Set_Infomation(CTransform::INFO_POSITION, &vLook);

		m_eState = WALK_LEFT;
	}

	++m_iSearch_Change_Count;
}

HRESULT CMonster_Imp::Create_Bullet(const _tchar * pLayertag, float fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
	_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);

	_vec3 vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);

	vLook.y = 0.f;


	pGameObject = CMonster_Imp_Bullet_Collision::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vLook, m_eStage);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, pLayertag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

CMonster_Imp * CMonster_Imp::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vStartPos, SCENETYPE eStage)
{
	CMonster_Imp *	pInstance = new CMonster_Imp(pGraphicDev, vStartPos, eStage);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CMonster_Imp Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CMonster_Imp::Free(void)
{
	_ulong		dwRefcnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom_Walk);
	Engine::Safe_Release(m_pTextureCom_Walk_Left);
	Engine::Safe_Release(m_pTextureCom_Walk_Right);
	Engine::Safe_Release(m_pTextureCom_Hit);
	Engine::Safe_Release(m_pTextureCom_Atk);
	Engine::Safe_Release(m_pTextureCom_Die);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTargetCom);
	Engine::Safe_Release(m_pComponent_Manager);
	Engine::Safe_Release(m_pObject_Manager);


	dwRefcnt = Engine::CGameObject::Free();

	return dwRefcnt;
}
