#pragma once

// 이미지 : 특정 애니메이션 모습이 담겨 있는 png파일 한 장 (Sprite_BabyOgu_Idle.png)
// 프레임 : 이미지 안에 있는 애니메이션 한 조각 (Idle이미지엔 7x4 프레임)

#include "VIBuffer.h"
#include "Export_Utility.h"
BEGIN(Engine)

class ENGINE_DLL CAnimation : public CVIBuffer//CComponent
{

private:
	explicit CAnimation();
	explicit CAnimation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimation(const CAnimation& rhs);
	virtual ~CAnimation();

public:
	HRESULT					Ready_Animation(const vector<_vec2> _vecFrameCount,
		const vector<vector<int>*> _vecFramePlay);

	virtual  _int			Update_Component(const _float& fTimeDelta);
	virtual  void			LateUpdate_Component(const _float& fTimeDelta);
	virtual	 void   		Render_Buffer();

public:
	void SetAnimFrame(PLAYERSTATE _ePlayerState, bool _bDiagonal);
	void SetAnimDir(PLAYERSTATE _ePlayerState, _vec3 _dir, bool& _bDiagonal)
	{
		if (_dir.x == 1)
		{
			if (_dir.z == -1)
			{
				_bDiagonal = true;
				m_iCurrentDir = 3;
			}
			else if (_dir.z == 0)
			{
				_bDiagonal = false;
				m_iCurrentDir = 3;

			}
			else
			{
				_bDiagonal = true;
				m_iCurrentDir = 2;
			}
		}
		else if (_dir.x == 0)
		{

			if (_dir.z == 1)
			{
				_bDiagonal = false;
				m_iCurrentDir = 0;
			}
			else if (_dir.z == 0)
			{
			}
			else
			{
				_bDiagonal = false;
				m_iCurrentDir = 1;
			}
		}
		else
		{
			if (_dir.z == -1)
			{
				_bDiagonal = true;
				m_iCurrentDir = 1;
			}
			else if (_dir.z == 0)
			{
				_bDiagonal = false;
				m_iCurrentDir = 2;
			}
			else
			{
				_bDiagonal = true;
				m_iCurrentDir = 0;
			}
		}
		SetAnimFrame(_ePlayerState, _bDiagonal);
	}

	void UpdateUV();
	int  GetCurrentFrame() { return currentFrame; }
	bool IsAnimationEnd() { return m_bAnimationEnd; }
	void SetAnimationPlaying() { m_bAnimationEnd = false; }
	void SetAnimationPause(bool _value) { m_bAniamtionPause = _value; }
private:

	// Loading에서 Animation.dat 정보를 받아와서
	// 해당 오브젝트의 모든 행동 애니메이션이 담겨 있음
	vector<_vec2>		 m_vecFrameCount;
	vector<vector<int>*> m_vecFramePlay;

	// 현재 진행중인 상태에 대한 애니메이션 정보
	_vec2				 m_currentFrameCount; // 프레임 개수
	_vec2				 m_vecCurrentFrameUV; // 현재 프레임을 출력하기 위한 UV 비율
	vector<int>			 m_currentFramePlay[OBJ_DIRECTION::OBJDIR_END]; // 프레임 출력 순서


	PLAYERSTATE			m_eCurrentState;
	//int					m_eCurrentDir;
	int					m_iCurrentDir;

	int					currentFrame;
	float				m_fAccTime;

	bool				m_bAnimationEnd;
	bool				m_bAniamtionPause;
public:
	static CAnimation* Create(LPDIRECT3DDEVICE9	      pGraphicDev,
		const vector<_vec2>		  _vecFrameCount,
		const vector<vector<int>*> _vecFramePlay);

	virtual CComponent* Clone();

private:
	virtual void			Free();


};

END