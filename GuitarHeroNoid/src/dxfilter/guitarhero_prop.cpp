//------------------------------------------------------------------------------
// File: GuitarHero_Prop.cpp
//
// Desc: DirectShow Guitar Hero Robot - implementation of CGuitarHeroProperties class.
//
// Author: Rafael Mizrahi
// http://www.garagegeeks.org
//------------------------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>
#include <streams.h>
#include <commctrl.h>
#include <olectl.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include "resource.h"
#include "guitarhero_guids.h"
#include "igh.h"
#include "guitarhero.h"
#include "guitarheroprop.h"


//
// CreateInstance
//
// Used by the DirectShow base classes to create instances
//
CUnknown *CGuitarHeroProperties::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
    ASSERT(phr);

    CUnknown *punk = new CGuitarHeroProperties(lpunk, phr);

    if (punk == NULL) {
        if (phr)
        	*phr = E_OUTOFMEMORY;
    }

    return punk;

} // CreateInstance


//
// Constructor
//
CGuitarHeroProperties::CGuitarHeroProperties(LPUNKNOWN pUnk, HRESULT *phr) :
    CBasePropertyPage(NAME("Special Effects Property Page"), pUnk,
                      IDD_GuitarHeroPROP, IDS_TITLE),
    m_pIPEffect(NULL),
    m_bIsInitialized(FALSE)
{
    ASSERT(phr);

} // (Constructor)


//
// OnReceiveMessage
//
// Handles the messages for our property window
//
INT_PTR CGuitarHeroProperties::OnReceiveMessage(HWND hwnd,
                                          UINT uMsg,
                                          WPARAM wParam,
                                          LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_COMMAND:
        {
            if (m_bIsInitialized)
            {
                m_bDirty = TRUE;
                if (m_pPageSite)
                {
                    m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
                }
            }
            return (LRESULT) 1;
        }

    }

    return CBasePropertyPage::OnReceiveMessage(hwnd,uMsg,wParam,lParam);

} // OnReceiveMessage


//
// OnConnect
//
// Called when we connect to a transform filter
//
HRESULT CGuitarHeroProperties::OnConnect(IUnknown *pUnknown)
{
    CheckPointer(pUnknown,E_POINTER);
    ASSERT(m_pIPEffect == NULL);

    HRESULT hr = pUnknown->QueryInterface(IID_IIPEffect, (void **) &m_pIPEffect);
    if (FAILED(hr)) {
        return E_NOINTERFACE;
    }

    // Get the initial image FX property
    CheckPointer(m_pIPEffect,E_FAIL);
    m_pIPEffect->get_IPEffect(&m_effect, &m_threshold, &m_detection_hit_delay);

    m_bIsInitialized = FALSE ;
    return NOERROR;

} // OnConnect


//
// OnDisconnect
//
// Likewise called when we disconnect from a filter
//
HRESULT CGuitarHeroProperties::OnDisconnect()
{
    // Release of Interface after setting the appropriate old effect value
    if(m_pIPEffect)
    {
        m_pIPEffect->Release();
        m_pIPEffect = NULL;
    }
    return NOERROR;

} // OnDisconnect


//
// OnActivate
//
// We are being activated
//
HRESULT CGuitarHeroProperties::OnActivate()
{
    TCHAR sz[60];

    //_stprintf(sz, TEXT("%f\0"), m_length);
    //Edit_SetText(GetDlgItem(m_Dlg, IDC_LENGTH), sz);

    //_stprintf(sz, TEXT("%f\0"), m_start);
    //Edit_SetText(GetDlgItem(m_Dlg, IDC_START), sz);

	_stprintf(sz, TEXT("%d\0"), m_threshold);
    Edit_SetText(GetDlgItem(m_Dlg, IDC_THRESHOLD), sz);
	
	_stprintf(sz, TEXT("%d\0"), m_detection_hit_delay);
    Edit_SetText(GetDlgItem(m_Dlg, IDC_STRUM_DELAY), sz);

	CheckRadioButton(m_Dlg, IDC_GUITAR, IDC_NONE, m_effect);
    m_bIsInitialized = TRUE;

    return NOERROR;

} // OnActivate


//
// OnDeactivate
//
// We are being deactivated
//
HRESULT CGuitarHeroProperties::OnDeactivate(void)
{
    ASSERT(m_pIPEffect);

    m_bIsInitialized = FALSE;
    GetControlValues();

    return NOERROR;

} // OnDeactivate


//
// OnApplyChanges
//
// Apply any changes so far made
//
HRESULT CGuitarHeroProperties::OnApplyChanges()
{
    GetControlValues();

    CheckPointer(m_pIPEffect,E_POINTER);
    m_pIPEffect->put_IPEffect(m_effect, m_threshold, m_detection_hit_delay);

    return NOERROR;

} // OnApplyChanges


void CGuitarHeroProperties::GetControlValues()
{
    TCHAR sz[STR_MAX_LENGTH];
//    REFTIME tmp1, tmp2 ;
	int tmp3,tmp4;

//    // Get the start and effect times
//    Edit_GetText(GetDlgItem(m_Dlg, IDC_LENGTH), sz, STR_MAX_LENGTH);
//#ifdef UNICODE
//    // Convert Multibyte string to ANSI
//    char szANSI[STR_MAX_LENGTH];
//
//    int rc = WideCharToMultiByte(CP_ACP, 0, sz, -1, szANSI, STR_MAX_LENGTH, NULL, NULL);
//    tmp2 = COARefTime(atof(szANSI));
//#else
//    tmp2 = COARefTime(atof(sz));
//#endif

//    Edit_GetText(GetDlgItem(m_Dlg, IDC_START), sz, STR_MAX_LENGTH);
//#ifdef UNICODE
//    // Convert Multibyte string to ANSI
//    rc = WideCharToMultiByte(CP_ACP, 0, sz, -1, szANSI, STR_MAX_LENGTH, NULL, NULL);
//    tmp1 = COARefTime(atof(szANSI));
//#else
//    tmp1 = COARefTime(atof(sz));
//#endif

    Edit_GetText(GetDlgItem(m_Dlg, IDC_THRESHOLD), sz, STR_MAX_LENGTH);
#ifdef UNICODE
    // Convert Multibyte string to ANSI
    rc = WideCharToMultiByte(CP_ACP, 0, sz, -1, szANSI, STR_MAX_LENGTH, NULL, NULL);
    tmp3 = = atoi(szANSI);
#else
    tmp3 = atoi(sz);
#endif

    Edit_GetText(GetDlgItem(m_Dlg, IDC_STRUM_DELAY), sz, STR_MAX_LENGTH);
#ifdef UNICODE
    // Convert Multibyte string to ANSI
    rc = WideCharToMultiByte(CP_ACP, 0, sz, -1, szANSI, STR_MAX_LENGTH, NULL, NULL);
    tmp4 = = atoi(szANSI);
#else
    tmp4 = atoi(sz);
#endif

	//
	// validation of fields
	//

    //if (tmp1 >= 0 && tmp2 >= 0) {
    //    m_start  = tmp1;
    //    m_length = tmp2;
    //}

	if (tmp3 >=0 && tmp3 <= 255)
	{
		m_threshold = tmp3;
	}

	if (tmp4 >=0 && tmp4 <= 1000)
	{
		m_detection_hit_delay = tmp4;
	}

    // Find which special effect we have selected

    for (int i = IDC_GUITAR; i <= IDC_NONE; i++) 
    {
        if (IsDlgButtonChecked(m_Dlg, i)) 
        {
            m_effect = i;
            break;
        }
    }
}
