//------------------------------------------------------------------------------
// File: guitarhero.h
//
// Desc: DirectShow Guitar Hero Robot - special effects filter header file.
//
// Author: Rafael Mizrahi
// http://www.garagegeeks.org
//------------------------------------------------------------------------------

#include <sys/timeb.h>	// get time milliseconds
#include <time.h>	/* for the 'time' structure     */
#include "stack.h"

// output types
#define OUTPUT_TYPE_FILE	0	// output to file
#define OUTPUT_TYPE_SOCK	1	// output to TCPIP socket
#define OUTPUT_TYPE_PORT	2	// output to parallel

class CGuitarHero : public CTransformFilter,
         public IIPEffect,
         public ISpecifyPropertyPages,
         public CPersistStream
{

public:

    DECLARE_IUNKNOWN;
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

    // Reveals ISpecifyPropertyPages
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

    // CPersistStream stuff
    HRESULT ScribbleToStream(IStream *pStream);
    HRESULT ReadFromStream(IStream *pStream);

    // Overrriden from CTransformFilter base class

    HRESULT Transform(IMediaSample *pIn, IMediaSample *pOut);
    HRESULT CheckInputType(const CMediaType *mtIn);
    HRESULT CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut);
    HRESULT DecideBufferSize(IMemAllocator *pAlloc,
                             ALLOCATOR_PROPERTIES *pProperties);
    HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);

    // These implement the custom IIPEffect interface

    STDMETHODIMP get_IPEffect(int *IPEffect, int *Threshold, int *pDetection_hit_delay);
    STDMETHODIMP put_IPEffect(int IPEffect, int Threshold, int pDetection_hit_delay);

    // ISpecifyPropertyPages interface
    STDMETHODIMP GetPages(CAUUID *pPages);

    // CPersistStream override
    STDMETHODIMP GetClassID(CLSID *pClsid);

private:

    // Constructor
    CGuitarHero(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);

    // Look after doing the special effect
    BOOL CanPerformGuitarHero(const CMediaType *pMediaType) const;
    HRESULT Copy(IMediaSample *pSource, IMediaSample *pDest) const;
    HRESULT Transform(IMediaSample *pMediaSample);

	// BOOL IsRegion(int x, int y, BYTE *pData);
	
	// color functions
	void RGBtoHSV(int R, int G, int B, int* H, int* S, int* V);
	bool is_green(unsigned char r, unsigned char g, unsigned char b);
	bool is_red(unsigned char r, unsigned char g, unsigned char b);
	bool is_yellow(unsigned char r, unsigned char g, unsigned char b);

	//
	// robot socket server
	//
	void CGuitarHero::ConnectToRobotServer();
	void CGuitarHero::DisconnectFromRobotServer();

	//
	// plateN_status are int and not bool, 
	//		because it is also used for tracing other values to file.
	// output_type: 0 file, 1 socket, 2 parallel
	void CGuitarHero::OutputToRobot(int plate1_status, 
								int plate2_status, 
								int plate3_status, 
								int plate4_status, 
								int plate5_status,
								bool strum,
								int output_type);

    CCritSec    m_GuitarHeroLock;       // Private play critical section
    const long m_lBufferRequest;        // The number of buffers to use
    int         m_effect;               // Which effect are we processing
	int			m_effectThreshold;		// Binary Threshold
	bool		m_verbose_to_screen;	// print to screen the trapezoids

	int m_plates_y;						// plates y position 
	int m_plate1_x1;					// plate 1 x1
	int m_plate2_x1;					// 
	int m_plate3_x1;					// 
	int m_plate4_x1;					// 
	int m_plate5_x1;					// 
	int m_plates_width;					// plate width
	int m_plates_height;				// plate height
	
	//TODO: capture line in order to estimate the BPM of the song
	// this can improve by auto calibrating the delays
	int m_plates_line_y;				//
	int m_plates_line_width;			//
	int m_plate1_line_x;				// plate 1 line x position
	int m_plate2_line_x;				//
	int m_plate3_line_x;				//
	int m_plate4_line_x;				//
	int m_plate5_line_x;				//

	// sync timers
	//
	// In order to avoid multiple detection of the same plate.
	// On every detection (is_detected = true), wait about 100 millisecond
	bool is_detected;
	struct _timeb m_time_current;
	struct _timeb m_time_previous_detection;
	struct _timeb m_time_previous_hit_delay;

	//
	// Delay - The plates detection is far from the hit location.
	//
	// adding a FIFO Stack which stores the plates and sends them to 
	// the robot when time (m_detection_hit_delay) has come.
	//TODO: m_detection_hit_delay is a configuration parameter. detect the horizontal lines when 
	// song starts, and adjust the time parameter to the bpm of the song.
	int m_detection_hit_delay;
	stack m_plates_stack;

	// TCP/IP members to connect to Robot Parallel server.
	SOCKET clisock;
	sockaddr_in cli;
}; 

