#pragma once
#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CComponent* Get_Component(COMPONENTID eID, const _tchar* pLayeTag, const _tchar* pObjTag, const _tchar* pComponentTag);
	CGameObject* Get_GameObject(const _tchar* pLayeTag, const _tchar* pObjTag);
	CLayer* Get_Layer(const _tchar* pLayerTag);

public:
	virtual  HRESULT Ready_Scene();
	virtual  void    LateReady_Scene();
	virtual  _int    Update_Scene(const _float& fTimeDelta);
	virtual  void    LateUpdate_Scene(const _float& fTimeDelta);
	virtual  void    Render_Scene()PURE;

	//0922 Seonhwan
	virtual  void    init() { ; }
	virtual  void    DontDestroy_Layer(const _tchar* layerName,
		map<const _tchar*, CGameObject*> _mapDontDestroy);

protected:
	map<const _tchar*, CLayer*>  m_mapLayer;
	LPDIRECT3DDEVICE9			 m_pGraphicDev; 
	HWND						m_hVideoHandle;
	_bool	m_bVideoPlaying;		

public:
	virtual  void  Free();


	//9월 26일 작업
public:
	map<const _tchar*, CLayer*>& GetLayerMapPtr() { return m_mapLayer; }
	const vector<CGameObject*>&  GetVecObjlist(GROUP_TYPE _eType) { return m_vecArrObj[(UINT)_eType]; };
	HRESULT						 Add_ObjectGroup(GROUP_TYPE _eType, CGameObject* pGameObject);

private:
	vector<CGameObject*>          m_vecArrObj[GROUP_TYPE::GROUP_END];


public:
	void  PlayVideo(HWND _hWnd, const wstring& _strFilePath);
};

END