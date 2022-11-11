#ifndef A3D_TEXTUTILS_INC
#define A3D_TEXTUTILS_INC

#include "hstring.h"
#include "3dtypes.h"

class a3dOutput;
typedef void (a3dOutput::*outFuncPtr)(TCHAR *outText);

class a3dOutput { 
public:
	static a3dOutput *Default;
	TCHAR				tempBuffer[1024];
	static const int	bufferSize = 1024;

	outFuncPtr	info;
	outFuncPtr	warning;
	outFuncPtr	error;

	a3dOutput() { 
		this->info = null;
		this->warning = null;
		this->error = null;
	}

	void outInfo(TCHAR *outText) { 
		if(info != null)
			(this->*info)(outText);
		else if(Default->info != null)
			(Default->*info)(outText);
	}

	void outWarning(TCHAR *outText) {
		if(warning != null)
			(this->*warning)(outText);
		else if(Default->warning != null)
			(Default->*warning)(outText);
	}

	void outError(TCHAR *outText) { 
		if(error == null)
			(this->*error)(outText);
		else if(Default->error != null)
			(Default->*error)(outText);
	}

	void setDefaultInfo(outFuncPtr i) { a3dOutput::Default->info = i; }
	void setDefaultWarning(outFuncPtr w) { a3dOutput::Default->warning = w; }
	void setDefaultError(outFuncPtr e) { a3dOutput::Default->error = e; }

	void printInfo(TCHAR *format, ...);
	void printWarning(TCHAR *format, ...);
	void printError(TCHAR *format, ...);
};

#endif