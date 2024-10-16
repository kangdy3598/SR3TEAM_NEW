#include "Export_Utility.h"
inline HRESULT Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement)
{ // 여기서 pGraphicDev는 어디다가 사용하는지 현재 
	CManagement* pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppManagement = pManagement;

	return S_OK;
}

inline CComponent* Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag)
{
	return CManagement::GetInstance()->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

inline CGameObject* Get_GameObject(const _tchar* pLayeTag, const _tchar* pObjTag)
{
	// 여기서부터 다시 코드작성 
	return CManagement::GetInstance()->Get_GameObject(pLayeTag, pObjTag);
}

inline CScene* Get_CurScenePtr()
{
	return CManagement::GetInstance()->GetCurScenePtr();
}

inline CLayer* Get_Layer(const _tchar* pLayerTag)
{
	return CManagement::GetInstance()->Get_Layer(pLayerTag);
}

inline HRESULT Set_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}

inline _int Update_Scene(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}

inline void LateUpdate_Scene(const _float& fTimeDelta)
{
	CManagement::GetInstance()->LateUpdate_Scene(fTimeDelta);
}

inline void Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
}


inline void Sound_Initialize()
{
	CSoundManager::GetInstance()->Initialize();
}

inline void Play_Sound(const _tchar* pSoundKey, CHANNELID eID, float fVolume) {
	CSoundManager::GetInstance()->Play_Sound(pSoundKey, eID, fVolume);
}

inline void PlayBGM(const _tchar* pSoundKey, float fVolume) {
	CSoundManager::GetInstance()->PlayBGM(pSoundKey, fVolume);
}

inline void StopSound(CHANNELID eID) {
	CSoundManager::GetInstance()->StopSound(eID);
}

inline void StopAll() {
	CSoundManager::GetInstance()->StopAll();
}

inline void SetChannelVolume(CHANNELID eID, float fVolume) {
	CSoundManager::GetInstance()->SetChannelVolume(eID, fVolume);
}

inline _tchar* Get_CurBGM() {
	return CSoundManager::GetInstance()->Get_CurBGM();
}


inline HRESULT Ready_Proto(const _tchar* pComponentTag, CComponent* pComponent)
{
	return CComponentMgr::GetInstance()->Ready_Proto(pComponentTag, pComponent);
}

inline CComponent* Clone_Proto(const _tchar* pComponentTag)
{
	return CComponentMgr::GetInstance()->Clone_Proto(pComponentTag);
}

inline void Add_RenderGroup(RENDERID eType, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eType, pGameObject);
}

inline void Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}

inline void Clear_RenderGroup()
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}

inline HRESULT Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

void		Release_Utility()
{
	CRenderer::DestroyInstance();
	CComponentMgr::DestroyInstance();
	CManagement::DestroyInstance();
}
