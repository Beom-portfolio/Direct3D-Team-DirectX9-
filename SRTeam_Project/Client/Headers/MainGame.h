#pragma once

#include "Defines.h"
#include "Engine_Defines.h"
#include "Base.h"
#include "Graphic_Device.h"

class CSceneMgr;
namespace Engine
{
	class CComponent_Manager;
	class CGraphic_Device;
}

class CMainGame : public CBase
{
private:
	explicit CMainGame(void);
	virtual ~CMainGame(void);

public:
	HRESULT Ready_MainGame(void); 
	int Update_MainGame(const float& fTimeDelta);
	void Render_MainGame(void);

private:
	HRESULT Ready_Default(HWND hWnd, Engine::CGraphic_Device::WINMODE eMode, const _uint& iBackCX, const _uint& iBackCY);
	HRESULT Ready_StaticComponent(void);
	HRESULT Ready_StartScene(void);

public:
	static CMainGame* Create(void);
public:
	virtual unsigned long Free(void) final;

private:
	CGraphic_Device*				m_pGraphic_Device = nullptr;
	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;

	CComponent_Manager*				m_pComponent_Manager = nullptr;
	CSceneMgr*						m_pSceneMgr = nullptr;
};

