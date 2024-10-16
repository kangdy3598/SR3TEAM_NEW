#pragma once
#include "GameObject.h"
#include "Player.h"
#include "PlayerInteractionBox.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CSwingEffect : public Engine::CGameObject
{
protected:
	explicit CSwingEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSwingEffect();

public:

	virtual	HRESULT	Ready_GameObject();
	virtual	void	LateReady_GameObject();
	virtual	_int	Update_GameObject(const _float& fTimeDelta);
	virtual	void	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual	void	Render_GameObject();


public:
	virtual void	SetPlayer(CPlayer* _Player) { m_CPlayer = _Player; }
	virtual void	GetLayer(CLayer* _layer) { m_pLayer = _layer; }

protected:
	virtual HRESULT	Add_Component();

private:

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CAnimator2* m_pAnimatorCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CLayer* m_pLayer;

	CPlayer* m_CPlayer;
	bool m_activation;

public:
	static CSwingEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

