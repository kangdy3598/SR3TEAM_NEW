#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex :public CVIBuffer
{
private:
	explicit CRcTex();
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex();


public:
	virtual  HRESULT  Ready_Buffer();
	virtual  void	  Render_Buffer();
	virtual _vec3* Get_VtxPos() { return m_pPos; }

public:
	static   CRcTex*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual  CComponent*		Clone();

private:
	virtual void Free();
};

END

