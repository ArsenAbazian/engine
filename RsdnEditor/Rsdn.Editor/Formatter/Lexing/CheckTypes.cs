using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	[Flags]
	public enum CheckTypes
	{
		None = 0,
		IsNumeric = 2,
		IsAlpha = 4,
		IsAlphaNumeric = 8,
		IsSpecificSymbol = 16,
		IsBrace = 32

	}
}