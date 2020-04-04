#include "..\Headers\Sky_Cube_Texture.h"

USING(Engine)


CSky_Cube_Texture::CSky_Cube_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CSky_Cube_Texture::CSky_Cube_Texture(const CSky_Cube_Texture& rhs)
	: CVIBuffer(rhs)
{
}

CSky_Cube_Texture::~CSky_Cube_Texture()
{
}

HRESULT CSky_Cube_Texture::Ready_Buffer(void)
{
	m_iVertexSize = sizeof(VTXCUBETEX);
	m_iVertexCnt = 8;
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;
	m_iTriCnt = 12;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	if (nullptr == m_pVB)
		return E_FAIL;

	VTXCUBETEX*			pVertex = nullptr;


	// Lock : 다른스레드가 접근할 수 없도록 막는다. (값을 갱신하는 중이니까)
	//      : 잠겨진 공간의 주소를 얻어온다.(값을 갱신하려고)
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 앞면
	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, -0.5f);
	pVertex[0].vTexUV = pVertex[0].vPosition;

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, -0.5f);
	pVertex[1].vTexUV = pVertex[1].vPosition;

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, -0.5f);
	pVertex[2].vTexUV = pVertex[2].vPosition;

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, -0.5f);
	pVertex[3].vTexUV = pVertex[3].vPosition;

	// 뒷면
	pVertex[4].vPosition = _vec3(-0.5f, 0.5f, 0.5f);
	pVertex[4].vTexUV = pVertex[4].vPosition;

	pVertex[5].vPosition = _vec3(0.5f, 0.5f, 0.5f);
	pVertex[5].vTexUV = pVertex[5].vPosition;

	pVertex[6].vPosition = _vec3(0.5f, -0.5f, 0.5f);
	pVertex[6].vTexUV = pVertex[6].vPosition;

	pVertex[7].vPosition = _vec3(-0.5f, -0.5f, 0.5f);
	pVertex[7].vTexUV = pVertex[7].vPosition;

	m_pVB->Unlock();

	INDEX16*			pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x 
	pIndex[0]._0 = 1; pIndex[0]._1 = 5; pIndex[0]._2 = 6;
	pIndex[1]._0 = 1; pIndex[1]._1 = 6; pIndex[1]._2 = 2;

	// -x 
	pIndex[2]._0 = 4; pIndex[2]._1 = 0; pIndex[2]._2 = 3;
	pIndex[3]._0 = 4; pIndex[3]._1 = 3; pIndex[3]._2 = 7;

	// +y
	pIndex[4]._0 = 4; pIndex[4]._1 = 5; pIndex[4]._2 = 1;
	pIndex[5]._0 = 4; pIndex[5]._1 = 1; pIndex[5]._2 = 0;

	// -y 
	pIndex[6]._0 = 3; pIndex[6]._1 = 2; pIndex[6]._2 = 6;
	pIndex[7]._0 = 3; pIndex[7]._1 = 6; pIndex[7]._2 = 7;

	// +z
	pIndex[8]._0 = 7; pIndex[8]._1 = 6; pIndex[8]._2 = 5;
	pIndex[9]._0 = 7; pIndex[9]._1 = 5; pIndex[9]._2 = 4;

	// -z 
	pIndex[10]._0 = 0; pIndex[10]._1 = 1; pIndex[10]._2 = 2;
	pIndex[11]._0 = 0; pIndex[11]._1 = 2; pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

void CSky_Cube_Texture::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CComponent * CSky_Cube_Texture::Clone(void)
{
	return new CSky_Cube_Texture(*this);
}

CSky_Cube_Texture * CSky_Cube_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSky_Cube_Texture *	pInstance = new CSky_Cube_Texture(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		MessageBox(0, L"CSky_Cube_Texture Created Failed", nullptr, MB_OK);
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CSky_Cube_Texture::Free(void)
{
	return Engine::CVIBuffer::Free();
}
