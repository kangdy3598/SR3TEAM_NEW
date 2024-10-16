#include "Scene.h"
#include "CollisionMgr.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
	,m_bVideoPlaying(false)	
	,m_hVideoHandle(nullptr)
{
	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CComponent* CScene::Get_Component(COMPONENTID eID, const _tchar* pLayeTag, const _tchar* pObjTag, const _tchar* pComponentTag)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayeTag));

	if (iter == m_mapLayer.end())
		return nullptr; 


	return iter->second->Get_Component(eID,pObjTag,pComponentTag);
}

CGameObject* CScene::Get_GameObject(const _tchar* pLayeTag, const _tchar* pObjTag)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayeTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(pLayeTag, pObjTag);
}

CLayer* CScene::Get_Layer(const _tchar* pLayerTag)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second;
}

HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

void CScene::LateReady_Scene()
{
	for (auto& pLayer : m_mapLayer)
	{
		pLayer.second->LateReady_Layer();
	}
}

_int CScene::Update_Scene(const _float& fTimeDelta)
{
	_int  iResult(0);

	for (auto& pLayer : m_mapLayer)
	{
		iResult = pLayer.second->Update_Layer(fTimeDelta);


		if (iResult & 0x80000000)
			return iResult; 
	}

	CCollisionMgr::GetInstance()->update();

	return iResult;
}

void CScene::LateUpdate_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_mapLayer)
	{
		pLayer.second->LateUpdate_Layer(fTimeDelta);
	}
}

void CScene::DontDestroy_Layer(const _tchar* layerName, map<const _tchar*, CGameObject*> _mapDontDestroy)
{
	for (auto& iter : _mapDontDestroy)
	{
		Get_Layer(layerName)->Add_GameObject(iter.first, iter.second);
	}
}

//void CScene::Render_Scene()
//{
//	for (auto& pLayer : m_mapLayer)
//	{
//		pLayer.second->Render_Layer();
//	}
//}

void CScene::Free()
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}

HRESULT CScene::Add_ObjectGroup(GROUP_TYPE _eType, CGameObject* pGameObject)
{
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	m_vecArrObj[(UINT)_eType].push_back(pGameObject);

	return S_OK;
}

void CScene::PlayVideo(HWND _hWnd, const wstring& _strFilePath)
{
	if (m_bVideoPlaying)
		return;

	m_hVideoHandle = MCIWndCreate(_hWnd,	
		NULL,
		WS_CHILD |	
		WS_VISIBLE |	
		MCIWNDF_NOPLAYBAR, _strFilePath.c_str());	

	if (m_hVideoHandle == NULL)		
	{
		MessageBox(_hWnd, L"동영상 핸들을 생성하지 못했습니다.", L"Error", MB_OK);	
		return;
	}	

	MoveWindow(m_hVideoHandle, 0, 0, WINCX, 720, FALSE);	

	m_bVideoPlaying = true;		
	MCIWndPlay(m_hVideoHandle);		
	HDC dc = GetDC(_hWnd);		
	HDC memDC = CreateCompatibleDC(dc);
	HBITMAP hBitmap = CreateCompatibleBitmap(dc, WINCX, WINCY);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

	Rectangle(dc, 0, 0, WINCX, WINCY);
	BitBlt(dc, 0, 0, WINCX, WINCY, memDC, 0, 0, SRCCOPY);	
	while (MCIWndGetLength(m_hVideoHandle) > MCIWndGetPosition(m_hVideoHandle))	
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			MCIWndClose(m_hVideoHandle);
			m_bVideoPlaying = false;
			break;
		}

	}

	SelectObject(memDC, hOldBitmap);
	ReleaseDC(_hWnd, memDC);
	ReleaseDC(_hWnd, dc);

	m_bVideoPlaying = false;
	MCIWndClose(m_hVideoHandle);
}
