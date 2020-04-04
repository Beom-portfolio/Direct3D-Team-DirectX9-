#pragma once

#include "Engine_Defines.h"
#include "Base.h"

#include "Triangle_Color.h"
#include "Rect_Color.h"
#include "Rect_Texture.h"
#include "Terrain_Buffer.h"
#include "Renderer.h"
#include "Texture.h"
#include "Transform.h"
#include "Rect_Cube.h"
#include "Material.h"
#include "Picking.h"
#include "Sky_Cube_Texture.h"
#include "RectUI_Texture.h"
//#include "Cube_Texture.h"


BEGIN(Engine)

// �뵵 : ��ü���� ������ �Ǵ� ����Կ� �־� �ʿ��� ��ǰ��ü���� ��Ƽ� �����Ѵ�.


class DLL_EXPORT CComponent_Manager : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	explicit CComponent_Manager();
	virtual ~CComponent_Manager() = default;
public:
	HRESULT Ready_Component(const _uint& iArrayIdx, const _tchar* pComponentTag, CComponent* pComponent);
	CComponent* Clone_Component(const _uint& iArrayIdx, const _tchar* pComponentTag);
public:
	HRESULT Reserve_ComponentContainer(const _uint& iMaxSize);
	HRESULT Release_Component(const _uint& iSceneIdx);
	HRESULT Release_Component_Certain(const _uint& iSceneIdx, const _tchar* pComponentTag);
private:
	map<const _tchar*, CComponent*>*			m_pMapComponents = nullptr;
	typedef map<const _tchar*, CComponent*>		MAPCOMPONENTS;
private:
	_uint				m_iContainerSize = 0;
private:
	CComponent* Find_Component(const _uint& iArrayIdx, const _tchar* pComponentTag);
private:
	virtual _ulong Free(void) final;


};

END