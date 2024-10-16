#pragma once
#include "Audience.h"
class CMoleSide : public CAudience
{
private:
	explicit CMoleSide(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CMoleSide();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	void	  Set_Dir() { m_pAnimatorCom->Play(L"MoleRight", true); }
	void	  Set_Pos(_vec3 _vPos) { m_pTransformCom->m_vInfo[INFO_POS] = _vPos; }

private:
	HRESULT    Add_Component();

public:
	static CMoleSide* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

