using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public enum TokenType
	{
		Identifier,

		Keyword,

		Block,

		BlockStart,

		BlockEnd,

		Error,

		Numeric,

		Operator
	}
}