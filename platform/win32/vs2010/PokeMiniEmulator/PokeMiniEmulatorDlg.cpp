#include "stdafx.h"
#include "PokeMiniEmulator.h"
#include "PokeMiniEmulatorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPokeMiniEmulatorDlg::CPokeMiniEmulatorDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CPokeMiniEmulatorDlg::IDD, pParent)
    , m_pSystem(GS_NULL)
    , m_state(EMU_STOP)
    , m_pRenderDev(GS_NULL)
    , m_pPaintDev(GS_NULL)
    , m_soundBufferThreshold(0)
    , m_pRomData(GS_NULL)
    , m_romSize(0)
    , m_pSaveData(GS_NULL)
    , m_emuThread(GS_NULL)
    , m_emuThreadID(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPokeMiniEmulatorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_SCREEN, m_screen);
}

BEGIN_MESSAGE_MAP(CPokeMiniEmulatorDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CPokeMiniEmulatorDlg message handlers

BOOL CPokeMiniEmulatorDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // extra initialization
    m_pSystem = (IGsPmSystem *)GsCreatePokeMiniEmuHandle();
    GS_ASSERT(GS_NULL != m_pSystem);
    m_cpuRate = m_pSystem->CpuRate();

    {
        m_dev[0].config = GS_COLOR_CONFIG_GRAY;
        m_dev[0].width = m_pSystem->ScreenWidth();
        m_dev[0].height = m_pSystem->ScreenHeight();
        m_dev[0].rowBytes = m_dev[0].width;
        m_dev[0].pData = GS_MALLOCZ(m_dev[0].height * m_dev[0].rowBytes);
        GS_ASSERT(GS_NULL != m_dev[0].pData);

        m_dev[1] = m_dev[0];
        m_dev[1].pData = GS_MALLOCZ(m_dev[1].height * m_dev[1].rowBytes);
        GS_ASSERT(GS_NULL != m_dev[1].pData);
    }

    m_pRenderDev = &(m_dev[0]);
    m_pPaintDev = &(m_dev[1]);

    {
        m_screenDev.config = GS_COLOR_CONFIG_BGRX_8888;
        m_screenDev.width = m_pPaintDev->width;
        m_screenDev.height = m_pPaintDev->height;
        m_screenDev.rowBytes = m_screenDev.width * 4;
        m_screenDev.pData = GS_MALLOCZ(m_screenDev.height * m_screenDev.rowBytes);
        GS_ASSERT(GS_NULL != m_screenDev.pData);
    }

    for (GS_UINT i = 0; i < 0x100; ++i)
    {
        m_colorTable[i] = (0xFF << 24) 
                        | ((0xFF - i) << 16) 
                        | ((0xFF - i) << 8) 
                        | (0xFF - i);
    }

    {
        GS_UINT screenScale = 4;
        GS_UINT showWidth = m_screenDev.width * screenScale;
        GS_UINT showHeight = m_screenDev.height * screenScale;

        CRect dlgRect;
        GetWindowRect(dlgRect);
        CRect screenRect;
        m_screen.GetWindowRect(screenRect);
        ScreenToClient(screenRect);

        GS_INT dx = showWidth - screenRect.Width();
        GS_INT dy = showHeight - screenRect.Height();

        dlgRect.right += dx;
        dlgRect.bottom += dy;
        screenRect.right += dx;
        screenRect.bottom += dy;

        MoveWindow(dlgRect);
        m_screen.MoveWindow(screenRect);
    }

    {
        GS_UINT soundBufferSize = m_pSystem->SoundBufferSize();
        m_soundBufferThreshold = soundBufferSize * 3 / 4;

        HRESULT hr = S_OK;
        hr = DirectSoundCreate(NULL, &m_pDirectSound, NULL);
        GS_ASSERT(SUCCEEDED(hr));
        hr = m_pDirectSound->SetCooperativeLevel(m_hWnd, DSSCL_EXCLUSIVE);
        GS_ASSERT(SUCCEEDED(hr));

        WAVEFORMATEX waveFormatEx;
        gs_zero(&waveFormatEx, sizeof(WAVEFORMATEX));
        waveFormatEx.wFormatTag      = WAVE_FORMAT_PCM;
        waveFormatEx.nChannels       = 1;
        waveFormatEx.nSamplesPerSec  = m_pSystem->SoundSamplingRate();
        waveFormatEx.wBitsPerSample  = 8;
        waveFormatEx.nBlockAlign     = waveFormatEx.nChannels * waveFormatEx.wBitsPerSample / 8;
        waveFormatEx.nAvgBytesPerSec = waveFormatEx.nSamplesPerSec * waveFormatEx.nBlockAlign;

        DSBUFFERDESC dsBufferDesc;
        gs_zero(&dsBufferDesc, sizeof(DSBUFFERDESC));
        dsBufferDesc.dwSize        = sizeof(DSBUFFERDESC);
        dsBufferDesc.dwFlags       = DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS;
        dsBufferDesc.dwBufferBytes = soundBufferSize;
        dsBufferDesc.lpwfxFormat   = &waveFormatEx;

        hr = m_pDirectSound->CreateSoundBuffer(&dsBufferDesc, &m_pDirectSoundBuffer, NULL);
        GS_ASSERT(SUCCEEDED(hr));
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPokeMiniEmulatorDlg::OnCancel()
{
    OnRomClose();
    SafeFreePointer(m_screenDev.pData);
    SafeFreePointer(m_dev[1].pData);
    SafeFreePointer(m_dev[0].pData);
    GsReleasePokeMiniEmuHandle(m_pSystem);

    if (GS_NULL != m_pDirectSoundBuffer) { m_pDirectSoundBuffer->Release(); }
    if (GS_NULL != m_pDirectSound) { m_pDirectSound->Release(); }

    CDialogEx::OnCancel();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPokeMiniEmulatorDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
        PaintScreen();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPokeMiniEmulatorDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

BOOL CPokeMiniEmulatorDlg::PreTranslateMessage(MSG* pMsg)
{
    if (GS_NULL == pMsg)
    {
        return FALSE;
    }

    switch (pMsg->message)
    {
    case WM_KEYDOWN:
        if (OnKeyDown(pMsg))
        {
            return TRUE;
        }
        break;
    case WM_KEYUP:
        if (OnKeyUp(pMsg))
        {
            return TRUE;
        }
        break;
    default:
        break;
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

GS_UINT CPokeMiniEmulatorDlg::EmulateFrame()
{
    return m_pSystem->EmulateFrame();
}

GS_VOID CPokeMiniEmulatorDlg::RenderScreen()
{
    m_pSystem->Render((GS_BYTE *)(m_pRenderDev->pData), m_pRenderDev->rowBytes);
    gs_swap(m_pRenderDev, m_pPaintDev);
}

GS_VOID CPokeMiniEmulatorDlg::PaintScreen()
{
    const GS_BYTE *pSrcRow = (const GS_BYTE *)m_pPaintDev->pData;
    GS_BYTE *pDstRow = (GS_BYTE *)m_screenDev.pData;
    for (GS_UINT h = 0; h < m_screenDev.height; ++h)
    {
        const GS_BYTE *pSrc = pSrcRow;
        GS_BYTE *pDst = pDstRow;
        for (GS_UINT w = 0; w < m_screenDev.width; ++w)
        {
            memcpy(pDst, &(m_colorTable[*pSrc]), 4);
            pSrc += 1;
            pDst += 4;
        }

        pSrcRow += m_pPaintDev->rowBytes;
        pDstRow += m_screenDev.rowBytes;
    }

    {
        CDC *pDC = m_screen.GetDC();
        CRect dcRect;
        m_screen.GetWindowRect(&dcRect);

        CBitmap newbmp;
        newbmp.CreateBitmap(m_screenDev.width, m_screenDev.height, 1, 32, m_screenDev.pData);

        CDC srcDC;
        srcDC.CreateCompatibleDC(pDC);
        CBitmap *pOldBmp = srcDC.SelectObject(&newbmp);

        pDC->StretchBlt(0, 0, dcRect.Width(), dcRect.Height(), &srcDC, 0, 0, m_screenDev.width, m_screenDev.height, SRCCOPY);

        srcDC.SelectObject(pOldBmp);
        srcDC.DeleteDC();
        newbmp.DeleteObject();
        m_screen.ReleaseDC(pDC);
    }
}

GS_VOID CPokeMiniEmulatorDlg::UpdateSoundBuffer()
{
    GS_UINT dataCount = m_pSystem->SoundDataCount();
    RETURN_ON_FAIL(dataCount > m_soundBufferThreshold);

    DWORD status = 0;
    m_pDirectSoundBuffer->GetStatus(&status);

    // Try to restore
    if (status & DSBSTATUS_BUFFERLOST)
    {
        while (DSERR_BUFFERLOST == m_pDirectSoundBuffer->Restore())
        {
            Sleep(10);
        }
    }

    LPVOID pBuffer1 = GS_NULL;
    DWORD bufferSize1 = 0;
    LPVOID pBuffer2 = GS_NULL;
    DWORD bufferSize2 = 0;
    m_pDirectSoundBuffer->Lock(0, dataCount, &pBuffer1, &bufferSize1, &pBuffer2, &bufferSize2, DSBLOCK_FROMWRITECURSOR);
    m_pSystem->OutputSound((GS_BYTE *)pBuffer1, bufferSize1);
    m_pSystem->OutputSound((GS_BYTE *)pBuffer2, bufferSize2);
    m_pDirectSoundBuffer->Unlock(pBuffer1, bufferSize1, pBuffer2, bufferSize2);
}

BOOL CPokeMiniEmulatorDlg::OnKeyUp(MSG* pMsg)
{
    switch (pMsg->wParam)
    {
    case 'Q':
        return TRUE;
    case 'W':
        return TRUE;
    default:
        break;
    }

    if (EMU_RUN == m_state)
    {
        switch (pMsg->wParam)
        {
        case VK_UP:
            m_pSystem->OnKeyUp(IGsPmSystem::PM_KEY_UP);
            return TRUE;
        case VK_DOWN:
            m_pSystem->OnKeyUp(IGsPmSystem::PM_KEY_DOWN);
            return TRUE;
        case VK_LEFT:
            m_pSystem->OnKeyUp(IGsPmSystem::PM_KEY_LEFT);
            return TRUE;
        case VK_RIGHT:
            m_pSystem->OnKeyUp(IGsPmSystem::PM_KEY_RIGHT);
            return TRUE;
        case 'X':
            m_pSystem->OnKeyUp(IGsPmSystem::PM_KEY_A);
            return TRUE;
        case 'Z':
            m_pSystem->OnKeyUp(IGsPmSystem::PM_KEY_B);
            return TRUE;
        case 'C':
            m_pSystem->OnKeyUp(IGsPmSystem::PM_KEY_C);
            return TRUE;
        case 'E':
            m_pSystem->OnKeyUp(IGsPmSystem::PM_KEY_POWER);
            return TRUE;
        case 'A':
            return TRUE;
        default:
            break;
        }
    }

    return FALSE;
}

BOOL CPokeMiniEmulatorDlg::OnKeyDown(MSG* pMsg)
{
    switch (pMsg->wParam)
    {
    case 'Q':
        OnRomOpen();
        return TRUE;
    case 'W':
        if (EMU_RUN == m_state)
        {
            m_state = EMU_PAUSE;
            m_pDirectSoundBuffer->Stop();
        }
        else if (EMU_PAUSE == m_state)
        {
            m_state = EMU_RUN;
            m_pDirectSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
        }
        else
        {
        }
        return TRUE;
    default:
        break;
    }

    if (EMU_RUN == m_state)
    {
        switch (pMsg->wParam)
        {
        case VK_UP:
            m_pSystem->OnKeyDown(IGsPmSystem::PM_KEY_UP);
            return TRUE;
        case VK_DOWN:
            m_pSystem->OnKeyDown(IGsPmSystem::PM_KEY_DOWN);
            return TRUE;
        case VK_LEFT:
            m_pSystem->OnKeyDown(IGsPmSystem::PM_KEY_LEFT);
            return TRUE;
        case VK_RIGHT:
            m_pSystem->OnKeyDown(IGsPmSystem::PM_KEY_RIGHT);
            return TRUE;
        case 'X':
            m_pSystem->OnKeyDown(IGsPmSystem::PM_KEY_A);
            return TRUE;
        case 'Z':
            m_pSystem->OnKeyDown(IGsPmSystem::PM_KEY_B);
            return TRUE;
        case 'C':
            m_pSystem->OnKeyDown(IGsPmSystem::PM_KEY_C);
            return TRUE;
        case 'E':
            m_pSystem->OnKeyDown(IGsPmSystem::PM_KEY_POWER);
            return TRUE;
        case 'A':
            m_pSystem->OnShock();
            return TRUE;
        default:
            break;
        }
    }

    return FALSE;
}

GS_VOID CPokeMiniEmulatorDlg::OnRomOpen()
{
    OnRomClose();

    {
        CFileDialog fileDialog(TRUE, _T("*.min"), NULL, OFN_ALLOWMULTISELECT|OFN_HIDEREADONLY|OFN_EXPLORER, _T("pokemon mini rom(*.min)|*.min"));
        if (IDOK == fileDialog.DoModal())
        {
            CString fileName = fileDialog.GetPathName();
            FILE *pFile = _wfopen(fileName, L"rb");
            fseek(pFile, 0, SEEK_END);
            m_romSize = ftell(pFile);
            m_pRomData = GS_MALLOCZ_OBJ_N(GS_BYTE, m_romSize);
            fseek(pFile, 0, SEEK_SET);
            fread(m_pRomData, 1, m_romSize, pFile);
            fclose(pFile);

            m_pSaveData = GS_MALLOCZ_OBJ_N(GS_BYTE, m_pSystem->SaveSize());
        }
    }

    if (m_pSystem->Initialize(m_pRomData, m_romSize, m_pSaveData))
    {
        HRESULT hr = S_OK;
        hr = m_pDirectSoundBuffer->SetCurrentPosition(0);
        GS_ASSERT(SUCCEEDED(hr));
        hr = m_pDirectSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
        GS_ASSERT(SUCCEEDED(hr));

        m_state = EMU_RUN;
        m_emuThread = CreateThread(NULL, 0, EmuThread, this, 0, &m_emuThreadID);
    }
}

GS_VOID CPokeMiniEmulatorDlg::OnRomClose()
{
    if (EMU_STOP != m_state)
    {
        m_state = EMU_STOP;
        WaitForSingleObject(m_emuThread, INFINITE);
        m_pDirectSoundBuffer->Stop();
    }

    if (GS_NULL != m_pSystem)
    {
        m_pSystem->Destroy();
    }

    SafeFreePointer(m_pSaveData);
    SafeFreePointer(m_pRomData);
}

DWORD CPokeMiniEmulatorDlg::EmuThread(LPVOID lpThreadParameter)
{
    CPokeMiniEmulatorDlg *pEmu = (CPokeMiniEmulatorDlg *)lpThreadParameter;
    EMU_STATE &state = pEmu->m_state;

    DWORD time = 0;
    timeBeginPeriod(1);
    while (EMU_STOP != state)
    {
        if (EMU_RUN == state)
        {
            // Time Control
            time = timeGetTime();

            // Run
            GS_UINT cycles = pEmu->EmulateFrame();
            DWORD timeAdd = 1000 * cycles / pEmu->m_cpuRate;
            DWORD timeEnd = time + timeAdd;

            // Vedio Control
            pEmu->RenderScreen();
            pEmu->Invalidate(FALSE);

            // Time Control
            while (time < timeEnd)
            {
                Sleep(1);
                time = timeGetTime();
            }

            // Audio Control
            pEmu->UpdateSoundBuffer();
        }
        else if (EMU_PAUSE == state)
        {
            Sleep(250);
        }
        else
        {
            GS_ASSERT(GS_FALSE);
        }
    }

    timeEndPeriod(1);
    return 0;
}
