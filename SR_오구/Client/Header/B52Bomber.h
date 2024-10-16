#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CB52Bomber :public Engine::CGameObject
{
private:
	explicit CB52Bomber(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CB52Bomber();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual void LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	void	Set_Player(CGameObject* _Player) { m_CPlayer = _Player; }

private:
	HRESULT Add_Component();

public:
	static CB52Bomber* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

public:
	void Key_Input(const _float& fTimeDelta);	
	void Pre_CreateShell();
	void Create_Shell();
	void Shot();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	vector<CGameObject*> m_vecWhitePhosphorus;
	_float m_fFireRate;
	_float m_fTime;
	_bool m_bIsCalled;
	_int m_iFirePosX;
	_int m_iFireCount;

protected:
	CGameObject* m_CPlayer;

private:

	virtual void Free();
};