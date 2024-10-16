#pragma once
#include "GameObject.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CBuffUI : public Engine::CGameObject
{
protected:
	explicit CBuffUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBuffUI();

public:
	_float	  Get_BuffTime() { return m_fBuffTime; }
	void	  Set_BuffTime(_int _SetTime) { m_fBuffTime += _SetTime; }
	void	  Set_YPos(_float _YPos) { m_pTransformCom->m_vInfo[INFO_POS].y = _YPos; }
	void	  Update_SettingTime(const _float& _fTimeDelta);

	virtual   HRESULT   Ready_GameObject();
	virtual   void      LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	_float		m_fBuffTime;

protected:
	virtual void Free();

};

