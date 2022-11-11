using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using Rsdn.Editor.ObjectModel;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;

namespace Rsdn.Editor
{
	public class Selection
	{
		#region Конструкторы

		[DebuggerHidden]
		internal Selection(View view)
		{
			_view = view;
		}

		[SuppressMessage("Microsoft.Performance", "CA1801:AvoidUnusedParameters")]
		private string Text
		{
			[DebuggerHidden]
			get { throw new Exception(); }
			[DebuggerHidden]
			set { throw new Exception(); }
		} 

		#endregion

		#region Свойство Range

		private Range _range;

		public Range Range
		{
			[DebuggerHidden]
			get { return _range; }
			[DebuggerHidden]
			set { _range = value; }
		}
		
		#endregion

		#region Свойство End

		public Position<IView> End
		{
			[DebuggerHidden]
			get { return _range.End; }
			[DebuggerHidden]
			set { _range.End = value; }
		}

		#endregion

		#region Свойство StartLine

		private int StartLine
		{
			[DebuggerHidden]
			get { return _range.Start.Line; }
			[DebuggerHidden]
			set { _range.Start.Line = value; }
		}

		#endregion

		#region Свойство EndLine

		private int EndLine
		{
			[DebuggerHidden]
			get { return _range.End.Line; }
			[DebuggerHidden]
			set { _range.End.Line = value; }
		}

		#endregion

		#region Свойство StartLineCharacter

		private int StartLineCharacter
		{
			[DebuggerHidden]
			get { return _range.Start.Character; }
			[DebuggerHidden]
			set { _range.Start.Character = value; }
		}

		#endregion

		#region Свойство EndLineCharacter

		private int EndLineCharacter
		{
			[DebuggerHidden]
			get { return _range.End.Character; }
			[DebuggerHidden]
			set { _range.End.Character = value; }
		}

		#endregion

		#region Свойство View

		private View _view;

		public IView View
		{
			[DebuggerHidden]
			get { return _view; }
		}
		
		#endregion
	}
}
