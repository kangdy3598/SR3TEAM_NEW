#pragma once
#include "GameObject.h"
#include "Export_Utility.h"

//0913
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CDefaultUI : public Engine::CGameObject
{
private:
	explicit CDefaultUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDefaultUI();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();


private:
	HRESULT    Add_Component();

private:
	enum DEFAULT_UI { HPFRAME, HPBAR, HPBACK, COIN_UI, UI_END };
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[UI_END];
	Engine::CTransform* m_pTransformCom[UI_END];


private:
	CPlayer* m_pPlayer;

public:
	static CDefaultUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

