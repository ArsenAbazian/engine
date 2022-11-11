using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public interface IConfigurableLexer : ILexer
	{
		SyntaxDefinition Syntax { get; set; }
	}
}