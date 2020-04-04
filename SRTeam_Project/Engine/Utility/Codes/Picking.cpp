#include "..\Headers\Picking.h"
#include "VIBuffer.h"

USING(Engine)


CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
	, m_vTempVec(nullptr)
{

}

CPicking::CPicking(const CPicking & rhs)
	:CComponent(rhs)
{

}

HRESULT CPicking::Ready_Picking(void)
{
	return NOERROR;
}

_vec3 * CPicking::Picking_ToBuffer(HWND hWnd, Engine::CVIBuffer * pTargetBuffer, const _matrix * pWorldMatrix)
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	POINT			ptMouse;

	//In.Screen Space
	GetCursorPos(&ptMouse);

	//In.Client Space(Viewport Space)
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vMousePos;

	//In.Projection Space
	//Compute BackBufferSize
	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.0f;

	//In.View Space
	_matrix			matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3			vRay, vPivot;
	vPivot = _vec3(0.0f, 0.0f, 0.0f);
	vRay = vMousePos - vPivot;

	//In.World Space
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vPivot, &vPivot, &matView);
	D3DXVec3TransformNormal(&vRay, &vRay, &matView);

	//In.Local Space
	if (nullptr != pWorldMatrix)
	{
		_matrix		matWorld;
		D3DXMatrixInverse(&matWorld, nullptr, pWorldMatrix);
		D3DXVec3TransformCoord(&vPivot, &vPivot, &matWorld);
		D3DXVec3TransformNormal(&vRay, &vRay, &matWorld);
	}

	const _vec3* pVertexPos = pTargetBuffer->Get_VertexPos();
	tuple<_uint, _uint, _float> Tuple = pTargetBuffer->Get_VertexCntXZItv();

	_float		fU = 0.f, fV = 0.f, fDist = 0.f;

	for (size_t i = 0; i < get<1>(Tuple) - 1; i++)
	{
		for (size_t j = 0; j < get<0>(Tuple) - 1; j++)
		{
			_uint		iIndex = i * get<0>(Tuple) + j;

			_uint		iLT, iRT, iLB, iRB;

			iLT = iIndex + get<0>(Tuple);
			iRT = iLT + 1;
			iLB = iIndex;
			iRB = iIndex + 1;

			if (TRUE == D3DXIntersectTri(&pVertexPos[iRT], &pVertexPos[iLT], &pVertexPos[iRB], &vPivot, &vRay
				, &fU, &fV, &fDist))
			{
				m_vTempVec = &(pVertexPos[iRT] + (pVertexPos[iLT] - pVertexPos[iRT]) * fU + (pVertexPos[iRB] - pVertexPos[iRT]) * fV);
				return m_vTempVec;
			}

			if (TRUE == D3DXIntersectTri(&pVertexPos[iLB], &pVertexPos[iRB], &pVertexPos[iLT], &vPivot, &vRay
				, &fU, &fV, &fDist))
			{
				m_vTempVec = &(pVertexPos[iLB] + (pVertexPos[iRB] - pVertexPos[iLB]) * fU + (pVertexPos[iLT] - pVertexPos[iLB]) * fV);
				return m_vTempVec;
			}
		}
	}

	return nullptr;
}

_vec3 * CPicking::Picking_ForCubeTool(HWND hWnd, Engine::CVIBuffer * pTargetBuffer, const _matrix * pWorldMatrix)
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	POINT			ptMouse;

	//In.Screen Space
	GetCursorPos(&ptMouse);

	//In.Client Space(Viewport Space)
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vMousePos;

	//In.Projection Space
	//Compute BackBufferSize
	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.0f;

	//In.View Space
	_matrix			matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3			vRay, vPivot;
	vPivot = _vec3(0.0f, 0.0f, 0.0f);
	vRay = vMousePos - vPivot;

	//In.World Space
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vPivot, &vPivot, &matView);
	D3DXVec3TransformNormal(&vRay, &vRay, &matView);

	//In.Local Space
	if (nullptr != pWorldMatrix)
	{
		_matrix		matWorld;
		D3DXMatrixInverse(&matWorld, nullptr, pWorldMatrix);
		D3DXVec3TransformCoord(&vPivot, &vPivot, &matWorld);
		D3DXVec3TransformNormal(&vRay, &vRay, &matWorld);
	}

	const _vec3* pVertexPos = pTargetBuffer->Get_VertexPos();
	tuple<_uint, _uint, _float> Tuple = pTargetBuffer->Get_VertexCntXZItv();

	_float		fU = 0.f, fV = 0.f, fDist = 0.f;

	for (size_t i = 0; i < get<1>(Tuple) - 1; i++)
	{
		for (size_t j = 0; j < get<0>(Tuple) - 1; j++)
		{
			_uint		iIndex = i * get<0>(Tuple) + j;

			_uint		iLT, iRT, iLB, iRB;

			iLT = iIndex + get<0>(Tuple);
			iRT = iLT + 1;

			iLB = iIndex;
			iRB = iIndex + 1;

			if (TRUE == D3DXIntersectTri(&pVertexPos[iRT], &pVertexPos[iLT], &pVertexPos[iRB], &vPivot, &vRay
				, &fU, &fV, &fDist))
			{
				m_vTempVec = &(pVertexPos[iLT] + (pVertexPos[iRT] - pVertexPos[iLT]) * 0.5f + (pVertexPos[iRB] - pVertexPos[iRT]) * 0.5f);
				return m_vTempVec;
			}

			if (TRUE == D3DXIntersectTri(&pVertexPos[iLB], &pVertexPos[iRB], &pVertexPos[iLT], &vPivot, &vRay
				, &fU, &fV, &fDist))
			{
				m_vTempVec = &(pVertexPos[iLB] + (pVertexPos[iRB] - pVertexPos[iLB]) * 0.5f + (pVertexPos[iLT] - pVertexPos[iLB]) * 0.5f);
				return m_vTempVec;
			}
		}
	}

	return nullptr;
}

CComponent * CPicking::Clone(void)
{
	return new CPicking(*this);
}

CPicking * CPicking::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPicking *		pInstance = new CPicking(pGraphicDev);

	if (FAILED(pInstance->Ready_Picking()))
	{
		MSG_BOX("CPicking Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CPicking::Free(void)
{
	return Engine::CComponent::Free();
}
