#include "MyFont.h"

CMyFont::CMyFont(LPDIRECT3DDEVICE9 pGraphicDev)
    :m_pGraphicDev(pGraphicDev)
    , m_pFont(nullptr)
    , m_pSprite(nullptr)
{
    m_pGraphicDev->AddRef();
}

CMyFont::~CMyFont()
{
}

HRESULT CMyFont::Ready_Font(const _tchar* pFontType,
    const _uint& iWidth,
    const _uint& iHeight,
    const _uint& iWeight)
{
    D3DXFONT_DESC  tFont_Desc;
    ZeroMemory(&tFont_Desc, sizeof(D3DXFONT_DESC));

    tFont_Desc.CharSet = HANGUL_CHARSET;
    tFont_Desc.Width = iWidth;
    tFont_Desc.Height = iHeight;
    tFont_Desc.Weight = iWeight;

    //0913 폰트 효과 추가(안티 앨리어싱) 
    //tFont_Desc.Quality = ANTIALIASED_QUALITY;
    tFont_Desc.Quality = PROOF_QUALITY;

    lstrcpy(tFont_Desc.FaceName, pFontType);

    if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &tFont_Desc, &m_pFont)))
    {
        MSG_BOX("Font Create Failed");
        return E_FAIL;
    }

    if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
    {
        MSG_BOX("Sprite Create Failed");
        return E_FAIL;
    }

    return S_OK;
}

void CMyFont::Render_Font(const _tchar* pString,
    const _vec2* pLTPos,
    D3DXCOLOR Color,
    TEXTRANGE _eTextRange,
    _vec2 vXYSize)
{
    //RECT rc{ (_long)pPos->x, (_long)pPos->y };
    RECT rc{ (_long)pLTPos->x, (_long)pLTPos->y, (_long)pLTPos->x + vXYSize.x, (_long)pLTPos->y + vXYSize.y }; // 렉트 RB 설정

    m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

    if (_eTextRange == TEXT_DEFAULT)
        m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rc, DT_WORDBREAK, Color); // 기본정렬 + 줄바꿈
    else if (_eTextRange == TEXT_CENTER)
        m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rc, DT_CENTER | DT_VCENTER | DT_WORDBREAK, Color); // 상 하 가운데정렬 + 줄바꿈

    m_pSprite->End();
}

CMyFont* CMyFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
    CMyFont* pInstance = new CMyFont(pGraphicDev);

    if (FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
    {
        Safe_Release(pInstance);
        MSG_BOX("폰트 문제");
        return nullptr;
    }

    return pInstance;
}

void CMyFont::Free()
{
    Safe_Release(m_pFont);
    Safe_Release(m_pSprite);
    Safe_Release(m_pGraphicDev);

}
