#include "stdafx.h"
#include "..\Headers\Stage1_Boss.h"

#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Client_Player.h"
#include "Camera.h"
#include "Boss_Bullet_Collision.h"
#include "Boss_Bullet_Collision1.h"
#include "Boss_Bullet_Collision2.h"
#include "Hit_Effect.h"


CStage1_Boss::CStage1_Boss(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vStartPos, SCENETYPE eStage)
	: Engine::CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom_Walk(nullptr)
	, m_pTextureCom_Atk(nullptr)
	, m_pTextureCom_Die(nullptr)
	, m_pTargetCom(nullptr)
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_fFrameMax(0.f)
	, m_fFrame(0.f)
	, m_eState(WALK)
	, m_bDieing(false)
	, m_eStage(SCENE_END)
	, m_bSetUp_Die(true)
	, m_bSetUpPattern(false)
	, m_bSetUp_Pos(true)
	, m_bSetUp_Pattern_Change(true)
	, m_bSetUp_ShotCount(true)
	, m_iPattern(PATTERN1)
	, m_iPattern_Count(1)
	, m_bShotBullet(true)
	, m_iPattern_Change_Count(0)
	, m_iDelay(0)
{
	m_pObject_Manager->AddRef();
	m_pComponent_Manager->AddRef();
	m_vStartPos = *vStartPos;

	m_tInfo.fCollisionSearchSize = 15.f;
	m_tInfo.fCollisionAtkSearchSize = 10.f;
	m_tInfo.iHp = 500;

	m_eStage = eStage;

}


CStage1_Boss::~CStage1_Boss()
{
}

HRESULT CStage1_Boss::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &m_vStartPos);

	m_tInfo.bAtk = false;

	m_tInfo.iHp = 500;
	m_tInfo.iHpMax = m_tInfo.iHp;
	m_tInfo.fSpeed = 2.f;

	return NOERROR;
}

_int CStage1_Boss::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALHPATEST, this);

	return 0;
}

_int CStage1_Boss::LastUpdate_GameObject(const _float & fTimeDelta)
{
	const _matrix* pmatView = m_pObject_Manager->Get_pCamera()->Get_TempView();
	_matrix matView = *pmatView;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_RIGHT, (_vec3*)&matView.m[Engine::CTransform::INFO_RIGHT][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_UP, (_vec3*)&matView.m[Engine::CTransform::INFO_UP][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, (_vec3*)&matView.m[Engine::CTransform::INFO_LOOK][0]);
	m_pTransformCom->Set_Scale(&_vec3(0.7f, 0.7f, 0.7f));

	

	if (true == m_tInfo.bEffectOn)
	{
		Engine::CGameObject*	pGameObject = nullptr;
		const _vec3* vPos = m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION);

		CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
		_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);

		_vec3 vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
		vLook.y = 0.f;
		pGameObject = CHit_Effect::Create(m_pGraphicDev, vPos, &vLook, m_eStage);

		m_tInfo.bEffectOn = false;

		if (nullptr == pGameObject)
			return 1;

		if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, L"BFG_Effect", pGameObject)))
			return 1;
	}


	if (0 >= m_tInfo.iHp)
		m_bDieing = true;

	if (true == m_tInfo.bDie)
	{
		return 1;
	}

	float fHp = (float)m_tInfo.iHp / (float)m_tInfo.iHpMax * 100.f;

	if (100 <= fHp)
	{
		if (true == m_bSetUp_Pattern_Change)
		{

		}
	}


	if (50 >= fHp)
	{
		if (true == m_bSetUp_Pattern_Change)
		{
			m_iPattern = PATTERN2;
			m_iPattern_Count = 1;
			m_iPattern_Change_Count = 0;
			m_fFrame = 0.f;
			m_bShotBullet = true;
			m_bSet_StartPos = true;

			m_bSetUp_Pattern_Change = false;

		}
	}


	if (true == m_tInfo.bSearch)//패턴시작
		m_bSetUpPattern = true;
	//else 로 돌아다니는거 만들수 있음..

	if (true == m_bDieing)
	{
		m_eState = DIE;

		if (true == m_bSetUp_Die)
		{
			m_bSetUp_Die = false;
			m_fFrame = 0.f;
		}
	}
	else //죽지않는이상 돌리는 구간.
	{
		if (true == m_bSetUpPattern)
		{
			if (5 <= m_iPattern_Count)
				m_iPattern_Count = 1;

			BossPattern(fTimeDelta);
		}
		else
		{
			m_eState = WALK;
		}

	}




	switch (m_eState)
	{
	case WALK:
		m_fFrameMax = 4.f;
		break;
	case ATTACK:
		m_fFrameMax = 4.f;
		break;
	case DIE:
		m_fFrameMax = 4.f;
		break;
	}


	m_fFrame += 5.f * fTimeDelta;

	if (m_fFrame >= m_fFrameMax)
	{
		m_fFrame = 0.f;

		if (ATTACK == m_eState)
			m_bShotBullet = true;

		if(ATTACK == m_eState && PATTERN1 == m_iPattern && 2 == m_iPattern_Count)
			Create_Bullet2(L"Monster_Bullet");

		if (DIE == m_eState)
		{
			m_fFrame = 3.f;
			m_tInfo.bDie = true;
		}
	}

	Move_OnTerrain();
	return 0;
}

void CStage1_Boss::Render_GameObject(void)
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

HRESULT CStage1_Boss::Ready_Component(void)
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
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer_Stage1_Boss", pComponent));
	m_pBufferCom->AddRef();

	// Renderer
	pComponent = m_pRendererCom = (Engine::CRenderer*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));
	m_pRendererCom->AddRef();


	// Texture_Walk
	pComponent = m_pTextureCom_Walk = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Stage1Boss_Walk");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Component_Texture_Stage1Boss_Walk", pComponent));
	m_pTextureCom_Walk->AddRef();

	// Texture_Atk
	pComponent = m_pTextureCom_Atk = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Stage1Boss_Atk");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Component_Texture_Stage1Boss_Atk", pComponent));
	m_pTextureCom_Atk->AddRef();

	// Texture_Die
	pComponent = m_pTextureCom_Die = (Engine::CTexture*)m_pComponent_Manager->Clone_Component(m_eStage, L"Component_Texture_Stage1Boss_Die");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Component_Texture_Stage1Boss_Die", pComponent));
	m_pTextureCom_Die->AddRef();

	

	return NOERROR;
}

void CStage1_Boss::Move_OnTerrain(void)
{
	if (nullptr == m_pTransformCom)
		return;

	if (nullptr == m_pTargetCom)
	{
		m_pTargetCom = (CVIBuffer*)m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Terrain", 0, L"Com_Buffer");
		m_pTargetCom->AddRef();
	}

	m_pTransformCom->Move_OnBuffer(m_pTargetCom, 0.7f);
}

void CStage1_Boss::PlayerPos_Check(float fTimeDelta)
{
	CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
	_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);

	_vec3 vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);

	m_pTransformCom->Set_Infomation(CTransform::INFO_LOOK, &vLook);

	m_pTransformCom->Go_Straight(m_tInfo.fSpeed, fTimeDelta);
}


HRESULT CStage1_Boss::Create_Bullet1(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
	_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);

	_vec3 vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);

	vLook.y = 0.f;


	pGameObject = CBoss_Bullet_Collision::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vLook, m_eStage);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, pLayertag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CStage1_Boss::Create_Bullet2(const _tchar * pLayertag)
{
	Engine::CGameObject*		pGameObject = nullptr;

	
	_vec3 vLook = m_vTempStartPosBullet - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
	vLook.y = 0.f;

	pGameObject = CBoss_Bullet_Collision1::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vLook, m_eStage);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, pLayertag, pGameObject)))
		return E_FAIL;

	vLook = m_vTempStartPosBullet - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
	_vec3 vUp = { 0.f, 1.f, 0.f };
	_vec3 vRight = { 0.f, 0.f, 0.f };
	_vec3 vLeft = { 0.f, 0.f, 0.f };


	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Cross(&vRight, &vUp, &vLook);
	D3DXVec3Cross(&vLeft, &vLook, &vUp);

	_vec3 vResultLook = (m_vTempStartPosBullet + vRight * 0.5f) - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
	vResultLook.y = 0.f;
	//오른쪽
	pGameObject = CBoss_Bullet_Collision1::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vResultLook, m_eStage);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, pLayertag, pGameObject)))
		return E_FAIL;



	vResultLook = (m_vTempStartPosBullet + vLeft * 0.5f) - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
	vResultLook.y = 0.f;

	//왼쪽
	pGameObject = CBoss_Bullet_Collision1::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vResultLook, m_eStage);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, pLayertag, pGameObject)))
		return E_FAIL;




	m_vTempStartPosBullet += m_vTempShot * 2.f;


	return NOERROR;
}

HRESULT CStage1_Boss::Create_Bullet3(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
	_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);

	_vec3 vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
	vLook.y = 0.f;

	pGameObject = CBoss_Bullet_Collision::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vLook, m_eStage);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, pLayertag, pGameObject)))
		return E_FAIL;




	vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
	_vec3 vUp = { 0.f, 1.f, 0.f };
	_vec3 vRight = { 0.f, 0.f, 0.f };
	_vec3 vLeft = { 0.f, 0.f, 0.f };


	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Cross(&vRight, &vUp, &vLook);
	D3DXVec3Cross(&vLeft, &vLook, &vUp);

	_vec3 vResultLook = (vPlayerPos + vRight * 0.5f) - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
	vResultLook.y = 0.f;

	//오른쪽
	pGameObject = CBoss_Bullet_Collision::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vResultLook, m_eStage);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, pLayertag, pGameObject)))
		return E_FAIL;




	vResultLook = (vPlayerPos + vLeft * 0.5f) - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
	vResultLook.y = 0.f;

	//왼쪽
	pGameObject = CBoss_Bullet_Collision::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vResultLook, m_eStage);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, pLayertag, pGameObject)))
		return E_FAIL;


	return NOERROR;
}


HRESULT CStage1_Boss::Create_Bullet4(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	_vec3 vLook = m_vTempStartPosBullet - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
	vLook.y = 0.f;

	pGameObject = CBoss_Bullet_Collision2::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vLook, m_eStage);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, pLayertag, pGameObject)))
		return E_FAIL;




	vLook = m_vTempStartPosBullet - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
	_vec3 vUp = { 0.f, 1.f, 0.f };
	_vec3 vRight = { 0.f, 0.f, 0.f };
	_vec3 vLeft = { 0.f, 0.f, 0.f };


	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Cross(&vRight, &vUp, &vLook);
	D3DXVec3Cross(&vLeft, &vLook, &vUp);

	_vec3 vResultLook = (m_vTempStartPosBullet + vRight * 0.5f) - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
	vResultLook.y = 0.f;

	//오른쪽
	pGameObject = CBoss_Bullet_Collision2::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vResultLook, m_eStage);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, pLayertag, pGameObject)))
		return E_FAIL;




	vResultLook = (m_vTempStartPosBullet + vLeft * 0.5f) - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
	vResultLook.y = 0.f;

	//왼쪽
	pGameObject = CBoss_Bullet_Collision2::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vResultLook, m_eStage);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, pLayertag, pGameObject)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CStage1_Boss::Create_Bullet5(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
	_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);

	_vec3 vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);

	vLook.y = 0.f;


	pGameObject = CBoss_Bullet_Collision1::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION), &vLook, m_eStage);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(m_eStage, pLayertag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}


//패턴 시작  2가지
void CStage1_Boss::BossPattern(float fTimeDelta)
{
	switch (m_iPattern)
	{
	case PATTERN1:
		Pattern_One(fTimeDelta);
		break;
	case PATTERN2:
		Pattern_Two(fTimeDelta);
		break;
	}

}
//1가지에 4개씩
void CStage1_Boss::Pattern_One(float fTimeDelta)
{
	switch (m_iPattern_Count)
	{
	case 1:
		Boss_PlayPattern_One1(fTimeDelta);
		break;
	case 2:
		Boss_PlayPattern_One2(fTimeDelta);
		break;
	case 3:
		Boss_PlayPattern_One3(fTimeDelta);
		break;
	case 4:
		Boss_PlayPattern_One4(fTimeDelta);
		break;
	}
}
//1가지에 4개씩
void CStage1_Boss::Pattern_Two(float fTimeDelta)
{
	switch (m_iPattern_Count)
	{
	case 1:
		Boss_PlayPattern_Two1(fTimeDelta);
		break;
	case 2:
		Boss_PlayPattern_Two2(fTimeDelta);
		break;
	case 3:
		Boss_PlayPattern_Two3(fTimeDelta);
		break;
	case 4:
		Boss_PlayPattern_Two4(fTimeDelta);
		break;
	}
}




// 패턴구간
void CStage1_Boss::Boss_PlayPattern_One1(float fTimeDelta)
{

	m_eState = WALK;
	PlayerPos_Check(fTimeDelta);

	if (true == m_bSetUp_ShotCount)
	{
		m_iShotCount = 50;
		m_bSetUp_ShotCount = false;
	}


	if (10 >= m_iShotCount)
		m_eState = ATTACK;

	if (50 <= m_iShotCount)
	{
		m_tInfo.bAtk = true;
		m_iShotCount = 0;
	}

	if (2 <= m_fFrame && true == m_tInfo.bAtk)
	{
		m_eState = ATTACK;

		Create_Bullet1(L"Monster_Bullet");
		m_tInfo.bAtk = false;

		++m_iPattern_Change_Count;
	}


	++m_iShotCount;

	if (3 <= m_iPattern_Change_Count)
	{
		m_iShotCount = 0;
		m_iPattern_Change_Count = 0;

		m_bSetUp_ShotCount = true;
		m_tInfo.bAtk = false;
		m_bShotBullet = true;

		++m_iPattern_Count;
	}


}

void CStage1_Boss::Boss_PlayPattern_One2(float fTimeDelta)
{
	m_eState = ATTACK;

	if (true == m_bSet_StartPos)
	{
		CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
		_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);

		_vec3 vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
		vLook.y = 0.f;
		_vec3 vUp = { 0.f, 1.f, 0.f };
		_vec3 vRight = { 0.f, 0.f, 0.f };
		_vec3 vLeft = { 0.f, 0.f, 0.f };


		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Cross(&vRight, &vUp, &vLook);
		D3DXVec3Cross(&vLeft, &vLook, &vUp);

		vRight *= 4.f;

		m_vTempStartPosBullet = vPlayerPos + vRight;
		D3DXVec3Normalize(&vRight, &vRight);
		m_vTempShot = vLeft;


		m_bSet_StartPos = false;
	}


	if (2 <= m_fFrame && true == m_bShotBullet)
	{
		Create_Bullet2(L"Monster_Bullet");
		m_bShotBullet = false;
		++m_iPattern_Change_Count;

		if (4 <= m_iPattern_Change_Count)
		{
			m_iPattern_Change_Count = 0;
			++m_iPattern_Count;
			m_bShotBullet = true;
			m_bSet_StartPos = true;
		}
	}



}

void CStage1_Boss::Boss_PlayPattern_One3(float fTimeDelta)
{
	m_eState = WALK;
	PlayerPos_Check(fTimeDelta);

	if (true == m_bSetUp_ShotCount)
	{
		m_iShotCount = 50;
		m_bSetUp_ShotCount = false;
	}


	if (10 >= m_iShotCount)
		m_eState = ATTACK;

	if (50 <= m_iShotCount)
	{
		m_tInfo.bAtk = true;
		m_iShotCount = 0;
	}

	if (2 <= m_fFrame && true == m_tInfo.bAtk)
	{
		m_eState = ATTACK;

		Create_Bullet1(L"Monster_Bullet");
		m_tInfo.bAtk = false;

		++m_iPattern_Change_Count;
	}


	++m_iShotCount;

	if (3 <= m_iPattern_Change_Count)
	{
		m_iShotCount = 0;
		m_iPattern_Change_Count = 0;

		m_bSetUp_ShotCount = true;
		m_tInfo.bAtk = false;
		m_bShotBullet = true;

		++m_iPattern_Count;
	}
}

void CStage1_Boss::Boss_PlayPattern_One4(float fTimeDelta)
{
	m_eState = WALK;
	PlayerPos_Check(fTimeDelta);

	if (true == m_bSetUp_ShotCount)
	{
		m_iShotCount = 50;
		m_bSetUp_ShotCount = false;
	}


	if (10 >= m_iShotCount)
		m_eState = ATTACK;

	if (50 <= m_iShotCount)
	{
		m_tInfo.bAtk = true;
		m_iShotCount = 0;
	}

	if (2 <= m_fFrame && true == m_tInfo.bAtk)
	{
		m_eState = ATTACK;

		Create_Bullet3(L"Monster_Bullet");
		m_tInfo.bAtk = false;

		++m_iPattern_Change_Count;
	}


	++m_iShotCount;

	if (3 <= m_iPattern_Change_Count)
	{
		m_iShotCount = 0;
		m_iPattern_Change_Count = 0;

		m_bSetUp_ShotCount = true;
		m_tInfo.bAtk = false;
		m_bShotBullet = true;

		++m_iPattern_Count;
	}
}




void CStage1_Boss::Boss_PlayPattern_Two1(float fTimeDelta)
{
	m_eState = ATTACK;

	if (true == m_bSetUp_ShotCount)
	{
		m_iShotCount = 20;
		m_bSetUp_ShotCount = false;
	}


	if (true == m_bSet_StartPos && PATTERN2 == m_iPattern)
	{
		m_bSetUp_Pos = false;

		_vec3 vPos = *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
		_vec3 TempPos = m_vStartPos;
		TempPos.y = vPos.y;

		m_pTransformCom->Set_Infomation(CTransform::INFO_POSITION, &TempPos);

	}


	if (0 == m_iPattern_Change_Count && true == m_bSet_StartPos)
	{
		CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
		_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);

		_vec3 vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
		vLook.y = 0.f;
		_vec3 vUp = { 0.f, 1.f, 0.f };
		_vec3 vRight = { 0.f, 0.f, 0.f };


		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Cross(&vRight, &vUp, &vLook);
		vRight *= 1.0f;
		m_vTempStartPosBullet = vPlayerPos + vRight;

		m_bShotBullet = true;
		m_bSet_StartPos = false;
	}

	if (1 == m_iPattern_Change_Count && true == m_bSet_StartPos)
	{
		CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
		_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);
		m_vTempStartPosBullet = vPlayerPos;


		m_bShotBullet = true;
		m_bSet_StartPos = false;
	}

	if (2 == m_iPattern_Change_Count && true == m_bSet_StartPos)
	{
		CComponent* pPlayerCom = m_pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", 0, L"Transform_com");
		_vec3 vPlayerPos = *dynamic_cast<CTransform*>(pPlayerCom)->Get_Infomation(CTransform::INFO_POSITION);

		_vec3 vLook = vPlayerPos - *m_pTransformCom->Get_Infomation(CTransform::INFO_POSITION);
		vLook.y = 0.f;
		_vec3 vUp = { 0.f, 1.f, 0.f };
		_vec3 vLeft = { 0.f, 0.f, 0.f };


		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Cross(&vLeft, &vLook, &vUp);
		vLeft *= 1.0f;
		m_vTempStartPosBullet = vPlayerPos + vLeft;


		m_bShotBullet = true;
		m_bSet_StartPos = false;
	}


	if (2.f <= m_fFrame && true == m_bShotBullet && 30 <= m_iShotCount)
	{
		m_iShotCount = 0;

		Create_Bullet4(L"Monster_Bullet");
		++m_iPattern_Change_Count;
		m_bSet_StartPos = true;
		m_bShotBullet = false;

	}


	++m_iShotCount;


	if (3 <= m_iPattern_Change_Count)
	{
		m_iPattern_Change_Count = 0;
		++m_iPattern_Count;
		m_bSet_StartPos = true;
		m_bShotBullet = true;
		m_bSetUp_ShotCount = true;
		m_iShotCount = 0;
	}

}

void CStage1_Boss::Boss_PlayPattern_Two2(float fTimeDelta)
{
	Create_Bullet5(L"Monster_Bullet");
	++m_iPattern_Change_Count;

	if (50 <= m_iPattern_Change_Count)
	{
		m_iPattern_Change_Count = 0;
		++m_iPattern_Count;
		m_bSet_StartPos = true;
		m_bShotBullet = true;
	}

}

void CStage1_Boss::Boss_PlayPattern_Two3(float fTimeDelta)
{
	m_eState = WALK;
	PlayerPos_Check(fTimeDelta);


	if (10 >= m_iShotCount)
		m_eState = ATTACK;

	if (50 <= m_iShotCount)
	{
		m_tInfo.bAtk = true;
		m_iShotCount = 0;
	}

	if (2 <= m_fFrame && true == m_tInfo.bAtk)
	{
		m_eState = ATTACK;

		Create_Bullet3(L"Monster_Bullet");
		m_tInfo.bAtk = false;

		++m_iPattern_Change_Count;
	}



	++m_iShotCount;


	if (5 <= m_iPattern_Change_Count)
	{
		m_iPattern_Change_Count = 0;
		m_tInfo.bAtk = false;
		++m_iPattern_Count;
		m_iShotCount = 0;
	}


}

void CStage1_Boss::Boss_PlayPattern_Two4(float fTimeDelta)
{
	Create_Bullet5(L"Monster_Bullet");
	++m_iPattern_Change_Count;

	if (50 <= m_iPattern_Change_Count)
	{
		m_iPattern_Change_Count = 0;
		++m_iPattern_Count;
		m_bSet_StartPos = true;
		m_bShotBullet = true;
	}

}


CStage1_Boss * CStage1_Boss::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * vStartPos, SCENETYPE eStage)
{
	CStage1_Boss *	pInstance = new CStage1_Boss(pGraphicDev, vStartPos, eStage);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CStage1_Boss Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CStage1_Boss::Free(void)
{
	_ulong		dwRefcnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom_Walk);
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
