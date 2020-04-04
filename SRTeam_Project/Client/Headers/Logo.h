#pragma once
#include "Scene.h"

#include "Defines.h"
#include "Engine_Defines.h"

namespace Engine
{
	class CComponent_Manager;
	class CGameObject;
}

class CLogo : public CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo() = default;


public:
	virtual HRESULT Ready_Scene(void) override;
	virtual int		Update_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene(void) override;

private:
	HRESULT Ready_LogoComponent(void);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp);
	HRESULT Ready_Layer_Logo(const _tchar* pLayerTag);

private:
	HRESULT ChangeScene(SCENETYPE eSceneID);

private:
	Engine::CGameObject*		m_pLogoOjbect = nullptr;

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual _ulong Free(void) final;

};

