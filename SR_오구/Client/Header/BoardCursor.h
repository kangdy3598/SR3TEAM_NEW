#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CBoardCursor :public Engine::CGameObject
{
private:
	explicit CBoardCursor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoardCursor();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	bool Is_Active() { return m_bIsActive; }
	void Set_Active(bool _bIsActive) { m_bIsActive = _bIsActive; }
	void Key_Input(const _float& fTimeDelta);

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	_int m_iTileId;
	_bool m_bIsActive;
	_bool m_bCheck = false;

public:
	static CBoardCursor* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};