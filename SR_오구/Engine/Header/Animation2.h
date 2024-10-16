#pragma once
#include "VIBuffer.h"
#include "Animator2.h"

BEGIN(Engine)

struct tAnimFrm
{
	_vec2 vLT;           // 좌상단 정보
	_vec2 vSlice;        // 짜를 값
	//_vec2 vOffset;       // 오프셋
	float fDuration;     // 시간 값 
};



class ENGINE_DLL CAnimation2 : public CVIBuffer
{
private:
	wstring			      m_strName;
	CAnimator2* m_pAnimator;
	IDirect3DTexture9* m_pTex;       // Animation이 사용하는 텍스처
	vector<tAnimFrm>      m_vecFrm;		// 모든 프레임 정보 
	int                   m_iCurFrm;    // 현재 프레임
	float                 m_fAccTime;   // 시간 누적

	bool				  m_bFinish;    // 재생 끝에 도달 여부

private:
	explicit CAnimation2();
	explicit CAnimation2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimation2(const CVIBuffer& rhs);
	virtual ~CAnimation2();

public:

	const wstring& GetName() { return m_strName; }
	bool IsFinish() { return m_bFinish; }
	void SetFinish(bool _bFinish) { m_bFinish = _bFinish; }


	void SetFrame(int _iFrameIdx)
	{
		m_bFinish = false;
		m_iCurFrm = _iFrameIdx;
		m_fAccTime = 0.f;
	}


	int GetCurrentFrm() { return m_iCurFrm; }
	tAnimFrm& GetFrame(int _iIdx) { return m_vecFrm[_iIdx]; }
	UINT GetMaxFrame() { return (UINT)m_vecFrm.size(); }


private:
	void SetName(const wstring& _strName) { m_strName = _strName; }


public:
	HRESULT Create(IDirect3DTexture9* _pTex,
		_vec2 _vLT,
		_vec2 _vSliceSize,
		_vec2 _vStep,
		float _fDuration,
		UINT  _iFrameCount);

	void update();
	virtual  HRESULT  Ready_Buffer();
	virtual  void     Render_Buffer();

public:
	friend class CAnimator2;

public:
	virtual CComponent* Clone();
};

END