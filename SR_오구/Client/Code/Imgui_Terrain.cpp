#include "pch.h"
#include "Imgui_Terrain.h"

CImgui_Terrain::CImgui_Terrain()
	: m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxCnt(0)
	, m_dwVtxSize(0)
	, m_dwTriCnt(0)
	, m_dwIdxSize(0)
{
	//주의할점 그래픽 디바이스 보다 먼저 객체가 생성되면 안됨! 
}

CImgui_Terrain::~CImgui_Terrain()
{

}

HRESULT CImgui_Terrain::Ready_Buffer()
{
	m_pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	//m_pVB->AddRef(); 안해도될듯 나중에 스마트포인터 쓰니깐

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize,
														  0,
														  m_dwFVF,
														  D3DPOOL_MANAGED,
														  &m_pVB,
														  NULL),
														  E_FAIL);
	

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwTriCnt* m_dwIdxSize,
														 0,
														 m_IdxFmt,
														 D3DPOOL_MANAGED,	
														 &m_pIB,
														 NULL),
														 E_FAIL);


	return S_OK;
}

HRESULT CImgui_Terrain::update()
{
 	m_dwTriCnt = 1; // Set_Terrain 함수로 만들어서 이걸 함수 입력 매개변수로 넣으면 될듯 
	m_dwVtxCnt = 3; 
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	Ready_Buffer(); // 버텍스버퍼와 인덱스 버퍼 만들어주는 함수
	
	VTXCOL* pVertex = NULL; 

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);


	pVertex[0].vPosition = { 0.f,1.f,0.f };	
	pVertex[0].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);	

	pVertex[1].vPosition = { 1.f, -1.f, 0.f };	
	pVertex[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);	

	pVertex[2].vPosition = { -1.f,-1.f,0.f };	
	pVertex[2].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);	

	m_pVB->Unlock();	


	INDEX32* pIndex = nullptr; 

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	m_pIB->Unlock();

	return S_OK;
 
}

void CImgui_Terrain::Render()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwFVF);

	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void CImgui_Terrain::Release()
{
	Safe_Release(m_pIB);	
	Safe_Release(m_pVB);	
}
