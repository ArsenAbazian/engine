using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public struct DefinitionError
	{
		internal DefinitionError(int lineNumber, int linePosition,
			string message, DefinitionErrorType type)
		{
			LineNumber = lineNumber;
			LinePosition = linePosition;
			Message = message;
			Type = type;
		}

		internal DefinitionError(string message, DefinitionErrorType type)
			:
			this(0, 0, message, type)
		{

		}

		public readonly int LineNumber;

		public readonly int LinePosition;

		public readonly string Message;

		public readonly DefinitionErrorType Type;
	}
}