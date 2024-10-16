#pragma once
#include "GameObject.h"
#include "Export_System.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CTriCol;
class CTransform;
class CAnimator2;
class CAnimation2;

END

class CAudience : public Engine::CGameObject
{
protected:
	explicit CAudience(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAudience();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

protected:
	HRESULT    Add_Component();

protected:
	Engine::CAnimator2* m_pAnimatorCom;
	Engine::CTransform* m_pTransformCom;

protected:
	virtual void Free();
	IDirect3DTexture9* m_pTex;

};

