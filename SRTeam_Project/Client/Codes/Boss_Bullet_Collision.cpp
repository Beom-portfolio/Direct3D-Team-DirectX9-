#include "stdafx.h"
#include "..\Headers\Boss_Bullet_Collision.h"

#include "Object_Manager.h"
#include "Component_Manager.h"

CBoss_Bullet_Collision::CBoss_Bullet_Collision(LPDIRECT3DDEVICE9 pGraphicDev, SCENETYPE eStage)
	:CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pRectTextureCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pObjectManager(CObject_Manager::GetInstance())
	, m_fFrame(0.f)
{
	m_pObjectManager->AddRef();
	m_eStage = eStage;
}

HRESULT CBoss_Bullet_Collision::Ready_GameObject(const _vec3 * vPos, const _vec3 * vLook)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, vLook);


	m_vLook = *vLook;
	m_vPos = *vPos;
	m_vStartPos = m_vPos;

	D3DXVec3Normalize(&m_vLook, &m_vLook);

	m_tInfo.fCollisionSize = 0.5f;
	m_tInfo.iAtk = 20;

	return NOERROR;

}

_int CBoss_Bullet_Collision::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALHPATEST, this);

	if (m_vStartPos.x + 5.f <= m_vPos.x || m_vStartPos.x - 5.f >= m_vPos.x || m_vStartPos.z + 5.f <= m_vPos.z || m_vStartPos.z - 5.f >= m_vPos.z)
		m_tInfo.iHp = 0;

	return 0;

}

_int CBoss_Bullet_Collision::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (0 >= m_tInfo.iHp)
		m_tInfo.bDie = true;

	if (true == m_tInfo.bDie)
		return 1;



	// ������
	const _matrix* pmatView = m_pObjectManager->Get_pCamera()->Get_TempView();
	_matrix matView = *pmatView;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_RIGHT, (_vec3*)&matView.m[Engine::CTransform::INFO_RIGHT][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_UP, (_vec3*)&matView.m[Engine::CTransform::INFO_UP][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, (_vec3*)&matView.m[Engine::CTransform::INFO_LOOK][0]);


	m_pTransformCom->Set_Scale(&_vec3(0.2f, 0.2f, 0.2f));


	//Bullet Move
	m_vPos += m_vLook * 5.f * fTimeDelta;
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &m_vPos);



	m_fFrame += 30.f * fTimeDelta;

	if (8.f <= m_fFrame)
		m_fFrame = 0.f;

	return 0;
}

void CBoss_Bullet_Collision::Render_GameObject(void)
{
	if (nullptr == m_pRectTextureCom)
		return;
	if (nullptr == m_pTextureCom)
		return;



	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0, _uint(m_fFrame));
	m_pRectTextureCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBoss_Bullet_Collision::Ready_Component()
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

	//���߿� ���� �ҷ����� �ٲ���
	//Texture
	pComponent = m_pTextureCom = DCAST(Engine::CTexture*, CComponent_Manager::GetInstance()->Clone_Component(m_eStage, L"Component_Texture_Stage1Boss_Bullet"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Texture_Imp_Bullet_com", pComponent));
	m_pTextureCom->AddRef();

	return NOERROR;
}

CBoss_Bullet_Collision * CBoss_Bullet_Collision::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * vPos, const _vec3 * vLook, SCENETYPE eStage)
{
	CBoss_Bullet_Collision* pInstance = new CBoss_Bullet_Collision(pGraphicDev, eStage);
	if (FAILED(pInstance->Ready_GameObject(vPos, vLook)))
	{
		MSG_BOX("CMonster_Imp_Bullet_Collision Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CBoss_Bullet_Collision::Free()
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRectTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pObjectManager);


	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
