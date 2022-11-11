using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;

namespace Rsdn.Editor.Lexing
{
	internal class Scanner : IScanner
	{
		private CharKind oldKind;
		private CharKind lastKind;

		[DebuggerHidden]
		public CharKind GetCharKind(string text, int index)
		{
			char c = text[index];
			CharKind ret = CharKind.Undefined;

			if (IsIdentifier(c))
				ret = CharKind.Identifier;
			else if (c == ' ' || c == '\t' || c == '\r')
				ret = CharKind.Space;
			else if (c == '\n')
				ret = CharKind.NewLine;

			oldKind = lastKind;
			lastKind = ret;
			return ret;
		}


		public void Reset()
		{
			lastKind = CharKind.Empty;
			oldKind = CharKind.Empty;
		}


		public void ResetStep()
		{
			lastKind = oldKind;
			oldKind = CharKind.Empty;
		}


		[DebuggerHidden]
		private bool IsIdentifier(char c)
		{
			bool ret = Char.IsLetter(c);
			return ret ? ret : (lastKind == CharKind.Identifier ? Char.IsNumber(c) : false);
		}
	}
}