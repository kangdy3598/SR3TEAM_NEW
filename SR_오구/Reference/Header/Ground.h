#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CGround :public CVIBuffer
{

private:
	explicit CGround();
	explicit CGround(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGround(const CGround& rhs);
	virtual ~CGround();

public:
	virtual		HRESULT			Ready_Buffer();
	virtual		void			Render_Buffer();

public:
	static CGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent* Clone();

private:
	virtual void	Free();


};

END
