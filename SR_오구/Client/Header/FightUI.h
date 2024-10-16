#pragma once
#include "GameObject.h"
#include "Export_Utility.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
class CFightUI : public Engine::CGameObject
{
private:
	explicit CFightUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFightUI();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void	  CallFight() 
	{ 
		m_bCall = true; 
		m_fElapsedTime = 0;
		Engine::Play_Sound(L"SFX_502_ArenaIntro.wav", SOUND_EFFECT, 1.f);
	}

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;


	_bool	m_bCall;
	_float	m_fElapsedTime;

public:
	static CFightUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

