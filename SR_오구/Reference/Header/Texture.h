#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture :public CComponent
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	HRESULT		Ready_Texture(const _tchar* pPath, TEXTUREID eType, const int& iCnt);
	HRESULT		Ready_Texture(list<const _tchar*> pPathList);
	void		Set_Texture(const _uint& iIndex = 0);
	vector<IDirect3DBaseTexture9*> Get_Texture() { return m_vecTexture; }

private:
	vector<IDirect3DBaseTexture9*>		 m_vecTexture;

public:
	static  CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTUREID eType, const _uint& iCnt = 1);
	static  CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, list<const _tchar*> pPathList);
	virtual CComponent* Clone();

private:
	virtual void Free();

};

END
