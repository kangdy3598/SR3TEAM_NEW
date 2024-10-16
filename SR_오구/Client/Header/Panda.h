#pragma once
#include "Export_Utility.h"
#include "StoreNPC.h"
class CPanda : public CStoreNPC
{
private:
	explicit CPanda(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CPanda();

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
	static CPanda* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

