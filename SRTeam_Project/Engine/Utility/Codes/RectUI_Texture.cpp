#include "..\Headers\RectUI_Texture.h"

USING(Engine)


CRectUI_Texture::CRectUI_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
	, m_vUIPos(_vec3(0.f, 0.f, 0.f))
	, m_fWidth(1)
	, m_fHeight(1)
{
}

CRectUI_Texture::CRectUI_Texture(const CRectUI_Texture & rhs)
	:CVIBuffer(rhs)
	, m_vUIPos(_vec3(0.f, 0.f, 0.f))
	, m_fWidth(1)
	, m_fHeight(1)
{
}

HRESULT CRectUI_Texture::Ready_Buffer()
{
	m_iVertexSize = sizeof(VTXTEX_VIEWPORT);
	m_iVertexCnt = 4;
	m_dwVertexFVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;
	m_iTriCnt = 2;


	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	if (nullptr == m_pVB)
		return E_FAIL;

	VTXTEX_VIEWPORT*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(m_vUIPos.x - (m_fWidth * 0.5f), m_vUIPos.y - (m_fHeight * 0.5f), 0, 1);
	pVertex[0].vTexUV = _vec2(0.0f, 0.f);

	pVertex[1].vPosition = _vec4(m_vUIPos.x + (m_fWidth * 0.5f), m_vUIPos.y - (m_fHeight * 0.5f), 0, 1);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4(m_vUIPos.x + (m_fWidth * 0.5f), m_vUIPos.y + (m_fHeight * 0.5f), 0, 1);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec4(m_vUIPos.x - (m_fWidth * 0.5f), m_vUIPos.y + (m_fHeight * 0.5f), 0, 1);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	//pVertex[0].vPosition = _vec4(-50.0f, -50.f, 0.0f, 1.f);
	//pVertex[0].vTexUV = _vec2(0.0f, 0.f);

	//pVertex[1].vPosition = _vec4(50.f, -50.f, 0.0f, 1.f);
	//pVertex[1].vTexUV = _vec2(1.f, 0.f);

	//pVertex[2].vPosition = _vec4(50.f, 50.f, 0.0f, 1.f);
	//pVertex[2].vTexUV = _vec2(1.f, 1.f);

	//pVertex[3].vPosition = _vec4(-50.f, 50.f, 0.0f, 1.f);
	//pVertex[3].vTexUV = _vec2(0.f, 1.f);


	m_pVB->Unlock();

	INDEX16*			pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

CComponent * CRectUI_Texture::Clone(void)
{
	return new CRectUI_Texture(*this);
}

void CRectUI_Texture::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

void CRectUI_Texture::SetUp_Buffer(const _vec3 * vUIPos, const _float & fWidth, const _float & fHeight)
{

	m_vUIPos = *vUIPos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	if (nullptr == m_pVB)
		return;

	VTXTEX_VIEWPORT*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(m_vUIPos.x - (m_fWidth * 0.5f), m_vUIPos.y - (m_fHeight * 0.5f), 0, 1);
	pVertex[0].vTexUV = _vec2(0.0f, 0.f);

	pVertex[1].vPosition = _vec4(m_vUIPos.x + (m_fWidth * 0.5f), m_vUIPos.y - (m_fHeight * 0.5f), 0, 1);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4(m_vUIPos.x + (m_fWidth * 0.5f), m_vUIPos.y + (m_fHeight * 0.5f), 0, 1);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec4(m_vUIPos.x - (m_fWidth * 0.5f), m_vUIPos.y + (m_fHeight * 0.5f), 0, 1);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*			pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();


}

CRectUI_Texture * CRectUI_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRectUI_Texture* pInstance = new CRectUI_Texture(pGraphicDev);

	

	if (FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("CRectUI_Texture Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CRectUI_Texture::Free()
{
	return Engine::CVIBuffer::Free();
}
