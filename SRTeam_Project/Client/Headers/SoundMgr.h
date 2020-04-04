#pragma once

#include "Defines.h"

class CSoundMgr
{
public:                           
	static CSoundMgr* GetInstance()
{                          
if (NULL == m_pInstance)       
m_pInstance = new CSoundMgr;
return m_pInstance;           
}                          
void DestroyInstance()           
{                          
if (m_pInstance)              
{                       
delete m_pInstance;        
m_pInstance = NULL;        
}                       
}                          
private:                        
	static CSoundMgr* m_pInstance;

public:
	enum ChannelID { Channel_Menu, Channel_Player, Channel_Monster, Channel_BGM, Channel_Eff, Channel_End };

public:
	CSoundMgr(void);
	~CSoundMgr(void);

public:
	void Initialize();
	void LoadSoundFile();
	void PlaySound(TCHAR* pSoundKey, ChannelID eID);
	void PlayBGM(TCHAR* pSoundKey, ChannelID eID);
	void UpdateSound();
	void StopSound(ChannelID eID);
	void StopAll();
	void Release();

private:
	FMOD_SYSTEM*	m_pSystem;
	FMOD_CHANNEL*	m_pChannelArr[Channel_End]; // 채널은 중첩되지 않게 준비.

	map<const TCHAR*, FMOD_SOUND*>	m_MapSound;
};
