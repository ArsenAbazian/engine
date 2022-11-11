#include "stdafx.h"

#include "a3dOutputMapper.h"

namespace classMapper {

	a3dOutputManaged::a3dOutputManaged() { 
		
		this->output = new a3dOutput();
		delegInfo = gcnew a3dOutputManagerDelegate();
		delegWarning = gcnew a3dOutputManagerDelegate();
		delegError = gcnew a3dOutputManagerDelegate();
		
		delegInfo->_Delegate = gcnew CallbackDelegate( this, &a3dOutputManaged::outInfo );
		delegWarning->_Delegate = gcnew CallbackDelegate( this, &a3dOutputManaged::outWarning );
		delegError->_Delegate = gcnew CallbackDelegate( this, &a3dOutputManaged::outError );
	
		outFuncPtr outFunc;
		Marshal::StructureToPtr(delegInfo, IntPtr(&outFunc), false);
		output->setDefaultInfo(outFunc);		
		Marshal::StructureToPtr(delegWarning, IntPtr(&outFunc), false);
		output->setDefaultWarning(outFunc);		
		Marshal::StructureToPtr(delegError, IntPtr(&outFunc), false);
		output->setDefaultError(outFunc);		
	}

	void a3dOutputManaged::outInfo(TCHAR *text) { 
		this->outManagedInfo(gcnew System::String(text));
	}

	void a3dOutputManaged::outWarning(TCHAR *text) { 
		this->outManagedWarning(gcnew System::String(text));
	}

	void a3dOutputManaged::outError(TCHAR *text) { 
		this->outManagedError(gcnew System::String(text));
	}

	void a3dOutputManaged::outManagedInfo(System::String ^text) { 
		
	}

	void a3dOutputManaged::outManagedWarning(System::String ^text) { 
		
	}

	void a3dOutputManaged::outManagedError(System::String ^text) { 
		
	}
}

