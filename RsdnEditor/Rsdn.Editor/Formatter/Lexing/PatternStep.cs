using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public struct PatternStep
	{
		public bool Continuos;
		public bool Optional;
		public bool AndSequence;
		public string SymbolCheck;
		public CheckTypes Checks;
	}
}