using System;
using System.Collections.Generic;
using System.Text;

namespace Rsdn.Editor
{
	internal struct StyledRange
	{
		#region Конструкторы

		public StyledRange(Style style, string text, int startPos, int endPos)
		{
			Style = style;
			Text = text;
			StartPos = startPos;
			EndPos = endPos;
		}

		public StyledRange(Style style, int startPos)
		{
			Style = style;
			Text = null;
			StartPos = startPos;
			EndPos = 0;
		}
		
		#endregion

		public Style Style;
		public string Text;
		public int StartPos;
		public int EndPos;

		#region ToString

		public override string ToString()
		{
			return "StartPos: " + StartPos
				+ "; EndPos: " + EndPos
				+ "; Style: " + Style
				+ "; Text: «" + Text + "»;";
		}
		
		#endregion

		#region BinarySearch

		public static int BinarySearch(StyledRange[] array, int position)
		{
			return BinarySearch(array, 0, array.Length - 1, position);
		}

		public static int BinarySearch(StyledRange[] array, int lo, int position)
		{
			return BinarySearch(array, lo, array.Length, position);
		}

		public static int BinarySearch(StyledRange[] array, int lo, int hi,
			int position)
		{
			while (lo <= hi)
			{
				int i = (lo + hi) >> 1;
				int cmpResult = array[i].StartPos - position;

				if (cmpResult == 0)
					return i;
				else if (cmpResult < 0)
					lo = i + 1;
				else
					hi = i - 1;
			}

			return ~lo;
		}

		#endregion
	}
}
