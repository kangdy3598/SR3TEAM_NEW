#pragma once
#include "Base.h"
#include "Scene.h"


BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit  CManagement();
	virtual  ~CManagement();

public:
	CComponent* Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag);
	CGameObject* Get_GameObject(const _tchar* pLayeTag, const _tchar* pObjTag);
	CLayer* Get_Layer(const _tchar* pLayerTag);

public:
	HRESULT   Set_Scene(CScene* pScene);

	//9월 11일 추가한 코드 
	CScene* Get_Scene() { return m_pScene; }

	void      LateReady_Scene();
	_int      Update_Scene(const _float& fTimeDelta);
	void      LateUpdate_Scene(const _float& fTimeDelta);
	void	  Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CScene*	  m_pScene; 


public:
	virtual void	Free();


//8월 26일 추가한 코드 
public: 
	CScene* GetCurScenePtr() { return m_pScene; }

public:
	int      m_imap_stage;
};

END