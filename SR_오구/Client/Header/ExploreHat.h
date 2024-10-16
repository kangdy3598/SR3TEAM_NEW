#pragma once
#include "EquipHat.h"
#include "Item.h"

class CExploreHat : public CItem
{
private:
	explicit CExploreHat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CExploreHat();

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
	static CExploreHat* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

