#pragma once

#include "Engine_Defines.h"
#include "Base.h"

#include "Object_Manager.h"

BEGIN(Engine)


class DLL_EXPORT CScene_Tool abstract : public CBase
{	
protected:
	explicit CScene_Tool(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_Tool() = default;
public:
	Engine::CObject_Manager*		GetObjectMgr(void)
	{
		return	m_pObject_Manager;
	}
public:
	virtual HRESULT Ready_Scene(void);
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual void Render_Scene(void);
public:
	HRESULT Release_Object(const _uint& iSceneIdx);
protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;
protected:
	Engine::CObject_Manager*		m_pObject_Manager = nullptr;
protected:
	virtual _ulong Free(void) override;
};

END