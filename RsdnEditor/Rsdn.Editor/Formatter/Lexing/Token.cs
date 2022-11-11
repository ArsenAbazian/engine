using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;

namespace Rsdn.Editor.Lexing
{
	public struct Token
	{
		public readonly TokenType Type;

		public readonly int Position;

		public readonly int Length;

		public readonly int Style;

		[DebuggerHidden]
		public Token(TokenType type, int position, int length, int style)
		{
			Type = type;
			Position = position;
			Length = length;
			Style = style;
		}
	}

}