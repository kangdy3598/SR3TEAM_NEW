#pragma once
#include "EquipHat.h"
#include "Item.h"
class CPoopHat : public CItem
{
private:
	explicit CPoopHat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPoopHat();

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
	static CPoopHat* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

