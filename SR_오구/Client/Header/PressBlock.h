#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

class CPressBlock :public Engine::CGameObject
{
private:
	explicit CPressBlock(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPressBlock();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

private:
	HRESULT    Add_Component();

public:
	_bool Is_Clear() { return m_bIsCleared; }
	_int Get_ImageID() { return m_iImageID; }
	void Init(CGameObject* _pGroup, _int _iID, _float _fX, _float _fZ);
	void Set_MaxID(_int _iID) { m_iMaxID = _iID; }
	void Set_Clear(_int _bIsClear) { m_bIsCleared = _bIsClear; }
	void Set_ImageID(_int _iID) { m_iImageID = _iID; }
	void Set_Group(CGameObject* _pGroup) { m_pGroup = _pGroup; }

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTexTransformCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

public:
	static CPressBlock* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CGameObject* m_pGroup;
	vector<IDirect3DTexture9*> m_vecTexture;
	_int m_iImageID;
	_int m_iMaxID;
	_bool m_bIsPressed;
	_bool m_bIsCleared;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};