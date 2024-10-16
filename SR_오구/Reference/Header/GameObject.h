#pragma once

#include "Base.h"
#include "Component.h"
//#include "Layer.h"
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();


public:
	CComponent* Get_Component(COMPONENTID eID, const _tchar* pComponentTag);

	//0913
	_float				Get_ViewZ() const { return m_fViewZ; }

public:
	virtual  HRESULT  Ready_GameObject();
	virtual  void	  LateReady_GameObject();
	virtual  _int	  Update_GameObject(const _float& fTimeDelta);
	virtual  void     LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void     Render_GameObject() PURE;

	//0913
	void			Compute_ViewZ(const _vec3* pPos);
	virtual	 _bool    GetTransparent() { return m_bIsTransparent; }
	virtual  void	  SetTransparent(_bool _bValue) { m_bIsTransparent = _bValue; }
	virtual  void	  SetDontDestroy(_bool _bValue) { m_bDontDestroy = _bValue; }
	virtual  _bool	  GetDontDestroy() { return m_bDontDestroy; }

protected:
	map<const _tchar*, CComponent*>    m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9				   m_pGraphicDev;

	//0913
	_float								m_fViewZ;
	int									m_iObjType;
	_bool								m_bIsTransparent;
public:
	CComponent* Find_Component(COMPONENTID eID, const _tchar* pComponentTag);

public:
	virtual void  Free();


	//==============================================================================
	//0915 Seonhwan
protected:
	wstring m_strKey; // 게임오브젝트가 자신이 어떤 명칭으로 저장되있는지 알기 위해서
	TAG m_eTag;
	_bool m_bIsActive;
	//0920 Seonhwan
	wstring m_strTextureKey; // 게임오브젝트의 텍스처 키 
	_bool m_bDontDestroy;

	//0915 Seonhwan
public:
	void SetObjectKey(wstring _strKey) { m_strKey = _strKey; }
	wstring GetObjectKey() { return m_strKey; }
	TAG Get_Tag() { return m_eTag; }
	virtual void Set_Active(_bool _bActive) { m_bIsActive = _bActive; }
	_bool Is_Active() { return m_bIsActive; }
	//0920 Seonhwan

	void SetTextureKey(wstring _strKey) { m_strTextureKey = _strKey; }
	wstring GetTextureKey() { return m_strTextureKey; }

	void SetObjectType(int _iObjType) { m_iObjType = _iObjType; }
	int GetObjectType() { return m_iObjType; }
	_bool IncludingType(OBJ_TYPE _eType) { return m_iObjType & _eType; }

public:
	virtual void OnCollision(CGameObject* _pOther)
	{
		
	}
	virtual void OnCollisionEnter(CGameObject* _pOther)
	{
		
	}
	virtual void OnCollisionExit(CGameObject* _pOther)
	{ 
		
	}
};

END
