#pragma once
#include "BuffUI.h"

class CPowerUI : public CBuffUI
{
private:
	explicit CPowerUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPowerUI();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   void      LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

private:
	HRESULT    Add_Component();

public:
	static CPowerUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

