#pragma once
#include "StoreNPC.h"
#include "Export_Utility.h"

class CHarborcat : public CStoreNPC
{
private:
	explicit CHarborcat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CHarborcat();

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
	static CHarborcat* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

