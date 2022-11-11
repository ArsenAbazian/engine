using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Diagnostics;

namespace Rsdn.Editor
{
	public class DocumentRow : Row
	{
		public DocumentRow()
		{
			_appliedStyles = _defaultPosStyleArray;
		}

		public DocumentRow(string text) : this()
		{
			_text = text;
		}

		public override DocumentRow RealDocumentRow
		{
			[DebuggerHidden]
			get { return this; }
		}

		private int _startStylerState;

		public int StartStylerState
		{
			[DebuggerHidden]
			get { return _startStylerState; }
			[DebuggerHidden]
			set { _startStylerState = value; }
		}

		private bool _isContinueStylingOnNextLine;

		public bool IsContinueStylingOnNextLine
		{
			[DebuggerHidden]
			get { return _isContinueStylingOnNextLine; }
			[DebuggerHidden]
			set { _isContinueStylingOnNextLine = value; }
		}

		private string _text;

		public override string Text
		{
			[DebuggerHidden]
			get { return _text; }
			[DebuggerHidden]
			set { _text = value; }
		}

		public override int TextLength
		{
			get { return _text.Length; }
		}

		public override int OffsetInDocumentRow { get { return 0; } }

		public Style EndStyle
		{
			get
			{
				return AppliedStyles.Length > 0 
					? _appliedStyles[AppliedStyles.Length - 1].Style : null;
			}
		}

		public override int ViewPosition { get { return 0; } }

		private AppliedStyle[] _appliedStyles;

		public override AppliedStyle[] AppliedStyles
		{
			[DebuggerHidden]
			get { return _appliedStyles; }
			[DebuggerHidden]
			set { _appliedStyles = value; }
		}

		internal AppliedStyle[] InternalPosStyles
		{
			[DebuggerHidden]
			get { return _appliedStyles; }
			[DebuggerHidden]
			set { _appliedStyles = value; }
		}

		[DebuggerHidden]
		public override void AcceptVisitor(IRowVisitor visitor)
		{
			visitor.Visit(this);
		}
	}
}
