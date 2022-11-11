using System;
using System.Collections.Generic;
using System.Text;

namespace Rsdn.Editor
{
	/// <summary>
	/// Диапазон между двумя позициями
	/// </summary>
	/// <typeparam name="T"></typeparam>
	class PositionSpan<T>
	{
		//public PositionSpan(int lineDelta, int characterDelta)
		//{
		//  CharacterDelta = characterDelta;
		//  LineDelta = lineDelta;
		//}

		public int LineDelta;
		public int CharacterDeltaStart;
		public int CharacterDeltaEnd;
	}
}
