#pragma once
#include "Base.h"
#include "GameObject.h"


BEGIN(Engine)

class ENGINE_DLL CLayer: public CBase
{
private:
	explicit  CLayer();
	virtual  ~CLayer();

public:
	CComponent* Get_Component(COMPONENTID eID, const _tchar* pObjTag, const _tchar* pComponentTag);

public:
	HRESULT     Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);
	//9월 5일 이후 추가한 코드 
	CGameObject*     Get_GameObject(const _tchar* pLayeTag, const _tchar* pObjTag);

public:
	HRESULT		Ready_Layer();
	void		LateReady_Layer();
	_int		Update_Layer(const _float& fTimeDelta);
	void		LateUpdate_Layer(const _float& fTimeDelta);
	void		Render_Layer();

	GAME_STATE	GetGameState() { return m_eGameState; }
	void		SetGameState(GAME_STATE _eGamestate) { m_eGameState = _eGamestate; }
private:
	map<const _tchar*, CGameObject*>  m_mapObejct;

public:
	GAME_STATE m_eGameState;
public:
	static CLayer* Create();

private:
	virtual void   Free();

public:
	map<const _tchar*, CGameObject*>& GetLayerGameObjectPtr() { return m_mapObejct; }


};



END
