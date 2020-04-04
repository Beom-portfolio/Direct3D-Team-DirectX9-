#include "stdafx.h"
#include "..\Headers\Bullet_Effect_Blue.h"

#include "Component_Manager.h"
#include "Object_Manager.h"

CBullet_Effect_Blue::CBullet_Effect_Blue(LPDIRECT3DDEVICE9 pGraphicDev, SCENETYPE eStage)
	:CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pRectTextureCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_fFrame(0.f)
	, m_pObjectManager(CObject_Manager::GetInstance())
	, m_eStage(SCENE_END)
{
	m_pObjectManager->AddRef();
	m_eStage = eStage;
}



HRESULT CBullet_Effect_Blue::Ready_GameObject(const _vec3* vPos)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (nullptr == m_pTransformCom)
		return E_FAIL;


	//m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, vLook);

	m_vPos = *vPos;
	m_vStartPos = m_vPos;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &m_vStartPos);

	m_tInfo.iHp = 1;


	return NOERROR;
}

_int CBullet_Effect_Blue::Update_GameObject(const _float & fTimeDelta)
{
	m_fFrame += 20.f * fTimeDelta;

	if (m_fFrame >= 8.f)
	{
		//m_fFrame = 16.f;
		m_tInfo.iHp = 0;
	}


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALHPATEST, this);


	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

_int CBullet_Effect_Blue::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (0 >= m_tInfo.iHp)
		m_tInfo.bDie = true;

	if (true == m_tInfo.bDie)
	{
		return 1;
	}

	// ºôº¸µå
	const _matrix* pmatView = m_pObjectManager->Get_pCamera()->Get_TempView();
	_matrix matView = *pmatView;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_RIGHT, (_vec3*)&matView.m[Engine::CTransform::INFO_RIGHT][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_UP, (_vec3*)&matView.m[Engine::CTransform::INFO_UP][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, (_vec3*)&matView.m[Engine::CTransform::INFO_LOOK][0]);

	m_pTransformCom->Set_Scale(&_vec3(0.4f, 0.4f, 0.4f));


	m_fFrame += 10.f * fTimeDelta;

	if (6.f <= m_fFrame)
	{
		m_fFrame = 0.f;
		m_tInfo.bDie = true;
	}

	return 0;
}

void CBullet_Effect_Blue::Render_GameObject(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0, _int(m_fFrame));
	m_pRectTextureCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBullet_Effect_Blue::Ready_Component()
{
	Engine::CComponent*			pComponent;

	pComponent = m_pTransformCom = DCAST(Engine::CTransform*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Transform_com", pComponent));
	m_pTransformCom->AddRef();

	pComponent = m_pRectTextureCom = DCAST(Engine::CRect_Texture*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"RectTex_com", pComponent));
	m_pRectTextureCom->AddRef();

	pComponent = m_pRendererCom = DCAST(Engine::CRenderer*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Renderer_com", pComponent));
	m_pRendererCom->AddRef();

	pComponent = m_pTextureCom = DCAST(Engine::CTexture*, CComponent_Manager::GetInstance()->Clone_Component(m_eStage, L"Component_Texture_Stage2Boss_Bullet_Blue_Effect"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Texture_Blue_Effect_com", pComponent));
	m_pTextureCom->AddRef();

	return NOERROR;
}

CBullet_Effect_Blue * CBullet_Effect_Blue::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos, SCENETYPE eStage)
{
	CBullet_Effect_Blue* pInstance = new CBullet_Effect_Blue(pGraphicDev, eStage);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		MSG_BOX("CBullet_Effect_Blue Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CBullet_Effect_Blue::Free()
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRectTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pObjectManager);
	Engine::Safe_Release(m_pTextureCom);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
