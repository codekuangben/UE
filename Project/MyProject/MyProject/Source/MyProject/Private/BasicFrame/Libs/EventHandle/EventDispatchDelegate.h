#ifndef __EventDispatchDelegate_H
#define __EventDispatchDelegate_H

#include "FastDelegate.h"

//#include "MyDelegate/MySmDelegateCombinations.h"

class IDispatchObject;

typedef fastdelegate::FastDelegate1<IDispatchObject*> EventDispatchDelegate;
typedef fastdelegate::FastDelegate1<IDispatchObject*>* EventDispatchDelegatePtr;

#define MakeEventDispatchDelegate EventDispatchDelegate

//MY_DECLARE_DELEGATE_OneParam(EventDispatchDelegate, IDispatchObject*);
//typedef EventDispatchDelegate* EventDispatchDelegatePtr;

//#define MakeEventDispatchDelegate EventDispatchDelegate().BindRaw

//typedef MySmBaseDelegate<void, IDispatchObject*> EventDispatchDelegate;
//typedef MySmBaseDelegate<void, IDispatchObject*>* EventDispatchDelegatePtr;
//
//#define MakeMemFunDelegate(pThis, handle) MySmBaseDelegate<void, IDispatchObject*>::MySmBaseDelegate(pThis, handle)
//#define MakeStaticFunDelegate(handle) MySmBaseDelegate<void, IDispatchObject*>::MySmBaseDelegate(0, handle)
//
//#define MakeEventDispatchDelegate(pThis, handle) MakeMemFunDelegate(pThis, handle)

#endif