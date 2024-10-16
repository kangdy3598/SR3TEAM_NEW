#pragma once
#include "GameObject.h"
#include "Export_Utility.h"
#include "Export_System.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END


class CEquipHat : public Engine::CGameObject
{
protected:
	explicit CEquipHat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEquipHat();

public:

	enum EQUIPITEM { EXPLORE, PARTY, MOHICAN, ENKY, APPLE, POOP, PENGUIN, EQUIP_END };

public:

public:
	virtual   HRESULT   Ready_GameObject();
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void SetHatTexture(CTexture* _pTextureCom) { m_pTextureCom = _pTextureCom; }
private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	CPlayer* m_pPlayer;

public:
	static CEquipHat* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free();
};

