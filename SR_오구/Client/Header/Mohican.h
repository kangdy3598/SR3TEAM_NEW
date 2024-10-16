#pragma once
#include "Item.h"

class CMohican : public CItem
{
private:
	explicit CMohican(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMohican();

public:

	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

private:
	HRESULT    Add_Component();

public:
	static CMohican* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

