#include "AnimationTex.h"

CAnimationTex::CAnimationTex()
	:m_fAccTime(0.f)
{
}

CAnimationTex::CAnimationTex(LPDIRECT3DDEVICE9 pGraphicDev)
    :CVIBuffer(pGraphicDev)
	, m_fAccTime(0.f)
{
}

CAnimationTex::CAnimationTex(const CAnimationTex& rhs)
    :CVIBuffer(rhs)
	, m_fAccTime(0.f)
{
	
}

CAnimationTex::~CAnimationTex()
{
}


HRESULT CAnimationTex::Ready_Buffer()
{
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

	m_pIB->Unlock();

	return S_OK;
}

//int _iXcount, int _iYcount, int _iFrameIndex
_int CAnimationTex::Update_Component_test(const _float& fTimeDelta
									, int _iXcount
									, int _iYcount
									, int _iStartIndex
									, int _iEndIndex
									, int TotalCount)
{

	// 3초마다 업데이트 될수있도록 현재 설정
	// 저 3초가 duration 값이 들어가면 될듯함.

	static int currentFrame = _iStartIndex;

	m_fAccTime += fTimeDelta; 

	float uStep = 1.f / _iXcount;
	float vStep = 1.f / _iYcount;

	if (m_fAccTime > 0.2f)
	{

		currentFrame = currentFrame % TotalCount;  // 총 프레임 수에 맞게 반복

		// UV 좌표 업데이트
		VTXTEX* pVertex = nullptr;
		m_pVB->Lock(0, 0, (void**)&pVertex, 0);

		// X와 Y에서의 UV 스텝 계산
		float uStep = 1.f / _iXcount;
		float vStep = 1.f / _iYcount;

		// 현재 프레임 인덱스를 바탕으로 UV 좌표 계산
		int frameX = currentFrame % _iXcount;
		int frameY = currentFrame / _iXcount;

		// 첫 번째 정점의 UV 좌표 업데이트
		pVertex[0].vTexUV = { frameX * uStep, frameY * vStep };

		// 나머지 정점의 UV 좌표 업데이트
		pVertex[1].vTexUV = { pVertex[0].vTexUV.x + uStep, pVertex[0].vTexUV.y };
		pVertex[2].vTexUV = { pVertex[0].vTexUV.x + uStep, pVertex[0].vTexUV.y + vStep };
		pVertex[3].vTexUV = { pVertex[0].vTexUV.x, pVertex[0].vTexUV.y + vStep };

		m_pVB->Unlock();
	
		if(currentFrame < _iEndIndex)
		{
			currentFrame++;
		}

		else
		{
			currentFrame = _iStartIndex;
		}

		m_fAccTime = 0.f; 
	}
	return _int();
}


void CAnimationTex::Render_Buffer()
{
	// 업데이트를 넣어서 설정한 애니메이션 1타임 시간이 지날때 마다 
	// uv좌표의 비율을 업데이트를 하여 애니메이션이 나오도록 하는게 맞나
	CVIBuffer::Render_Buffer();
}

CAnimationTex* CAnimationTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAnimationTex* pInstance = new CAnimationTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))	
	{
		Safe_Release(pInstance);
		MSG_BOX("RcTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CAnimationTex::Clone()
{
	return new CAnimationTex(*this);
}

void CAnimationTex::Free()
{
	CVIBuffer::Free();	
}





//HRESULT CAnimationTex::Ready_Buffer(int _iXcount, int _iYcount, int _iFrameIndex)
//{
//	m_dwTriCnt = 2;
//	m_dwVtxCnt = 4;
//	m_dwVtxSize = sizeof(VTXTEX);
//	m_dwFVF = FVF_TEX;
//
//	m_dwIdxSize = sizeof(INDEX32);
//	m_IdxFmt = D3DFMT_INDEX32;
//
//	float uStep = 1.f / _iXcount;	
//	float vStep = 1.f / _iYcount;	
//
//
//	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);
//
//	VTXTEX* pVertex = NULL;
//
//	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
//
//
//	pVertex[0].vPosition = { -1.f, 1.f, 0.f };
//	pVertex[0].vTexUV = { (_iFrameIndex % _iXcount)*uStep, ((_iFrameIndex / _iXcount)* vStep)};
//	
//	pVertex[1].vPosition = { 1.f, 1.f, 0.f };
//	pVertex[1].vTexUV = { pVertex[0].vTexUV.x + uStep , pVertex[0].vTexUV.y };
//	
//	pVertex[2].vPosition = { 1.f, -1.f, 0.f };
//	pVertex[2].vTexUV = { pVertex[0].vTexUV.x + uStep , pVertex[0].vTexUV.y + vStep };
//	
//	pVertex[3].vPosition = { -1.f, -1.f, 0.f };
//	pVertex[3].vTexUV = { pVertex[0].vTexUV.x , pVertex[0].vTexUV.y + vStep };
//
//	m_pVB->Unlock();
//
//	INDEX32* pIndex = nullptr;
//
//	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
//
//	// 오른쪽 위
//	pIndex[0]._0 = 0;
//	pIndex[0]._1 = 1;
//	pIndex[0]._2 = 2;
//
//	// 왼쪽 아래
//	pIndex[1]._0 = 0;
//	pIndex[1]._1 = 2;
//	pIndex[1]._2 = 3;
//
//	m_pIB->Unlock();
//
//	return S_OK;
//}