#pragma once

#include "VIBuffer.h"

BEGIN(Engine)



class DLL_EXPORT CRectUI_Texture final : public CVIBuffer
{
private:
	explicit CRectUI_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRectUI_Texture(const CRectUI_Texture& rhs);
	virtual ~CRectUI_Texture() = default;

public:
	HRESULT Ready_Buffer();
	void Render_Buffer(void);
	void SetUp_Buffer(const _vec3* vUIPos, const _float& fWidth, const _float& fHeight);

public:
	virtual CComponent* Clone(void) final;

private:
	_vec3		m_vUIPos;
	_float		m_fWidth;
	_float		m_fHeight;


public:
	static CRectUI_Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual _ulong Free(void) final;

	

};

END

