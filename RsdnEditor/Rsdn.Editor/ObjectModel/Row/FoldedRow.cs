using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace Rsdn.Editor
{
	/// <summary>
	/// Это строка получившаяся в результате агрегигации реальных строк 
	/// (например, в следствии схлопывания региона.
	/// </summary>
	public class FoldedRow : Row
	{
		public FoldedRow(int startIndex, int count)
		{
			StartIndex = startIndex;
			Count = count;
		}

		public override DocumentRow RealDocumentRow
		{
			[DebuggerHidden]
			get { throw new Exception("The method or operation is not implemented."); }
		}

		public override string Text
		{
			get { throw new Exception("The method or operation is not implemented."); }
			set { throw new Exception("The method or operation is not implemented."); }
		}

		public override int TextLength
		{
			get { throw new Exception("The method or operation is not implemented."); }
		}

		public override int OffsetInDocumentRow { get { return 0; } }

		public override int ViewPosition { get { return -1; } }

		public override AppliedStyle[] AppliedStyles
		{
			get { throw new Exception("The method or operation is not implemented."); }
			set { throw new Exception("The method or operation is not implemented."); }
		}

		private int _startIndex;

		/// <summary>
		/// Индекс первой строки входящей в область агрегации.
		/// </summary>
		public int StartIndex
		{
			get { return _startIndex; }
			set { _startIndex = value; }
		}

		private int _count;

		/// <summary>
		/// Количество строк входящих в область агрегации.
		/// </summary>
		public int Count
		{
			get { return _count; }
			set { _count = value; }
		}

		public override void AcceptVisitor(IRowVisitor visitor)
		{
			visitor.Visit(this);
		}
	}
}
