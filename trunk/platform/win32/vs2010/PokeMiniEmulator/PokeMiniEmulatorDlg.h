#pragma once

#include "GsPokeMiniApi.h"
#include "KernelDef.h"
#include "afxwin.h"
#include <MMSystem.h>
#include <dsound.h>

#pragma comment(lib, "dsound.lib")

// CPokeMiniEmulatorDlg dialog
class CPokeMiniEmulatorDlg : public CDialogEx
{
// Construction
public:
    CPokeMiniEmulatorDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    enum { IDD = IDD_POKEMINIEMULATOR_DIALOG };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
    GS_UINT EmulateFrame();
    GS_VOID RenderScreen();
    GS_VOID PaintScreen();

    GS_VOID UpdateSoundBuffer();

    BOOL OnKeyUp(MSG* pMsg);
    BOOL OnKeyDown(MSG* pMsg);
    GS_VOID OnOpen();
    GS_VOID OnClose();

private:
    static DWORD WINAPI EmuThread(LPVOID lpThreadParameter);

private:
    enum EMU_STATE
    {
        EMU_STOP, 
        EMU_PAUSE, 
        EMU_RUN, 
    };

private:
    IGsPmSystem * m_pSystem;
    GS_UINT m_cpuRate;
    EMU_STATE m_state;

    GsRenderDevice m_dev[2];
    GsRenderDevice * m_pRenderDev;
    GsRenderDevice * m_pPaintDev;
    GsRenderDevice m_screenDev;
    GS_UINT32 m_colorTable[256];
    CStatic m_screen;

    IDirectSound * m_pDirectSound;
    IDirectSoundBuffer * m_pDirectSoundBuffer;
    GS_UINT m_soundBufferThreshold; // To make sound smooth

    GS_BYTE * m_pRomData;
    GS_UINT m_romSize;
    GS_BYTE * m_pSaveData;

    HANDLE m_emuThread;
    DWORD m_emuThreadID;
};
