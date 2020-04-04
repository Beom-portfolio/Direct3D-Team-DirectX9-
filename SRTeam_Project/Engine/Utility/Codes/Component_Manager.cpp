#include "..\Headers\Component_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
	: m_pMapComponents(nullptr)
{
}

//Component를 준비한다. (Insert함)
HRESULT CComponent_Manager::Ready_Component(const _uint& iArrayIdx, const _tchar * pComponentTag, CComponent * pComponent)
{
	//씬의 범위를 넘어가거나 MapComponent가 비어있으면 빠져나온다.
	if (nullptr == m_pMapComponents
		|| iArrayIdx >= m_iContainerSize)
		return E_FAIL;

	//ComponentTag로 찾아서 있으면 빠져나온다.
	CComponent* pComponent_Find = Find_Component(iArrayIdx, pComponentTag);
	if (nullptr != pComponent_Find)
		return E_FAIL;

	//ComponentTag의 기능을 Tag와 함께 집어넣는다.
	m_pMapComponents[iArrayIdx].insert(MAPCOMPONENTS::value_type(pComponentTag, pComponent));
	
	return NOERROR;
}

//Component를 복제 한다.
CComponent* CComponent_Manager::Clone_Component(const _uint & iArrayIdx, const _tchar * pComponentTag)
{
	//Reserve범위를 벗어나면 빠져나옴
	if (iArrayIdx >= m_iContainerSize)
		return nullptr;

	//Find했을시 Component가 없으면 return
	CComponent* pComponent = Find_Component(iArrayIdx, pComponentTag);
	if (nullptr == pComponent)
		return nullptr;

	//Clone함
	return pComponent->Clone();
}

HRESULT CComponent_Manager::Reserve_ComponentContainer(const _uint & iMaxSize)
{
	//이미 MapComponent가 있으면 이미 Reserve한것이므로 Return
	if (nullptr != m_pMapComponents)
	{
		MSG_BOX("Reserve_ResourceContainer Failed");
		return E_FAIL;
	}

	//iMaxSize 만큼 Dynamic Array 를 한다.
	m_pMapComponents = new MAPCOMPONENTS[iMaxSize];


	m_iContainerSize = iMaxSize;

	return NOERROR;
}

//어떤 Scene의 Component들을 해제한다.
HRESULT CComponent_Manager::Release_Component(const _uint & iSceneIdx)
{
	if (iSceneIdx >= m_iContainerSize)
		return E_FAIL;

	_ulong		dwRefCnt = 0;

	//Scene의 Component들을 Safe_Release 범위기반 for문 사용
	for (auto& Pair : m_pMapComponents[iSceneIdx])
	{
		if (dwRefCnt = Safe_Release(Pair.second))
		{	
			//Safe_Release시 RefCnt가 0이 아닐시 오류 메세지
			MSG_BOX("Scene`s Component Release Failed");
			return E_FAIL;
		}
	}
	m_pMapComponents[iSceneIdx].clear();

	return NOERROR;
}

HRESULT CComponent_Manager::Release_Component_Certain(const _uint & iSceneIdx, const _tchar * pComponentTag)
{
	if (iSceneIdx >= m_iContainerSize)
		return E_FAIL;

	_ulong		dwRefCnt = 0;

	
	if(dwRefCnt = Engine::Safe_Release(m_pMapComponents[iSceneIdx].find(pComponentTag)->second))
	{
		MSG_BOX("Scene`s Certain Component Release Failed");
		return E_FAIL;
	}

	m_pMapComponents[iSceneIdx].erase(pComponentTag);

	return NOERROR;
}

//ComponentTag를 통해 Component를 찾는다.
CComponent * CComponent_Manager::Find_Component(const _uint& iArrayIdx, const _tchar * pComponentTag)
{
	if (nullptr == m_pMapComponents
		|| iArrayIdx >= m_iContainerSize)
		return nullptr;

	//find_if Algorithm을 사용하여 Scene에 있는 컴포넌트 Contianer중에서 찾는 역할을 함
	auto iter = find_if(m_pMapComponents[iArrayIdx].begin(), m_pMapComponents[iArrayIdx].end(), CFind_Tag(pComponentTag));

	//Tag가 없으면 return
	if(iter == m_pMapComponents[iArrayIdx].end())		
		return nullptr;

	return iter->second;
}

_ulong CComponent_Manager::Free(void)	//By.Jang
{
	//다 Release
	for (size_t i = 0; i < m_iContainerSize; i++)
	{
		for_each(m_pMapComponents[i].begin(), m_pMapComponents[i].end(), CRelease_Pair());
		m_pMapComponents[i].clear();
	}

	//Delete Dynamic Array
	Safe_Delete_Array(m_pMapComponents);
	return _ulong();
}
