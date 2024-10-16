#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CStartMap : public CVIBuffer
{
private:
	explicit CStartMap();
	explicit CStartMap(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStartMap(const CStartMap& rhs);
	virtual ~CStartMap();


public:
	virtual HRESULT Ready_Buffer();
	virtual void    Render_Buffer();

public:
	static  CStartMap*  Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();


private:

	virtual  void Free();


};

END