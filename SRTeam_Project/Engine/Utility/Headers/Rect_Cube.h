#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class DLL_EXPORT CRect_Cube final : public CVIBuffer
{
private:
	explicit CRect_Cube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRect_Cube(const CRect_Cube& rhs);
	virtual ~CRect_Cube();
public:
	HRESULT Ready_Buffer(void);
	void Render_Buffer(void);
public:
	virtual CComponent* Clone(void) final;
public:
	static CRect_Cube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual _ulong Free(void) final;
};

END