//------------------------------------------------------------------------------
// File: GuitarHeroProp.h
//
// Desc: DirectShow Guitar Hero Robot - definition of guitar hero Properties class.
//
// Author: Rafael Mizrahi
// http://www.garagegeeks.org
//------------------------------------------------------------------------------


class CGuitarHeroProperties : public CBasePropertyPage
{

public:

    static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

private:

    INT_PTR OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
    HRESULT OnConnect(IUnknown *pUnknown);
    HRESULT OnDisconnect();
    HRESULT OnActivate();
    HRESULT OnDeactivate();
    HRESULT OnApplyChanges();

    void    GetControlValues();

    CGuitarHeroProperties(LPUNKNOWN lpunk, HRESULT *phr);

    BOOL m_bIsInitialized;      // Used to ignore startup messages
    int m_effect;               // Which effect are we processing
	int m_threshold;            // Binary Threshold
	int m_detection_hit_delay;
    //REFTIME m_start;            // When the effect will begin
    //REFTIME m_length;           // And how long it will last for
    IIPEffect *m_pIPEffect;     // The custom interface on the filter

}; // GuitarHeroProperties

