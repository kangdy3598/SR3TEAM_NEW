#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;
class CPlantOrbCom;

END

class CPlantOrb :public Engine::CGameObject
{
private:
	explicit CPlantOrb(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlantOrb();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:	
	void Set_ImageID(_int _iId) { m_iImageID = _iId; }
	_int Get_ImageID() { return m_iImageID; }	
	void Init_Pos(_float _fX, _float _fZ, _float _fDir);
	void Set_Speed(_float _fSpeed) { m_fSpeed = _fSpeed; }
	void Set_Duration() { m_fDruration = 0; }
	void Move(const _float& fTimeDelta);

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;	
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;
	Engine::CPlantOrbCom* m_pPlantOrbCom;
	Engine::CTexture* m_pShadowTextureCom;
	Engine::CTransform* m_pShadowTransformCom;

public:
	static CPlantOrb* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:	
	vector<IDirect3DTexture9*> m_vecTexture;
	_int m_iImageID;
	_float m_fSpeed;
	_float m_fDruration;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};