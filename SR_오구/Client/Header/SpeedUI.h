#pragma once
#include "BuffUI.h"

class CSpeedUI :  public CBuffUI
{
private:
	explicit CSpeedUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpeedUI();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   void      LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

private:
	HRESULT    Add_Component();

public:
	static CSpeedUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

