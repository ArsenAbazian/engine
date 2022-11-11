using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public interface IScanner
	{
		CharKind GetCharKind(string text, int index);

		void Reset();

		void ResetStep();
	}
}