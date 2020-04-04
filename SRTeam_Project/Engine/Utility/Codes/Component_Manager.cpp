#include "..\Headers\Component_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
	: m_pMapComponents(nullptr)
{
}

//Component�� �غ��Ѵ�. (Insert��)
HRESULT CComponent_Manager::Ready_Component(const _uint& iArrayIdx, const _tchar * pComponentTag, CComponent * pComponent)
{
	//���� ������ �Ѿ�ų� MapComponent�� ��������� �������´�.
	if (nullptr == m_pMapComponents
		|| iArrayIdx >= m_iContainerSize)
		return E_FAIL;

	//ComponentTag�� ã�Ƽ� ������ �������´�.
	CComponent* pComponent_Find = Find_Component(iArrayIdx, pComponentTag);
	if (nullptr != pComponent_Find)
		return E_FAIL;

	//ComponentTag�� ����� Tag�� �Բ� ����ִ´�.
	m_pMapComponents[iArrayIdx].insert(MAPCOMPONENTS::value_type(pComponentTag, pComponent));
	
	return NOERROR;
}

//Component�� ���� �Ѵ�.
CComponent* CComponent_Manager::Clone_Component(const _uint & iArrayIdx, const _tchar * pComponentTag)
{
	//Reserve������ ����� ��������
	if (iArrayIdx >= m_iContainerSize)
		return nullptr;

	//Find������ Component�� ������ return
	CComponent* pComponent = Find_Component(iArrayIdx, pComponentTag);
	if (nullptr == pComponent)
		return nullptr;

	//Clone��
	return pComponent->Clone();
}

HRESULT CComponent_Manager::Reserve_ComponentContainer(const _uint & iMaxSize)
{
	//�̹� MapComponent�� ������ �̹� Reserve�Ѱ��̹Ƿ� Return
	if (nullptr != m_pMapComponents)
	{
		MSG_BOX("Reserve_ResourceContainer Failed");
		return E_FAIL;
	}

	//iMaxSize ��ŭ Dynamic Array �� �Ѵ�.
	m_pMapComponents = new MAPCOMPONENTS[iMaxSize];


	m_iContainerSize = iMaxSize;

	return NOERROR;
}

//� Scene�� Component���� �����Ѵ�.
HRESULT CComponent_Manager::Release_Component(const _uint & iSceneIdx)
{
	if (iSceneIdx >= m_iContainerSize)
		return E_FAIL;

	_ulong		dwRefCnt = 0;

	//Scene�� Component���� Safe_Release ������� for�� ���
	for (auto& Pair : m_pMapComponents[iSceneIdx])
	{
		if (dwRefCnt = Safe_Release(Pair.second))
		{	
			//Safe_Release�� RefCnt�� 0�� �ƴҽ� ���� �޼���
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

//ComponentTag�� ���� Component�� ã�´�.
CComponent * CComponent_Manager::Find_Component(const _uint& iArrayIdx, const _tchar * pComponentTag)
{
	if (nullptr == m_pMapComponents
		|| iArrayIdx >= m_iContainerSize)
		return nullptr;

	//find_if Algorithm�� ����Ͽ� Scene�� �ִ� ������Ʈ Contianer�߿��� ã�� ������ ��
	auto iter = find_if(m_pMapComponents[iArrayIdx].begin(), m_pMapComponents[iArrayIdx].end(), CFind_Tag(pComponentTag));

	//Tag�� ������ return
	if(iter == m_pMapComponents[iArrayIdx].end())		
		return nullptr;

	return iter->second;
}

_ulong CComponent_Manager::Free(void)	//By.Jang
{
	//�� Release
	for (size_t i = 0; i < m_iContainerSize; i++)
	{
		for_each(m_pMapComponents[i].begin(), m_pMapComponents[i].end(), CRelease_Pair());
		m_pMapComponents[i].clear();
	}

	//Delete Dynamic Array
	Safe_Delete_Array(m_pMapComponents);
	return _ulong();
}
