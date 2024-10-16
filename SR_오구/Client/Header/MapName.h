#pragma once
#include "Export_Utility.h"
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
class CMapName : public Engine::CGameObject
{
private:
	explicit CMapName(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapName();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void	  CallName() { m_bCall = true; m_fElapsedTime = 0; }
	void	  Set_Time(_float _iTime) { m_SettingTime = _iTime; }
	void	  Set_MapName(const _tchar* _Name) { m_pName = _Name; }
	void	  Hide_MapName();
private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	const _tchar* m_pName;
	_bool	m_bCall;
	_float	m_fElapsedTime;
	_float  m_SettingTime;
public:
	static CMapName* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

