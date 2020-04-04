#include "..\Headers\Layer.h"
#include "GameObject.h"

USING(Engine)

CLayer::CLayer()
{
}

Engine::CComponent * CLayer::Get_Component(const _uint & iIndex, const _tchar * pComTag)
{
	_uint iSize = m_ObjectList.size();

	if (iIndex >= iSize)
		return nullptr;

	auto iter = m_ObjectList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	if (iter == m_ObjectList.end())
		return nullptr;


	return (*iter)->Get_Component(pComTag);
}

Engine::CGameObject * CLayer::Get_GameObject(const _uint & iIndex)
{
	_uint iSize = m_ObjectList.size();

	if (iIndex >= iSize)
		return nullptr;

	auto iter = m_ObjectList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	if (iter == m_ObjectList.end())
		return nullptr;


	return (*iter)->Get_GameObject();
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_ObjectList.push_back(pGameObject);

	return NOERROR;
}

HRESULT CLayer::Ready_Layer(void)
{
	return NOERROR;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	_int		iExitCode = 0;


	for ( auto& pGameObject : m_ObjectList )
	{
		iExitCode = pGameObject->Update_GameObject(fTimeDelta);

		if (2 == iExitCode)
			break;

		if (iExitCode & 0x80000000)
			return iExitCode;
	}
	return iExitCode;
}

_int CLayer::LastUpdate_Layer(const _float & fTimeDelta)
{
	_int		iExitCode = 0;

	list<CGameObject*>::iterator iter_begin = m_ObjectList.begin();
	list<CGameObject*>::iterator iter_end = m_ObjectList.end();

	for (; iter_begin != iter_end; )
	{
		iExitCode = (*iter_begin)->LastUpdate_GameObject(fTimeDelta);

		if (1 == iExitCode)
		{
			int iCount = Safe_Release((*iter_begin));
			iter_begin = m_ObjectList.erase(iter_begin);
		}
		else if (2 == iExitCode)
			return iExitCode;
		else
			++iter_begin;

		if(iExitCode & 0x80000000)
			return iExitCode;
		

	}
	return iExitCode;
}

void CLayer::Render_Layer(void)
{
	for (auto& pGameObject : m_ObjectList)	
		pGameObject->Render_GameObject();	
}

CLayer * CLayer::Create(void)
{
	CLayer *	pInstance = new CLayer();

	if (FAILED(pInstance->Ready_Layer()))
	{
		MessageBox(0, L"CLayer Created Failed", nullptr, MB_OK);
		Engine::Safe_Release(pInstance);
	}
	
	return pInstance;
}


_ulong CLayer::Free(void)	//By.Jang
{
	
	_ulong dwRefCnt = 0;

 	for_each(m_ObjectList.begin(), m_ObjectList.end(), CRelease_Single());
	m_ObjectList.clear();

	//for (auto& iter : m_ObjectList)
	//{
	//	Safe_Release((*iter));
	//}
	//m_ObjectList.clear();


	return dwRefCnt;
}


