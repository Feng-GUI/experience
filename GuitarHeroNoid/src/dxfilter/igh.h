//------------------------------------------------------------------------------
// File: IGH.h
//
// Desc: DirectShow Guitar Hero Robot - custom interface to allow the user to
//       perform image special effects.
//
// Author: Rafael Mizrahi
// http://www.garagegeeks.org
//------------------------------------------------------------------------------


#ifndef __IGUITARHERO__
#define __IGUITARHERO__

#ifdef __cplusplus
extern "C" {
#endif

    // { fd5010a3-8ebe-11ce-8183-00aa00577da1 }
    DEFINE_GUID(IID_IIPEffect,
    0xfd5010a3, 0x8ebe, 0x11ce, 0x81, 0x83, 0x00, 0xaa, 0x00, 0x57, 0x7d, 0xa1);

    DECLARE_INTERFACE_(IIPEffect, IUnknown)
    {
        STDMETHOD(get_IPEffect) (THIS_
                    int *effectNum,         // The current effect
					int *Threshold,			// Binary threshold
					int *Detection_hit_delay
                 ) PURE;

        STDMETHOD(put_IPEffect) (THIS_
                    int effectNum,          // Change to this effect
					int Threshold,			// Binary threshold
					int Detection_hit_delay
                 ) PURE;
    };

#ifdef __cplusplus
}
#endif

#endif // __IGUITARHERO__

