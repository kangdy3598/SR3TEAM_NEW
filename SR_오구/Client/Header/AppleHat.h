#pragma once
#include "EquipHat.h"
#include "Item.h"
class CAppleHat : public CItem
{
private:
	explicit CAppleHat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAppleHat();

public:

	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual	   void	    Use_Item();

private:
	CEquipHat* m_pEquipHat;

private:
	HRESULT    Add_Component();

public:
	static CAppleHat* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

