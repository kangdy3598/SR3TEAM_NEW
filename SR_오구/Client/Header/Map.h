#pragma once
#include "GameObject.h"


BEGIN(Engine)

class CTexture;
class CMapTex;
class CTransform;

END


class CMap : public Engine::CGameObject
{
private:
	explicit CMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMap();

public:
	virtual  HRESULT  Ready_GameObject();
	virtual  _int     Update_GameObject(const _float& fTimeDelta);
	virtual  void     LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void     Render_GameObject();


private:
	HRESULT  Add_Component();
	HRESULT  SetUp_Material();
	void     Key_Input(const _float& fTimeDelta);

private:
	Engine::CMapTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom; 

public:
	static CMap* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(); 

};
