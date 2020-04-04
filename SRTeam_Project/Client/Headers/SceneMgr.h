#pragma once

#include "Base.h"
#include "Engine_Defines.h"
#include "Scene.h"

namespace Engine
{
	class CRenderer;
}


class CSceneMgr : public CBase
{
	DECLARE_SINGLETON(CSceneMgr)

public:
	explicit CSceneMgr();
	virtual ~CSceneMgr();

public:
	CScene* const GetCurrentScene(void)
	{
		return m_pCurrent_Scene;
	}

	CScene* const GetNewScene(void)
	{
		return m_pNew_Scene;
	}

public:
	HRESULT SetUp_CurrentScene(CScene* pNewScene);
	HRESULT SetUp_Renderer(CRenderer* pRenderer);
public:
	HRESULT Ready_SceneMgr(const _uint& iMaxSize);
	_int Update_SceneMgr(const _float& fTimeDelta);
	void Render_SceneMgr(void);

private:
	virtual _ulong Free(void) final;

private:
	CScene*						m_pCurrent_Scene = nullptr;
	CScene*						m_pNew_Scene = nullptr;
	CRenderer*					m_pRenderer = nullptr;

};

