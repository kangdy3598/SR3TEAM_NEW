#pragma once
#include "Audience.h"


class CRabbitLeft : public CAudience
{
private:
	explicit CRabbitLeft(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CRabbitLeft();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	void	  Set_Dir() {m_pAnimatorCom->Play(L"CheerRabbitRight", true);	}
	void	  Set_Pos(_vec3 _vPos) { m_pTransformCom->m_vInfo[INFO_POS] = _vPos; }

private:
	HRESULT    Add_Component();

public:
	static CRabbitLeft* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

