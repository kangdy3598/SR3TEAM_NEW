#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END


class CTestCol2 : public Engine::CGameObject
{
private:
	explicit CTestCol2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestCol2();

public:

	virtual			HRESULT			Ready_GameObject();
	virtual			void			LateReady_GameObject();
	virtual			_int			Update_GameObject(const _float& fTimeDelta);
	virtual			void			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void			Render_GameObject();

	virtual			void			OnCollision(CGameObject* _pOther);
public:
	void			SetPlayer(CPlayer* _Player) { m_CPlayer = _Player; }
private:
	HRESULT			Add_Component();

private:
	IDirect3DTexture9* m_Texture;
	_int m_iImageID;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;
	Engine::CTexture* m_pTexture;

	CPlayer* m_CPlayer;
public:
	static CTestCol2* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

