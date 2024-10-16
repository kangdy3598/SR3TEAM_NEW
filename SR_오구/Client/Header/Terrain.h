#pragma once
#include "GameObject.h"


BEGIN(Engine)

class CTerrainTex;
class CTransform;
class CTexture;	

END

class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain();

public:
	virtual			HRESULT			Ready_GameObject();
	virtual			_int			Update_GameObject(const _float& fTimeDelta);
	virtual			void			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void			Render_GameObject();

private:
	HRESULT			Add_Component();
	HRESULT         SetUp_Material(); 
	void			Key_Input(const _float& fTimeDelta);

private:
	Engine::CTerrainTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free();
};



