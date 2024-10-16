#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CPuzzleObject : public CGameObject
{
protected:
	explicit CPuzzleObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPuzzleObject();

public:
	virtual  HRESULT  Ready_GameObject();
	virtual  _int	  Update_GameObject(const _float& fTimeDelta);
	virtual  void	  LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void	  Render_GameObject();
	virtual  void	  Match_Puzzle() PURE;

protected:
	virtual  void Free();	
};

END