#include "StdAfx.h"
#include "SoundMgr.h"

CSoundMgr* CSoundMgr::m_pInstance = NULL;

CSoundMgr::CSoundMgr(void)
{
	Initialize();
}

CSoundMgr::~CSoundMgr(void)
{
	Release();
}

void CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, Channel_End, FMOD_INIT_NORMAL, NULL);
}

void CSoundMgr::LoadSoundFile()
{
	_finddata_t fd;	

	int iResult = 0;

	long handle = _findfirst("../Sound/*.*", &fd);

	if(-1 == handle)
	{
		MessageBox(g_hWnd, L"Not found file!!", L"Not found file!!", MB_OK);
		return;
	}

	while(-1 != iResult)
	{
		char szFullPath[256] = "";
		strcpy_s(szFullPath, "../Sound/");
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound;
		FMOD_RESULT FRes = FMOD_System_CreateSound(m_pSystem, szFullPath
			, FMOD_HARDWARE, NULL, &pSound);

		if(FMOD_OK == FRes)
		{
			TCHAR* pSoundKey = new TCHAR[256];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * 256);

			MultiByteToWideChar(CP_ACP, 0, fd.name, strlen(fd.name) + 1, 
				pSoundKey, 256);

			m_MapSound.insert(make_pair(pSoundKey, pSound));
		}

		iResult = _findnext(handle, &fd);
	}

	_findclose(handle);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlaySound(TCHAR* pSoundKey, ChannelID eID)
{
	map<const TCHAR*, FMOD_SOUND*>::iterator iter = find_if(
		m_MapSound.begin(), m_MapSound.end(), STRING_COMPARE(pSoundKey));

	if(iter == m_MapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, 0, &(m_pChannelArr[eID]));
}

void CSoundMgr::PlayBGM(TCHAR* pSoundKey, ChannelID eID)
{
	map<const TCHAR*, FMOD_SOUND*>::iterator iter = find_if(
		m_MapSound.begin(), m_MapSound.end(), STRING_COMPARE(pSoundKey));

	if(iter == m_MapSound.end())
		return;

	FMOD_Sound_SetMode(iter->second, FMOD_LOOP_NORMAL);
	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, 0, &(m_pChannelArr[eID]));
}

void CSoundMgr::UpdateSound()
{
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound(ChannelID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopAll()
{
	for(int i = 0; i < Channel_End; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::Release()
{
	map<const TCHAR*, FMOD_SOUND*>::iterator iter_begin = m_MapSound.begin();
	map<const TCHAR*, FMOD_SOUND*>::iterator iter_end = m_MapSound.end();

	for(; iter_begin != iter_end; ++iter_begin)
	{
		delete[] iter_begin->first;
		FMOD_Sound_Release(iter_begin->second);
	}

	m_MapSound.clear();

	FMOD_System_Close(m_pSystem);
	FMOD_System_Release(m_pSystem);
}