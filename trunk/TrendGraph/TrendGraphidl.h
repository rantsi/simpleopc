

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Feb 26 21:44:08 2008
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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __TrendGraphidl_h__
#define __TrendGraphidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DTrendGraph_FWD_DEFINED__
#define ___DTrendGraph_FWD_DEFINED__
typedef interface _DTrendGraph _DTrendGraph;
#endif 	/* ___DTrendGraph_FWD_DEFINED__ */


#ifndef ___DTrendGraphEvents_FWD_DEFINED__
#define ___DTrendGraphEvents_FWD_DEFINED__
typedef interface _DTrendGraphEvents _DTrendGraphEvents;
#endif 	/* ___DTrendGraphEvents_FWD_DEFINED__ */


#ifndef __TrendGraph_FWD_DEFINED__
#define __TrendGraph_FWD_DEFINED__

#ifdef __cplusplus
typedef class TrendGraph TrendGraph;
#else
typedef struct TrendGraph TrendGraph;
#endif /* __cplusplus */

#endif 	/* __TrendGraph_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __TrendGraphLib_LIBRARY_DEFINED__
#define __TrendGraphLib_LIBRARY_DEFINED__

/* library TrendGraphLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_TrendGraphLib;

#ifndef ___DTrendGraph_DISPINTERFACE_DEFINED__
#define ___DTrendGraph_DISPINTERFACE_DEFINED__

/* dispinterface _DTrendGraph */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DTrendGraph;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E8644C23-411B-4678-A2AB-951196AC28A5")
    _DTrendGraph : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DTrendGraphVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DTrendGraph * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DTrendGraph * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DTrendGraph * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DTrendGraph * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DTrendGraph * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DTrendGraph * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DTrendGraph * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DTrendGraphVtbl;

    interface _DTrendGraph
    {
        CONST_VTBL struct _DTrendGraphVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DTrendGraph_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DTrendGraph_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DTrendGraph_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DTrendGraph_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DTrendGraph_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DTrendGraph_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DTrendGraph_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DTrendGraph_DISPINTERFACE_DEFINED__ */


#ifndef ___DTrendGraphEvents_DISPINTERFACE_DEFINED__
#define ___DTrendGraphEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DTrendGraphEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DTrendGraphEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("BD179925-E263-4DAF-B656-DF960AFE9A54")
    _DTrendGraphEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DTrendGraphEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DTrendGraphEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DTrendGraphEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DTrendGraphEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DTrendGraphEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DTrendGraphEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DTrendGraphEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DTrendGraphEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DTrendGraphEventsVtbl;

    interface _DTrendGraphEvents
    {
        CONST_VTBL struct _DTrendGraphEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DTrendGraphEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DTrendGraphEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DTrendGraphEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DTrendGraphEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DTrendGraphEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DTrendGraphEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DTrendGraphEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DTrendGraphEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_TrendGraph;

#ifdef __cplusplus

class DECLSPEC_UUID("58714506-5456-42EF-8849-E25F3E79C651")
TrendGraph;
#endif
#endif /* __TrendGraphLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


