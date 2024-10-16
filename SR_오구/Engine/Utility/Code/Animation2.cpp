#include "Animation2.h"
#include "TimerMgr.h"

CAnimation2::CAnimation2()
	:m_strName(L"")
	, m_pAnimator(nullptr)
	, m_pTex(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{

}

CAnimation2::CAnimation2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_strName(L"")
	, m_pAnimator(nullptr)
	, m_pTex(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{

}

CAnimation2::CAnimation2(const CVIBuffer& rhs)
	:CVIBuffer(rhs)
	//:CVIBuffer(rhs)
	//, m_strName(rhs)
	//, m_pAnimator(nullptr)
	//, m_pTex(nullptr)
	//, m_iCurFrm(0)
	//, m_fAccTime(0.f)
	//, m_bFinish(false)

{

}

CAnimation2::~CAnimation2()
{
}



void CAnimation2::update()
{
	if (m_bFinish)
		return;

	m_fAccTime += CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");

	if (m_fAccTime > m_vecFrm[m_iCurFrm].fDuration)
	{
		++m_iCurFrm;

		if (m_vecFrm.size() <= m_iCurFrm)
		{
			m_iCurFrm = -1;
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}

		m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;
	}

}

HRESULT CAnimation2::Ready_Buffer()
{

	return S_OK;
}

void CAnimation2::Render_Buffer()
{
	if (m_bFinish)
		return;

	VTXTEX2* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vTexUV = { m_vecFrm[m_iCurFrm].vLT.x,m_vecFrm[m_iCurFrm].vLT.y };

	pVertex[1].vTexUV = { m_vecFrm[m_iCurFrm].vLT.x + m_vecFrm[m_iCurFrm].vSlice.x
						 , m_vecFrm[m_iCurFrm].vLT.y };

	pVertex[2].vTexUV = { m_vecFrm[m_iCurFrm].vLT.x + m_vecFrm[m_iCurFrm].vSlice.x
						 , m_vecFrm[m_iCurFrm].vLT.y + m_vecFrm[m_iCurFrm].vSlice.y };

	pVertex[3].vTexUV = { m_vecFrm[m_iCurFrm].vLT.x
						 , m_vecFrm[m_iCurFrm].vLT.y + m_vecFrm[m_iCurFrm].vSlice.y };

	m_pVB->Unlock();

	CVIBuffer::Render_Buffer();
}


HRESULT CAnimation2::Create(IDirect3DTexture9* _pTex, _vec2 _vLT, _vec2 _vSliceSize, _vec2 _vStep, float _fDuration, UINT _iFrameCount)
{


	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX2);
	m_dwFVF = FVF_TEX2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;


	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX2* pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);


	pVertex[0].vPosition = { -1.f, 1.f, 0.f };
	pVertex[1].vPosition = { 1.f, 1.f, 0.f };
	pVertex[2].vPosition = { 1.f, -1.f, 0.f };
	pVertex[3].vPosition = { -1.f, -1.f, 0.f };


	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 왼쪽 아래
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	D3DSURFACE_DESC desc;

	m_pTex = _pTex;
	_pTex->GetLevelDesc(0, &desc);

	UINT textureWidth = desc.Width;
	UINT textureHeight = desc.Height;

	tAnimFrm frm = {};

	for (UINT i = 0; i <= _iFrameCount; ++i)
	{

		frm.fDuration = _fDuration;
		frm.vSlice = { _vSliceSize.x  / textureWidth, _vSliceSize.y  / textureHeight };
		frm.vLT = { _vLT.x / textureWidth + _vStep.x / textureWidth * (float)i,  _vLT.y / textureHeight + _vStep.y / textureHeight * (float)i };

		m_vecFrm.push_back(frm);
	}

	return S_OK;
}



CComponent* CAnimation2::Clone()
{
	return new CAnimation2(*this);
}
