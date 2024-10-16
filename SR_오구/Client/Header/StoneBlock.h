#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CStoneBlock :public Engine::CGameObject
{
private:
	explicit CStoneBlock(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStoneBlock();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

private:
	Engine::CRcTex* m_pBufferCom;	
	Engine::CTransform* m_pTransformCom;	
	Engine::CTransform* m_pHoleTransformCom;
	Engine::CCollider* m_pBoundBox;

private:
	HRESULT    Add_Component();

public:
	void Init(_float _fX, _float _fZ, _bool _bIsUp);
	_bool Is_Move() { return m_bIsMove; }
	_int Get_ImageID() { return m_iImageID; }	
	void Set_ImageID(_int _iID) { m_iImageID = _iID; }	
	void Move_StoneBlock();
	void Move_StoneBlockOnce();

private:	
	_bool m_bIsMove;
	_bool m_bIsUp;
	_bool m_bActiceOnce;
	_vec3 m_fTargetPos;
	_float m_fMoveTime;
	_int m_iImageID, m_iHoleImageID;	
	vector<IDirect3DTexture9*> m_vecTexture;
	vector<IDirect3DTexture9*> m_vecHoleTexture;

public:
	static CStoneBlock* Create(LPDIRECT3DDEVICE9 pGraphicDev);		

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};
