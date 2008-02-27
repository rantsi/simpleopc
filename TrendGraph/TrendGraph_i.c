

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Wed Feb 27 15:56:40 2008
 */
/* Compiler settings for .\TrendGraph.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_TrendGraphLib,0x2911BACA,0x4701,0x4A9C,0xB3,0x1E,0xEB,0x9C,0x7B,0xA5,0x81,0x88);


MIDL_DEFINE_GUID(IID, DIID__DTrendGraph,0xE8644C23,0x411B,0x4678,0xA2,0xAB,0x95,0x11,0x96,0xAC,0x28,0xA5);


MIDL_DEFINE_GUID(IID, DIID__DTrendGraphEvents,0xBD179925,0xE263,0x4DAF,0xB6,0x56,0xDF,0x96,0x0A,0xFE,0x9A,0x54);


MIDL_DEFINE_GUID(CLSID, CLSID_TrendGraph,0x58714506,0x5456,0x42EF,0x88,0x49,0xE2,0x5F,0x3E,0x79,0xC6,0x51);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



