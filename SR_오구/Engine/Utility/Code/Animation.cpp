#include "Animation.h"

CAnimation::CAnimation()
	: m_eCurrentState(PLAYERSTATE::PLY_IDLE)
	, m_iCurrentDir(0)
	, currentFrame(0)
	, m_fAccTime(0.2f)
	, m_bAnimationEnd(false)
	, m_bAniamtionPause(false)
{
}

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
	, m_eCurrentState(PLAYERSTATE::PLY_IDLE)
	, m_iCurrentDir(0)
	, currentFrame(0)
	, m_fAccTime(0.2f)
	, m_bAnimationEnd(false)
	, m_bAniamtionPause(false)
{
	//pGraphicDev->AddRef();
}

CAnimation::CAnimation(const CAnimation& rhs)
	:CVIBuffer(rhs)
	, m_eCurrentState(PLAYERSTATE::PLY_IDLE)
	, m_iCurrentDir(0)
	, currentFrame(0)
	, m_fAccTime(0.2f)
	, m_bAnimationEnd(false)
	, m_bAniamtionPause(false)
{
	m_currentFrameCount = rhs.m_currentFrameCount;
	m_vecFrameCount = rhs.m_vecFrameCount;

	for (int i = 0; i < OBJ_DIRECTION::OBJDIR_END; i++)
	{
		m_currentFramePlay[i] = rhs.m_currentFramePlay[i];

	}
	m_vecFramePlay = rhs.m_vecFramePlay;
}

CAnimation::~CAnimation()
{
}

HRESULT CAnimation::Ready_Animation(
	const vector<_vec2> _vecFrameCount,
	const vector<vector<int>*> _vecFramePlay)
{
	// Animation.dat에 읽은 정보 저장하기
	m_vecFrameCount.clear();
	for (auto iter : _vecFrameCount)
	{
		m_vecFrameCount.push_back(iter);
	}

	m_vecFramePlay.clear();
	for (auto iter : _vecFramePlay)
	{
		m_vecFramePlay.push_back(iter);
	}

	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;


	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX* pVertex = NULL;

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

	return S_OK;
}

_int CAnimation::Update_Component(const _float& fTimeDelta)
{
	if (m_bAniamtionPause)
		return 0;

	if (fTimeDelta > 10.f)
		return 0;

	m_fAccTime -= fTimeDelta;
	if (m_fAccTime <= 0.f)
	{
		m_bAnimationEnd = false;
		m_fAccTime += 0.2f;

		int size = m_vecFramePlay[m_eCurrentState][m_iCurrentDir].size();
		if (currentFrame >= size - 1)
		{
			m_bAnimationEnd = true;

		}

		if (currentFrame >= size)
		{
			currentFrame = 0;
		}
		UpdateUV();
		currentFrame++;
	}
	return 0;
}

void CAnimation::LateUpdate_Component(const _float& fTimeDelta)
{
}

void CAnimation::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const vector<_vec2> _vecFrameCount,
	const vector<vector<int>*> _vecFramePlay)
{
	CAnimation* pAnimation = new CAnimation(pGraphicDev);

	if (FAILED(pAnimation->Ready_Animation(_vecFrameCount, _vecFramePlay)))
	{
		Safe_Release(pAnimation);
		MSG_BOX("Animation Create Failed");
		return nullptr;
	}

	return pAnimation;
}

CComponent* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	CComponent::Free();
}

void CAnimation::SetAnimFrame(PLAYERSTATE _eObjState, bool _bDiagonal)
{
	currentFrame = 0;
	m_eCurrentState = _eObjState;
	if (_bDiagonal)
		m_eCurrentState = static_cast<PLAYERSTATE>(_eObjState + 1);

	m_currentFrameCount = m_vecFrameCount[m_eCurrentState];

	// 상하좌우 방향
	for (int i = 0; i < OBJ_DIRECTION::OBJDIR_END; i++)
	{
		m_currentFramePlay[i].clear();
		for (auto iter : m_vecFramePlay[m_eCurrentState][i])
		{
			m_currentFramePlay[i].push_back(iter);

		}
	}

	m_vecCurrentFrameUV.x = 1.f / m_currentFrameCount.x;
	m_vecCurrentFrameUV.y = 1.f / m_currentFrameCount.y;


	UpdateUV();
}

void CAnimation::UpdateUV()
{
	VTXTEX* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	int frameX = m_vecFramePlay[m_eCurrentState][m_iCurrentDir][currentFrame]
		% (int)m_currentFrameCount.x;
	int frameY = m_vecFramePlay[m_eCurrentState][m_iCurrentDir][currentFrame]
		/ m_currentFrameCount.x;

	float FrameU = 1.f / m_currentFrameCount.x;
	float FrameV = 1.f / m_currentFrameCount.y;

	pVertex[0].vTexUV = { frameX * FrameU,
						  frameY * FrameV };
	pVertex[1].vTexUV = { pVertex[0].vTexUV.x + FrameU,
						  pVertex[0].vTexUV.y };
	pVertex[2].vTexUV = { pVertex[0].vTexUV.x + FrameU,
						  pVertex[0].vTexUV.y + FrameV };
	pVertex[3].vTexUV = { pVertex[0].vTexUV.x,
						  pVertex[0].vTexUV.y + FrameV };

	m_pVB->Unlock();
}