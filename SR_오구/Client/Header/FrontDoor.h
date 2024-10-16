#pragma once
#include "PuzzleObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CFrontDoor :public Engine::CPuzzleObject
{
private:
	explicit CFrontDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFrontDoor();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	static CFrontDoor* Create(LPDIRECT3DDEVICE9 pGraphicDev);	
	void Init_Pos(_float _fX, _float _fZ);
	void Match_Puzzle() override;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pLTextureCom;
	Engine::CTexture* m_pRTextureCom;
	Engine::CTransform* m_pLTransformCom;	
	Engine::CTransform* m_pRTransformCom;

private:
	HRESULT    Add_Component();


private:
	_bool m_bIsOpened;
	_float m_fOpenTime;	
	_float m_fDoorSize;

private:
	virtual void Free();
};