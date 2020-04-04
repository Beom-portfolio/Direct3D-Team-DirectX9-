#include "stdafx.h"
#include "..\Headers\Plasma_Effect.h"


#include "Object_Manager.h"
#include "Component_Manager.h"

CPlasma_Effect::CPlasma_Effect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pRectTextureCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_fFrame(0.f)
	, m_pObjectManager(CObject_Manager::GetInstance())
{
	//m_pObjectManager->AddRef();
}
HRESULT CPlasma_Effect::Ready_GameObject(const _vec3 * vPos)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (nullptr == m_pTransformCom)
		return E_FAIL;


	//m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, vLook);

	m_vPos = *vPos;
	m_vStartPos = m_vPos;


	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos);

	m_tInfo.iHp = 1;


	return NOERROR;
}

_int CPlasma_Effect::Update_GameObject(const _float & fTimeDelta)
{

	m_fFrame += 8.f * fTimeDelta;

	if (m_fFrame >= 12.f)
	{
		m_fFrame = 12.f;
		m_tInfo.iHp = 0;
	}


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALHPATEST, this);

	// ºôº¸µå
	const _matrix* pmatView = m_pObjectManager->Get_pCamera()->Get_TempView();
	_matrix matView = *pmatView;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_RIGHT, (_vec3*)&matView.m[Engine::CTransform::INFO_RIGHT][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_UP, (_vec3*)&matView.m[Engine::CTransform::INFO_UP][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, (_vec3*)&matView.m[Engine::CTransform::INFO_LOOK][0]);

	m_pTransformCom->Set_Scale(&_vec3(0.4f, 0.4f, 0.4f));

	// ºôº¸µå


	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

_int CPlasma_Effect::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (0 >= m_tInfo.iHp)
		m_tInfo.bDie = true;



	if (true == m_tInfo.bDie)
	{
		return 1;
	}

	return 0;
}

void CPlasma_Effect::Render_GameObject(void)
{

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	/*m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/

	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0, _int(m_fFrame));
	m_pRectTextureCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlasma_Effect::Ready_Component()
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

	pComponent = m_pTextureCom = DCAST(Engine::CTexture*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Plasma_Effect"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Texture_Plasma_Effect_com", pComponent));
	m_pTextureCom->AddRef();

	return NOERROR;
}

CPlasma_Effect * CPlasma_Effect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * vPos)
{
	CPlasma_Effect* pInstance = new CPlasma_Effect(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		MSG_BOX("CBFG_Effect Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CPlasma_Effect::Free()
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRectTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	//Engine::Safe_Release(m_pObjectManager);
	Engine::Safe_Release(m_pTextureCom);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
