using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public interface ILexer
	{
		int Process(string text, int position);

		Block ParentBlock { get; set; }

		int State { get; set; }

		bool ContinueState { get; }

		List<Token> Tokens { get; }
	}
}