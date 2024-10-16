#pragma once
#include "PuzzleObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CLightFlower :public Engine::CPuzzleObject
{
private:
	explicit CLightFlower(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightFlower();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();	

public:
	void Set_Glow(CGameObject* _pObj) { m_pGlow = _pObj; }
	void Set_Pedestal(CGameObject* _pObj) { m_pPedestal = _pObj; }
	void Init_Pos(_float _fX, _float _fZ);
	void Match_Puzzle() override;

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;	
	Engine::CTransform* m_pRootTransformCom;
	Engine::CCollider* m_pBoundBox;

public:
	static CLightFlower* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	vector<IDirect3DTexture9*> m_vecTexture;
	_int m_iImageID;
	_float m_fTime;
	_int m_iTargetID;
	CGameObject* m_pGlow;
	CGameObject* m_pPedestal;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};