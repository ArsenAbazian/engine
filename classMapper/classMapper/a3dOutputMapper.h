// gridMapper.h

#pragma once

#include "a3dUtils\\a3dTextUtils.h"

using namespace System;
using namespace System::Runtime::InteropServices;

void managedOutInfo(TCHAR *text);
void managedOutWarning(TCHAR *text);
void managedOutError(TCHAR *text);

namespace classMapper {

	public delegate void CallbackDelegate(TCHAR *str);
	
	[StructLayoutAttribute( LayoutKind::Sequential, CharSet = CharSet::Unicode )]
	public ref struct a3dOutputManagerDelegate
	{
		[MarshalAsAttribute(UnmanagedType::FunctionPtr)]
		CallbackDelegate  ^_Delegate;
	};
	
	public ref class a3dOutputManaged { 
		void				outInfo(TCHAR *text);
		void				outWarning(TCHAR *text);
		void				outError(TCHAR *text);
	public:
		a3dOutputManaged();
		
		a3dOutput				*output;
		a3dOutputManagerDelegate ^delegInfo;
		a3dOutputManagerDelegate ^delegWarning;
		a3dOutputManagerDelegate ^delegError;

		delegate void		outDelegInfo(TCHAR *text);
		delegate void		outDelegWarning(TCHAR *text);
		delegate void		outDelegError(TCHAR *text);

		virtual void		outManagedInfo(System::String ^text);
		virtual void		outManagedWarning(System::String ^text);
		virtual void		outManagedError(System::String ^text);
	};
}