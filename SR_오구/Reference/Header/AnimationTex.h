#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CAnimationTex : public CVIBuffer
{
private:
	explicit CAnimationTex();
	explicit CAnimationTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimationTex(const CAnimationTex& rhs);
	virtual ~CAnimationTex();

public:
	_int  Update_Component_test(const _float& fTimeDelta, 
									int _iXcount, 
									int _iYcount,
									int _iStartIndex,
									int _iEndIndex,
									int TotalCount);
	HRESULT		Ready_Buffer();
	virtual		void		Render_Buffer();

public:
	static		CAnimationTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		CComponent* Clone();

private:
	virtual void		Free();


//9월 5일 추가한 코드 
private:
	float m_fAccTime;

};

END