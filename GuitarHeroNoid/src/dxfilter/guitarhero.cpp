//------------------------------------------------------------------------------
// File: guitarhero.cpp
//
// Desc: DirectShow Guitar Hero Robot -  filter.
//
// Author: Rafael Mizrahi
// http://www.garagegeeks.org
//------------------------------------------------------------------------------

//
// Files
//
// guitarhero_prop.cpp      A property page to control the video effects
// guitarheroprop.h         Class definition for the property page object
// guitarhero_prop.rc       Dialog box template for the property page
// guitarhero.cpp			Main filter code that does the special effects
// guitarhero.def			What APIs we import and export from this DLL
// guitarhero.h				Class definition for the special effects filter
// guitarhero_guids.h		Header file containing the filter CLSIDs
// igh.h					Defines the special effects custom interface
// resource.h				Microsoft Visual C++ generated resource file
//
//
// Base classes used
//
// CTransformFilter     A transform filter with one input and output pin
// CPersistStream       Handles the grunge of supporting IPersistStream
//
//


#include <windows.h>
#include <streams.h>
#include <initguid.h>

#include <stdio.h>		// for FILE

#if (1100 > _MSC_VER)
#include <olectlid.h>
#else
#include <olectl.h>
#endif

#include "guitarhero_guids.h"
#include "igh.h"
#include "guitarheroprop.h"
#include "guitarhero.h"
#include "resource.h"

#include "inpout32.h"

// Setup information

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
    &MEDIATYPE_Video,       // Major type
    &MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN sudpPins[] =
{
    { L"Input",             // Pins string name
      FALSE,                // Is it rendered
      FALSE,                // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    },
    { L"Output",            // Pins string name
      FALSE,                // Is it rendered
      TRUE,                 // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    }
};

const AMOVIESETUP_FILTER sudGuitarHero =
{
    &CLSID_GuitarHero,      // Filter CLSID
    L"GuitarHero",			// String name
    MERIT_DO_NOT_USE,       // Filter merit
    2,                      // Number of pins
    sudpPins                // Pin information
};


// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance

CFactoryTemplate g_Templates[] = {
    { L"Image Effects"
    , &CLSID_GuitarHero
    , CGuitarHero::CreateInstance
    , NULL
    , &sudGuitarHero }
  ,
    { L"Special Effects"
    , &CLSID_GuitarHeroPropertyPage
    , CGuitarHeroProperties::CreateInstance }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


////////////////////////////////////////////////////////////////////////
//
// Exported entry points for registration and unregistration 
// (in this case they only call through to default implementations).
//
////////////////////////////////////////////////////////////////////////

//
// DllRegisterServer: Handles sample registry and unregistry
//
STDAPI DllRegisterServer()
{
    return AMovieDllRegisterServer2( TRUE );
}

//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
    return AMovieDllRegisterServer2( FALSE );
}

//
// DllEntryPoint
//
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  dwReason, 
                      LPVOID lpReserved)
{
	return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}


//
// Constructor
//
CGuitarHero::CGuitarHero(TCHAR *tszName,
                   LPUNKNOWN punk,
                   HRESULT *phr) :
    CTransformFilter(tszName, punk, CLSID_GuitarHero),
    m_effect(IDC_GUITAR_V),
    m_lBufferRequest(1),
    CPersistStream(punk, phr)
{
    char sz[60];

	//
	//	trapezoids location to capture the plates
	//

	// trapezoids y location
	//
	GetPrivateProfileStringA("guitarhero", "plates_y", "156.0", sz, 60, "guitarhero.ini");
    m_plates_y = atoi(sz);


	//	trapezoids width and height
	//
	GetPrivateProfileStringA("guitarhero", "plates_width", "44.0", sz, 60, "guitarhero.ini");
    m_plates_width = atoi(sz);
	GetPrivateProfileStringA("guitarhero", "plates_height", "16.0", sz, 60, "guitarhero.ini");
    m_plates_height = atoi(sz);
	
	//	trapezoids base left location 
	//
	GetPrivateProfileStringA("guitarhero", "plate1_x1", "156.0", sz, 60, "guitarhero.ini");
    m_plate1_x1 = atoi(sz);
	GetPrivateProfileStringA("guitarhero", "plate2_x1", "194.0", sz, 60, "guitarhero.ini");
    m_plate2_x1 = atoi(sz);
	GetPrivateProfileStringA("guitarhero", "plate3_x1", "230.0", sz, 60, "guitarhero.ini");
    m_plate3_x1 = atoi(sz);
	GetPrivateProfileStringA("guitarhero", "plate4_x1", "264.0", sz, 60, "guitarhero.ini");
    m_plate4_x1 = atoi(sz);
	GetPrivateProfileStringA("guitarhero", "plate5_x1", "300.0", sz, 60, "guitarhero.ini");
    m_plate5_x1 = atoi(sz);

	//
	//	rects to capture the lines
	//

	//GetPrivateProfileStringA("guitarhero", "plates_line_y", "80.0", sz, 60, "guitarhero.ini");
	//m_plates_line_y = atoi(sz);
	//GetPrivateProfileStringA("guitarhero", "plates_line_width", "40.0", sz, 60, "guitarhero.ini");
	//m_plates_line_width = atoi(sz);

	//GetPrivateProfileStringA("guitarhero", "plate1_line_x", "320.0", sz, 60, "guitarhero.ini");
	//m_plate1_line_x = atoi(sz);
	//GetPrivateProfileStringA("guitarhero", "plate2_line_x", "320.0", sz, 60, "guitarhero.ini");
	//m_plate2_line_x = atoi(sz);
	//GetPrivateProfileStringA("guitarhero", "plate3_line_x", "320.0", sz, 60, "guitarhero.ini");
	//m_plate3_line_x = atoi(sz);
	//GetPrivateProfileStringA("guitarhero", "plate4_line_x", "320.0", sz, 60, "guitarhero.ini");
	//m_plate4_line_x = atoi(sz);
	//GetPrivateProfileStringA("guitarhero", "plate5_line_x", "320.0", sz, 60, "guitarhero.ini");
	//m_plate5_line_x = atoi(sz);

	//	binary threshold
	m_verbose_to_screen = false;
    m_effectThreshold = 120;
	m_detection_hit_delay = 250; // milliseconds

	// connecting to socket server.
	ConnectToRobotServer();
}

//
// CreateInstance
//
// Provide the way for COM to create a GuitarHero object
//
CUnknown *CGuitarHero::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
    ASSERT(phr);
    
    CGuitarHero *pNewObject = new CGuitarHero(NAME("Image Effects"), punk, phr);

    if (pNewObject == NULL) {
        if (phr)
            *phr = E_OUTOFMEMORY;
    }
    return pNewObject;

} // CreateInstance


//
// NonDelegatingQueryInterface: Reveals IIPEffect and ISpecifyPropertyPages
//
STDMETHODIMP CGuitarHero::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);

    if (riid == IID_IIPEffect) {
        return GetInterface((IIPEffect *) this, ppv);

    } else if (riid == IID_ISpecifyPropertyPages) {
        return GetInterface((ISpecifyPropertyPages *) this, ppv);

    } else {
        return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
    }
}


//
// Transform
//
// Copy the input sample into the output sample - then transform the output
// sample 'in place'. If we have all keyframes, then we shouldn't do a copy
// If we have cinepak or indeo and are decompressing frame N it needs frame
// decompressed frame N-1 available to calculate it, unless we are at a
// keyframe. So with keyframed codecs, you can't get away with applying the
// transform to change the frames in place, because you'll mess up the next
// frames decompression. The runtime MPEG decoder does not have keyframes in
// the same way so it can be done in place. We know if a sample is key frame
// as we transform because the sync point property will be set on the sample
//
HRESULT CGuitarHero::Transform(IMediaSample *pIn, IMediaSample *pOut)
{
    CheckPointer(pIn,E_POINTER);   
    CheckPointer(pOut,E_POINTER);   

    // Copy the properties across
    HRESULT hr = Copy(pIn, pOut);
    if (FAILED(hr)) {
        return hr;
    }

    return Transform(pOut);
}


//
// Copy: Make destination an identical copy of source
//
HRESULT CGuitarHero::Copy(IMediaSample *pSource, IMediaSample *pDest) const
{
    CheckPointer(pSource,E_POINTER);   
    CheckPointer(pDest,E_POINTER);   

    // Copy the sample data

    BYTE *pSourceBuffer, *pDestBuffer;
    long lSourceSize = pSource->GetActualDataLength();

#ifdef DEBUG
    long lDestSize = pDest->GetSize();
    ASSERT(lDestSize >= lSourceSize);
#endif

    pSource->GetPointer(&pSourceBuffer);
    pDest->GetPointer(&pDestBuffer);

    CopyMemory( (PVOID) pDestBuffer,(PVOID) pSourceBuffer,lSourceSize);

    // Copy the sample times

    REFERENCE_TIME TimeStart, TimeEnd;
    if (NOERROR == pSource->GetTime(&TimeStart, &TimeEnd)) {
        pDest->SetTime(&TimeStart, &TimeEnd);
    }

    LONGLONG MediaStart, MediaEnd;
    if (pSource->GetMediaTime(&MediaStart,&MediaEnd) == NOERROR) {
        pDest->SetMediaTime(&MediaStart,&MediaEnd);
    }

    // Copy the Sync point property

    HRESULT hr = pSource->IsSyncPoint();
    if (hr == S_OK) {
        pDest->SetSyncPoint(TRUE);
    }
    else if (hr == S_FALSE) {
        pDest->SetSyncPoint(FALSE);
    }
    else {  // an unexpected error has occured...
        return E_UNEXPECTED;
    }

    // Copy the media type

    AM_MEDIA_TYPE *pMediaType;
    pSource->GetMediaType(&pMediaType);
    pDest->SetMediaType(pMediaType);
    DeleteMediaType(pMediaType);

    // Copy the preroll property

    hr = pSource->IsPreroll();
    if (hr == S_OK) {
        pDest->SetPreroll(TRUE);
    }
    else if (hr == S_FALSE) {
        pDest->SetPreroll(FALSE);
    }
    else {  // an unexpected error has occured...
        return E_UNEXPECTED;
    }

    // Copy the discontinuity property

    hr = pSource->IsDiscontinuity();
    if (hr == S_OK) {
    pDest->SetDiscontinuity(TRUE);
    }
    else if (hr == S_FALSE) {
        pDest->SetDiscontinuity(FALSE);
    }
    else {  // an unexpected error has occured...
        return E_UNEXPECTED;
    }

    // Copy the actual data length

    long lDataLength = pSource->GetActualDataLength();
    pDest->SetActualDataLength(lDataLength);

    return NOERROR;
}

//
// Transform (in place)
//
// 'In place' apply the image effect to this sample
//
HRESULT CGuitarHero::Transform(IMediaSample *pMediaSample)
{
    BYTE *pData;                // Pointer to the actual image buffer
    long lDataLen;              // Holds length of any given sample
    int x,y;					// loop counters for transforms
    RGBTRIPLE *prgb;            // Holds a pointer to the current pixel
	RGBTRIPLE *prgb_x_start;	

    AM_MEDIA_TYPE* pType = &m_pInput->CurrentMediaType();
    VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pType->pbFormat;
    ASSERT(pvi);

    CheckPointer(pMediaSample,E_POINTER);
    pMediaSample->GetPointer(&pData);
    lDataLen = pMediaSample->GetSize();

    // Get the image properties from the BITMAPINFOHEADER
    int cxImage    = pvi->bmiHeader.biWidth;
    //int cyImage    = pvi->bmiHeader.biHeight;
    //int numPixels  = cxImage * cyImage;
    // int iPixelSize = pvi->bmiHeader.biBitCount / 8;
    // int cbImage    = cyImage * cxImage * iPixelSize;

	int min = m_effectThreshold;
	
	int in_plate = 0;
	int play_plate = PLATE_NONE; // bit Or operator of 5 plates 0,1,2,4,8,16
	int in_line = 0;
	double f = 0;
	int white_sum = 0;			// sum of white at the plate position.
	int white_sum_line = 0;		// sum of white at the line position.
	int max = 255;				

	// fill the array with plates x positions.
	int plates_position[5] = {m_plate1_x1,m_plate2_x1,m_plate3_x1,m_plate4_x1,m_plate5_x1};
	//int lines_position[5] = {m_plate1_line_x,m_plate2_line_x,m_plate3_line_x,m_plate4_line_x,m_plate5_line_x};
#if DEBUG
	int plates_debug[5];	// store values for debug output to file.
#endif
	int R,G,B,H,S,V,Y,U;

	// print detection and output action to screen
	if (m_effect == IDC_GUITAR_V) 
		m_verbose_to_screen = true;
	else
		m_verbose_to_screen = false;

	////////////////////////////////////////
	//
	// timers for sync
	//
	// get current time for each frame.
	double d_time_current = 0, d_time_previous_detection = 0, d_time_previous_hit_delay = 0;
	_ftime(&m_time_current);
	d_time_current = m_time_current.time + (double) m_time_current.millitm / 1000;
	d_time_previous_detection = m_time_previous_detection.time + (double) m_time_previous_detection.millitm / 1000;
	d_time_previous_hit_delay = m_time_previous_hit_delay.time + (double) m_time_previous_hit_delay.millitm / 1000;

	////////////////////////////////////////
	//
	// Pop from plates stack and send to robot if time has come.
	// note: it is done before 'Multiple detection delay', because 'Multiple detection relay'
	// exists the function.
	//
	if ((d_time_current - d_time_previous_hit_delay > ((double)m_detection_hit_delay/1000)))
	{
		play_plate = m_plates_stack.pop();
		// should play only if one of the plates is true. which is play_plate >0
		if (play_plate != PLATE_NONE)
		{
			OutputToRobot(((play_plate & PLATE_1) == PLATE_1),
						((play_plate & PLATE_2) == PLATE_2),
						((play_plate & PLATE_3) == PLATE_3),
						((play_plate & PLATE_4) == PLATE_4),
						((play_plate & PLATE_5) == PLATE_5),
						1 , OUTPUT_TYPE_PORT);

			//
			//add a visual indication that a plate was hit.
			//
			if (m_verbose_to_screen == true)
			{
				prgb = (RGBTRIPLE*) pData;
				//FIXME: y - 100 is not the in the right x position
				prgb = prgb + (m_plates_y - 100) * (pvi->bmiHeader.biWidth);
				prgb_x_start = prgb;
				for (int in_plate=0;in_plate<5;in_plate++)
				{
					if (!((((play_plate & PLATE_1) == PLATE_1) && (in_plate == 0)) ||
						(((play_plate & PLATE_2) == PLATE_2) && (in_plate == 1)) ||
						(((play_plate & PLATE_3) == PLATE_3) && (in_plate == 2)) ||
						(((play_plate & PLATE_4) == PLATE_4) && (in_plate == 3)) ||
						(((play_plate & PLATE_5) == PLATE_5))&& (in_plate == 4)) )
						continue;

					prgb = prgb_x_start + plates_position[in_plate];
					f = 0; 
					for (int i=0;i<m_plates_height;i++)
					{
						for (int j=0;j<m_plates_width-f*2;j++) 
						{
							prgb[j+i*cxImage+(int)f].rgbtRed   = (BYTE) 255;
							prgb[j+i*cxImage+(int)f].rgbtGreen = (BYTE) 255;
							prgb[j+i*cxImage+(int)f].rgbtBlue = (BYTE) 0;
						}
						f = f + 1;
					}
				}
			}//m_verbose_to_screen
		}
	
		// reset previous as current
		_ftime(&m_time_previous_hit_delay);
	}


	////////////////////////////////////////
	//
	// Multiple detection delay
	//
	// every plate is detected multiple times.
	// in order to avoid sending multiple hits to the robot, when a plate (or more) is detected,
	// we ignore detections for 150 milliseconds after a detection.
	//
	//TODO: move this parameter to the dialog
	if ((is_detected == true) && (d_time_current - d_time_previous_detection < 0.15))
	{
		return NOERROR;
	}

	is_detected = false;
	// reset previous as current
	_ftime(&m_time_previous_detection); // almost as: m_time_previous_detection = m_time_current;
	//
	////////////////////////////////////////

	switch (m_effect)
    {
        case IDC_NONE: 
			break;
		case IDC_GUITAR_V:
			m_verbose_to_screen = true;
			//no breaking here; going straight to IDC_GUITAR
		case IDC_GUITAR:
			
			prgb = (RGBTRIPLE*) pData;
			// go straight to the y position of the places.
			prgb = prgb + m_plates_y * (pvi->bmiHeader.biWidth);
			play_plate = PLATE_NONE; //reset. playing nothing

#if DEBUG
			plates_debug[0]=0;plates_debug[1]=0;plates_debug[2]=0;
			plates_debug[3]=0;plates_debug[4]=0;
#endif
			// go straight to the x position of each plate.
			prgb_x_start = prgb;
			// loop on 5 trapezoids plates locations
			for (in_plate=0;in_plate<5;in_plate++)
			{
				// go to plate location
				prgb = prgb_x_start + plates_position[in_plate];

				white_sum = 0;
				white_sum_line = 0;
				int h_avg = 0;				// avarage of color (H) at the plate position.
				int s_avg = 0;				// avarage of S
				int v_avg = 0;				// avarage of V

				h_avg = 0; 
				int color_avg_counter = 0;	//because we have black interlaced lines
				int trapezoid_size = 0;		//how many pixels in a trapezoid
				f = 0;

				// create a trapezoid shaped object
				for (int i=0;i<m_plates_height;i++)	{
					for (int j=0;j<m_plates_width-f*2;j++)	{

						trapezoid_size++;

						// get current pixel RGB
						R = (int)prgb[j+i*cxImage+(int)f].rgbtRed;
						G = (int)prgb[j+i*cxImage+(int)f].rgbtGreen;
						B = (int)prgb[j+i*cxImage+(int)f].rgbtBlue;

						// HSV calculation
						H=0;S=0;V=0;
						RGBtoHSV(R,G,B,&H,&S,&V);
						
						// Test YUV: doesn't seem as better as HSV
						//Y = (( ( 66 * R + 129 * G + 25 * B + 128) >> 8) + 16);
						//V = Y;

						// threashold V to binary, and sum the white pixels
						byte b = (byte)(((V >= min) && (V <= max)) ? 255 : 0);
						if(b == (BYTE)255)
						{
							white_sum++;

							//
							// get HSV avarages
							// calculate avarage only for real values
							// do not calculate black (interlace) and white
							if (((R == 0) && (G == 0) && (B == 0)) || ((R == 255) && (G == 255) && (B == 255)))
							{
								// do nothing
								//color_avg_counter = color_avg_counter;
							}else
							{
								h_avg = h_avg + H;
								s_avg = s_avg + S;
								v_avg = v_avg + V;
								color_avg_counter++;
							}
						}
					}
					f = f + 1;
				}
				if (color_avg_counter != 0)
				{
					h_avg = h_avg /color_avg_counter;
					s_avg = s_avg /color_avg_counter;
					v_avg = v_avg /color_avg_counter;
				}

				//TODO: improve detection by detecting plates color
				//check if plate_color_in_range
				//TODO: plate_color_in_range should init as false when the color detection is finished
				bool plate_color_in_range = true;
				//if (in_plate+1 == 1) //green
				//{
				//	if ((h_avg > 70) && (h_avg < 120))
				//		plate_color_in_range = true;
				//}
				//if (in_plate+1 == 2) //red
				//{
				//	if (((h_avg > 330) && (h_avg < 360)) || ((h_avg > 0) && (h_avg < 20)))
				//		plate_color_in_range = true;
				//}
				//if (in_plate+1 == 3) //yellow
				//{
				//	if ((h_avg > 40) && (h_avg < 60))
				//		plate_color_in_range = true;
				//}

				// 160-190 is a blue star
				// 60 is yellow
				// 160 is green
				// 

				if ((white_sum > (0.50 * trapezoid_size)) && (plate_color_in_range == true))
				{
					is_detected = true;

					// add the plate to play
					if (in_plate+1 == 1)	play_plate = play_plate | PLATE_1;
					if (in_plate+1 == 2)	play_plate = play_plate | PLATE_2;
					if (in_plate+1 == 3)	play_plate = play_plate | PLATE_3;
					if (in_plate+1 == 4)	play_plate = play_plate | PLATE_4;
					if (in_plate+1 == 5)	play_plate = play_plate | PLATE_5;

#if DEBUG
					plates_debug[in_plate] = white_sum;//white_sum_line; //h_avg;
#endif
					//
					// draw the plate on the screen
					//
					if (m_verbose_to_screen == true)
					{
						f = 0; // f helps to create a trapezoid.
						for (int i=0;i<m_plates_height;i++) {
							for (int j=0;j<m_plates_width-f*2;j++) {
								prgb[j+i*cxImage+(int)f].rgbtRed   = (BYTE) 0;
								prgb[j+i*cxImage+(int)f].rgbtGreen = (BYTE) 0;
								prgb[j+i*cxImage+(int)f].rgbtBlue = (BYTE) 255;
							}
							f = f + 1;
						}
					}
				}
            } // x


			// if we are not playing anything, we do not care if a line is here
			if (play_plate == PLATE_NONE)
				break;

			/////////////////////////////////////////
			//
			// Push to plates stack
			//
			if (play_plate != PLATE_NONE)
			{
				m_plates_stack.push(play_plate,m_plates_stack.current);
			}

			// play the 5 without a strum hit. infact, release the strum up.
			OutputToRobot(((play_plate & PLATE_1) == PLATE_1),
						((play_plate & PLATE_2) == PLATE_2),
						((play_plate & PLATE_3) == PLATE_3),
						((play_plate & PLATE_4) == PLATE_4),
						((play_plate & PLATE_5) == PLATE_5),
						0 , OUTPUT_TYPE_PORT);


#if DEBUG
			//OutputToRobot(plates_debug[0],
			//			plates_debug[1],
			//			plates_debug[2],
			//			plates_debug[3],
			//			plates_debug[4],
			//			true, OUTPUT_TYPE_FILE);
#endif

            break;

		//
		// draw the plates
		//
        case IDC_PLATES:

			prgb = (RGBTRIPLE*) pData;
			// go straight to the y position of the places.
			prgb = prgb + m_plates_y * (pvi->bmiHeader.biWidth);

			// go straight to the x position of each plate.
			prgb_x_start = prgb;
			for (int in_plate=0;in_plate<5;in_plate++)
			{
				prgb = prgb_x_start + plates_position[in_plate];

				f = 0; // f helps to create a trapezoid.
				for (int i=0;i<m_plates_height;i++)
				{
					for (int j=0;j<m_plates_width-f*2;j++) 
					{
						prgb[j+i*cxImage+(int)f].rgbtRed   = (BYTE) 255;
						prgb[j+i*cxImage+(int)f].rgbtGreen = (BYTE) 255;
						prgb[j+i*cxImage+(int)f].rgbtBlue = (BYTE) 0;
					}
					f = f + 1;
				}
			}

            break;

		case IDC_BINARY:   
			////////////////////////////////////////
			//
			// demonstrate how the binary image looks like.
			//
			prgb = (RGBTRIPLE*) pData;
            for (y = 0 ; y < pvi->bmiHeader.biHeight; y++) {
                for (x = 0 ; x < pvi->bmiHeader.biWidth; x++,prgb++) 
				{
					R = (int)prgb->rgbtRed;
					G = (int)prgb->rgbtGreen;
					B = (int)prgb->rgbtBlue;
				
					H=0;S=0;V=0;
					RGBtoHSV(R,G,B,&H,&S,&V);
					byte b = (byte)(((V >= min) && (V <= max)) ? 255 : 0);

					// Test YUV: doesn't seem as better as HSV
					//Y = (( ( 66 * R + 129 * G + 25 * B + 128) >> 8) + 16);
					//U = ( ( -38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
					//V = ( ( 112 * R - 94 * G - 18 * B + 128) >> 8) + 128;
					//byte b = (byte)(((Y >= min) && (Y <= max)) ? 255 : 0);
					
					// set the pixels on the frame
					prgb->rgbtRed = b;
					prgb->rgbtGreen = b;
					prgb->rgbtBlue = b;
				}
			}
			break;
    }

    return NOERROR;

} // Transform (in place)

bool CGuitarHero::is_green(unsigned char r, unsigned char g, unsigned char b)
{
   if (g < 100)
     return 0;
     
   if (g - r < 30 || g - b < 30)
     return 0;

   printf("found green!!!\n");
     
   return 1;
}

bool CGuitarHero::is_red(unsigned char r, unsigned char g, unsigned char b)
{
   if (r < 150)
     return 0;

   if (r - g < 25 || r - b < 25)
     return 0;
     
        printf("found red!!!\n");
     
   return 1;
}

bool CGuitarHero::is_yellow(unsigned char r, unsigned char g, unsigned char b)
{
   if (r < 120 || g < 120)
     return 0;

   if (r - b < 25 || g - b < 25)
     return 0;
   
   printf("found yello!!!\n");
     
   return 1;
}

// connect to robot winsock
void CGuitarHero::ConnectToRobotServer()
{
	// init winsock
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	 
	wVersionRequested = MAKEWORD( 2, 2 );
	 
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return;
	}
	 
	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */
	 
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
			HIBYTE( wsaData.wVersion ) != 2 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup( );
		return; 
	}
	 
	/* The WinSock DLL is acceptable. Proceed. */

	int ret;
	//TODO: get IP address from configuration file or from properties dialog 
	char ipaddress[35] = "127.0.0.1";// "10.1.1.200"; //"127.0.0.1";

	cli.sin_addr.s_addr=inet_addr(ipaddress);//INADDR_ANY
	cli.sin_family=AF_INET;
	cli.sin_port=htons(20248);
	clisock=socket(AF_INET,SOCK_STREAM,0);	

	if (connect(clisock,(sockaddr*)&(cli),sizeof(cli)) == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
	}
//	cli.sin_addr.s_addr=inet_addr(ipaddress);
}

// disconnect from robot winsock
void CGuitarHero::DisconnectFromRobotServer()
{
	closesocket(clisock);
}

void CGuitarHero::OutputToRobot(int plate1_status, 
								int plate2_status, 
								int plate3_status, 
								int plate4_status, 
								int plate5_status,
								bool strum,
								int output_type)
{

	char buff[6] = "";
	char c[8]; 
	short dataByte = 0;  //binary representation of the parameters

	itoa(plate5_status,c,10);	strcpy(buff, c);
	itoa(plate4_status,c,10);	strcat(buff, c);
	itoa(plate3_status,c,10);	strcat(buff, c);
	itoa(plate2_status,c,10);	strcat(buff, c);
	itoa(plate1_status,c,10);	strcat(buff, c);
	itoa(strum,c,10);			strcat(buff, c);

	switch(output_type)
	{
		case OUTPUT_TYPE_PORT:
			// parallel port
			dataByte = encodeFeedByte(buff);
		//	//printf("ecoded to: %d\n", dataByte);
			Out32(0x378,dataByte);
			break;

		case OUTPUT_TYPE_SOCK:
			// socket
			send(clisock,buff,6,0);

			break;

		case OUTPUT_TYPE_FILE:

			FILE *filehandle;
			struct _timeb timebuffer;
			//char *timeline;

			_ftime(&timebuffer);
			//timeline = ctime( &(timebuffer.time)); // printf format %.19s.

			filehandle = fopen("output.csv", "a+");
			if (filehandle == NULL)
			{
				return;
			}
		    
			if (filehandle != NULL)
			{
				fprintf(filehandle, "%d,%d,%d,%d,%d,%d,%d.%hu\n", 
					plate1_status,plate2_status,plate3_status,plate4_status,plate5_status,strum,
					timebuffer.time, timebuffer.millitm);
			}

		if (filehandle != NULL) fclose(filehandle);

		break;
	}

    return;
}

void CGuitarHero::RGBtoHSV(int R, int G, int B, int* H, int* S, int* V)
{
	// In this function, R, G, and B values must be scaled 
	// to be between 0 and 1.
	// (h)Hue will be a value between 0 and 360, and 
	// (s,v)Saturation and value are between 0 and 1. Scaled to 0-255

	double min;
	double max;
	double delta;

	double r = (double) R / 255;
	double g = (double) G / 255;
	double b = (double) B / 255;

	double h;
	double s;
	double v;

	min = min(min(r, g), b);
	max = max(max(r, g), b);
	v = max;
	delta = max - min;
	if ( max == 0 || delta == 0 ) 
	{
		// R, G, and B must be 0, or all the same.
		// In this case, S is 0, and H is undefined.
		// Using H = 0 is as good as any...
		s = 0;
		h = 0;
	} 
	else 
	{
		s = delta / max;
		if ( r == max ) 
		{
			// Between Yellow and Magenta
			h = (g - b) / delta;
		} 
		else if ( g == max ) 
		{
			// Between Cyan and Yellow
			h = 2 + (b - r) / delta;
		} 
		else 
		{
			// Between Magenta and Cyan
			h = 4 + (r - g) / delta;
		}

	}
	// Scale h to be between 0 and 360. 
	// This may require adding 360, if the value is negative.
	h *= 60;
	if ( h < 0 ) 
	{
		h += 360;
	}

	*H = (int)(h); *S = (int)(s * 255); *V = (int)(v * 255);
	return ;
}

//
// Check the input type is OK - return an error otherwise
//
HRESULT CGuitarHero::CheckInputType(const CMediaType *mtIn)
{
    CheckPointer(mtIn,E_POINTER);

    // check this is a VIDEOINFOHEADER type
    if (*mtIn->FormatType() != FORMAT_VideoInfo) {
        return E_INVALIDARG;
    }

    // Can we transform this type
    if (CanPerformGuitarHero(mtIn)) {
        return NOERROR;
    }
    return E_FAIL;
}


//
// Checktransform: Check a transform can be done between these formats
//
HRESULT CGuitarHero::CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut)
{
    CheckPointer(mtIn,E_POINTER);
    CheckPointer(mtOut,E_POINTER);

    if (CanPerformGuitarHero(mtIn)) 
    {
        if (*mtIn == *mtOut) 
        {
            return NOERROR;
        }
    }

    return E_FAIL;
}


//
// DecideBufferSize
//
// Tell the output pin's allocator what size buffers we
// require. Can only do this when the input is connected
//
HRESULT CGuitarHero::DecideBufferSize(IMemAllocator *pAlloc,ALLOCATOR_PROPERTIES *pProperties)
{
    // Is the input pin connected

    if (m_pInput->IsConnected() == FALSE) {
        return E_UNEXPECTED;
    }

    CheckPointer(pAlloc,E_POINTER);
    CheckPointer(pProperties,E_POINTER);
    HRESULT hr = NOERROR;

    pProperties->cBuffers = 1;
    pProperties->cbBuffer = m_pInput->CurrentMediaType().GetSampleSize();
    ASSERT(pProperties->cbBuffer);

    // Ask the allocator to reserve us some sample memory, NOTE the function
    // can succeed (that is return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted

    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties,&Actual);
    if (FAILED(hr)) {
        return hr;
    }

    ASSERT( Actual.cBuffers == 1 );

    if (pProperties->cBuffers > Actual.cBuffers ||
            pProperties->cbBuffer > Actual.cbBuffer) {
                return E_FAIL;
    }
    return NOERROR;
}


//
// GetMediaType
//
// I support one type, namely the type of the input pin
// This type is only available if my input is connected
//
HRESULT CGuitarHero::GetMediaType(int iPosition, CMediaType *pMediaType)
{
    // Is the input pin connected
    if (m_pInput->IsConnected() == FALSE) {
        return E_UNEXPECTED;
    }

    // This should never happen
    if (iPosition < 0) {
        return E_INVALIDARG;
    }

    // Do we have more items to offer

    if (iPosition > 0) {
        return VFW_S_NO_MORE_ITEMS;
    }

    CheckPointer(pMediaType,E_POINTER);
    *pMediaType = m_pInput->CurrentMediaType();

    return NOERROR;
}


//
// CanPerformGuitarHero: Check if this is a RGB24 true colour format
//
BOOL CGuitarHero::CanPerformGuitarHero(const CMediaType *pMediaType) const
{
    CheckPointer(pMediaType,FALSE);

    if (IsEqualGUID(*pMediaType->Type(), MEDIATYPE_Video)) 
    {
        if (IsEqualGUID(*pMediaType->Subtype(), MEDIASUBTYPE_RGB24)) 
        {
            VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pMediaType->Format();
            return (pvi->bmiHeader.biBitCount == 24);
        }
    }

    return FALSE;
}


#define WRITEOUT(var)  hr = pStream->Write(&var, sizeof(var), NULL); \
               if (FAILED(hr)) return hr;

#define READIN(var)    hr = pStream->Read(&var, sizeof(var), NULL); \
               if (FAILED(hr)) return hr;


//
// GetClassID: This is the only method of IPersist
//
STDMETHODIMP CGuitarHero::GetClassID(CLSID *pClsid)
{
    return CBaseFilter::GetClassID(pClsid);
}


//
// ScribbleToStream
//
// Overriden to write our state into a stream
//
HRESULT CGuitarHero::ScribbleToStream(IStream *pStream)
{
    HRESULT hr;
    WRITEOUT(m_effect);
	WRITEOUT(m_effectThreshold);
	WRITEOUT(m_detection_hit_delay);
    return NOERROR;
}


//
// ReadFromStream
//
// Likewise overriden to restore our state from a stream
//
HRESULT CGuitarHero::ReadFromStream(IStream *pStream)
{
    HRESULT hr;
    READIN(m_effect);
	READIN(m_effectThreshold);
	READIN(m_detection_hit_delay);
    return NOERROR;
} 

//
// GetPages
//
// Returns the clsid's of the property pages we support
//
STDMETHODIMP CGuitarHero::GetPages(CAUUID *pPages)
{
    CheckPointer(pPages,E_POINTER);

    pPages->cElems = 1;
    pPages->pElems = (GUID *) CoTaskMemAlloc(sizeof(GUID));
    if (pPages->pElems == NULL) {
        return E_OUTOFMEMORY;
    }

    *(pPages->pElems) = CLSID_GuitarHeroPropertyPage;
    return NOERROR;
} 


//
// get_IPEffect
//
// Return the current effect selected
//
STDMETHODIMP CGuitarHero::get_IPEffect(int *IPEffect,int *pThreshold, int *pDetection_hit_delay)
{
    CAutoLock cAutolock(&m_GuitarHeroLock);
    CheckPointer(IPEffect,E_POINTER);
	CheckPointer(pThreshold,E_POINTER);
	CheckPointer(pDetection_hit_delay,E_POINTER);

    *IPEffect = m_effect;
	*pThreshold = m_effectThreshold;
	*pDetection_hit_delay = m_detection_hit_delay;

    return NOERROR;
} 


//
// put_IPEffect: Set the required video effect
//
STDMETHODIMP CGuitarHero::put_IPEffect(int IPEffect, int threshold, int detection_hit_delay)
{
    CAutoLock cAutolock(&m_GuitarHeroLock);

    m_effect = IPEffect;
	m_effectThreshold = threshold;
	m_detection_hit_delay = detection_hit_delay;

    SetDirty(TRUE);
    return NOERROR;
}


