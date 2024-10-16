#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
END


class CStartBackGround : public Engine::CGameObject
{
private:
	explicit CStartBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStartBackGround();


public:
	virtual    HRESULT    Ready_GameObject();
	virtual    _int       Update_GameObject(const _float& fTimeDelta);
	virtual    void		  LateUpdate_GameObject(const _float& fTimeDelta);
	virtual    void       Render_GameObject();


private:
	HRESULT		Add_Component();



private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CStartBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	virtual void Free();




};
