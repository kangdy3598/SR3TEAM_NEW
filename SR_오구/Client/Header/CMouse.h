#pragma once

#include "TextBox.h"

class CMouse : public CQuestNPC
{
private:
	explicit CMouse(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CMouse();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual   void		OnCollision(CGameObject* _pOther);
	virtual   void		OnCollisionEnter(CGameObject* _pOther);
	virtual   void		OnCollisionExit(CGameObject* _pOther);

private:
	HRESULT    Add_Component();

public:
	static CMouse* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static _bool   g_bQuestClear;
private:
	virtual void Free();

};

