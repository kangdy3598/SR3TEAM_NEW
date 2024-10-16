#pragma once
#include "Item.h"
class CPenguin : public CItem
{
private:
	explicit CPenguin(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPenguin();

public:

	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

private:
	HRESULT    Add_Component();

public:
	static CPenguin* Create(LPDIRECT3DDEVICE9 pGraphicDev);

};

