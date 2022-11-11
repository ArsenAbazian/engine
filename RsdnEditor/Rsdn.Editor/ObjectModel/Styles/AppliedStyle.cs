using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;

namespace Rsdn.Editor
{
	public struct AppliedStyle
	{
		#region Кострукторы

		[DebuggerHidden]
		public AppliedStyle(SimpleStyle style, int pos, int len)
		{
			_pos = pos;
			_length = len;
			_style = style;
		}
		
		#endregion

		#region Свойство Pos

		private int _pos;

		public int Pos
		{
			[DebuggerHidden]
			get { return _pos; }
			[DebuggerHidden]
			set
			{
				if (value < 0)
				{
					System.Diagnostics.Debug.Assert(value >= 0);
				}
				_pos = value;
			}
		}
		
		#endregion

		#region Свойство Length

		private int _length;

		public int Length
		{
			[DebuggerHidden]
			get { return _length; }
			[DebuggerHidden]
			set { _length = value; }
		}

		#endregion

		#region Свойство EndPos

		public int EndPos
		{
			[DebuggerHidden]
			get { return Pos + Length - 1; }
			[DebuggerHidden]
			set { Length = value - Pos + 1; }
		}
		
		#endregion

		#region Свойство Style

		private SimpleStyle _style;

		public SimpleStyle Style
		{
			[DebuggerHidden]
			get { return _style; }
			[DebuggerHidden]
			set { _style = value; }
		}

		#endregion

		#region ToString

		[DebuggerHidden]
		public override string ToString()
		{
			return "Pos: " + Pos + "; " + "Len: " + Length + "; " + "Style: " + Style + "; ";
		}
		
		#endregion		
	}
}