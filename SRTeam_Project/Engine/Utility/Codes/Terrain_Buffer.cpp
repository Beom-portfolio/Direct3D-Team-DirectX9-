#include "..\Headers\Terrain_Buffer.h"

USING(Engine)

CTerrain_Buffer::CTerrain_Buffer(LPDIRECT3DDEVICE9	pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

CTerrain_Buffer::CTerrain_Buffer(const CTerrain_Buffer & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CTerrain_Buffer::Ready_Buffer(const _uint & iVtxCntX, const _uint & iVtxCntZ, const _float & fInterval)
{
	m_iVertexSize = sizeof(VTXTEX);
	m_iVertexCnt = iVtxCntX * iVtxCntZ;
	m_iVertexCntX = iVtxCntX;
	m_iVertexCntZ = iVtxCntZ;
	m_fVertexItv = fInterval;
	m_pVertexPos = new _vec3[m_iVertexCnt];
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;
	m_iTriCnt = (iVtxCntX - 1) * (iVtxCntZ - 1) * 2;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (size_t i = 0; i < iVtxCntZ; i++)
	{
		for (size_t j = 0; j < iVtxCntX; j++)
		{
			_uint		iIndex = i * iVtxCntX + j;

			pVertex[iIndex].vPosition = _vec3(j * fInterval, 0.f, i * fInterval);
			m_pVertexPos[iIndex] = pVertex[iIndex].vPosition;
			pVertex[iIndex].vTexUV = _vec2(j / _float(iVtxCntX - 1) * 129.f, i / _float(iVtxCntZ - 1) * 129.f);
		}
	}

	m_pVB->Unlock();

	INDEX16*			pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	
	_uint			iIndex = 0;

	for (size_t i = 0; i < iVtxCntZ - 1; ++i)
	{
		for (size_t j = 0; j < iVtxCntX - 1; ++j)
		{	
			_uint		iVtxIdx = i * iVtxCntX + j;
			_uint		iLT, iRT, iLB, iRB;

			// 현재 반복문에서 기준이되는 정점인덱스를 바탕으로 사각형 네꼭지점의 인덱스르 ㄹ구한다.
			iLT = iVtxIdx + iVtxCntX;
			iRT = iLT + 1;
			iLB = iVtxIdx;
			iRB = iLB + 1;

			// 오른쪽 위
			pIndex[iIndex]._0 = iLT;
			pIndex[iIndex]._1 = iRT;
			pIndex[iIndex]._2 = iRB;
			++iIndex;

			pIndex[iIndex]._0 = iLT;
			pIndex[iIndex]._1 = iRB;
			pIndex[iIndex]._2 = iLB;
			++iIndex;
		}
	}


	m_pIB->Unlock();

	return NOERROR;
}

void CTerrain_Buffer::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CComponent * CTerrain_Buffer::Clone(void)
{
	return new CTerrain_Buffer(*this);
}

CTerrain_Buffer * CTerrain_Buffer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint & iVtxCntX, const _uint & iVtxCntZ, const _float & fInterval)
{
	CTerrain_Buffer *	pInstance = new CTerrain_Buffer(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(iVtxCntX, iVtxCntZ, fInterval)))
	{
		MessageBox(0, L"CTerrain_Buffer Created Failed", nullptr, MB_OK);
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CTerrain_Buffer::Free(void)
{
	return Engine::CVIBuffer::Free();
}
