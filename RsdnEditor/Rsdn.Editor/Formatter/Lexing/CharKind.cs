using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public enum CharKind
	{
		Empty,

		Undefined,

		Space,

		NewLine,

		Identifier,

		Numeric,

		Operator,

		Error
	}
}