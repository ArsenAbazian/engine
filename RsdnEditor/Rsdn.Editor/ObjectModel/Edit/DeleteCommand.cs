using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace Rsdn.Editor.ObjectModel.Edit
{
	/// <summary>
	/// ������� �������������� ��������� ������� ������.
	/// </summary>
	public class DeleteCommand : ModifyTextCommand, ICommand
	{
		#region ������������

		//[DebuggerHidden]
		public DeleteCommand(Document document,
			Position<Document> startPosition, Position<Document> endPosition,
			Range<Document> selectionBefore, Range<Document> selectionAfter)
			: base(document, startPosition, selectionBefore, selectionAfter)
		{
			_endPosition = endPosition;
		}

		//[DebuggerHidden]
		public DeleteCommand(Document document,
			Position<Document> startPosition, Position<Document> endPosition)
			: base(document, startPosition)
		{
			_endPosition = endPosition;
		}
 
		#endregion

		#region EndPosition property

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		private Position<Document> _endPosition;

		public Position<Document> EndPosition
		{
			[DebuggerHidden]
			get { return _endPosition; }
		}

		#endregion

		#region ICommand Members

		public ICommand Execute(Range<Document> previousSelection)
		{
			Range<Document> range = GetRange();

			string deletedText = Document.GetText(StartPosition, EndPosition);

			// ��������� ��� view � ���, ��� ����� ����� �������.
			foreach (IView view in Document.InternalViews)
				if (!view.BeginTextUpdated(StartPosition, EndPosition))
					return null;

			// ������� �����.
			Document.InternalDelete(range.Start, range.End);

			// ��������� ��� view � ���, ��� ����� ���������.
			foreach (IView view in Document.InternalViews)
				view.EndTextUpdated(range.Start, range.Start);

			// ��������������� ���������, ���� ���������.
			if (RestoreSelection && SelectionAfter != Range<Document>.Invalid)
				Document.ActiveView.Selection.Range = SelectionAfter;
			else
				Document.ActiveView.Selection.SetSelection(range.Start, range.Start);

			// ��������� ��������������� ������� (��� �������� � undo/redo-�����.
			InsertCommand inverse = new InsertCommand(Document,
				// ��������� ������, ���������� ��������� �������� ������, � 
				// �������� �� � undo-�����.
				deletedText, range.Start, 
				Document.ActiveView.Selection.Range, previousSelection);

			// ���������� ������� �������� ������� (��� ��������� � undo/redo-�����).
			return inverse;
		}

		public ICommand Execute()
		{
			return Execute(SelectionBefor);
		}

		#region GetRange

		/// <summary>
		/// ���������� � ������ ���������� ����������� ���������� ���������, � 
		/// �� ������ ������������.
		/// </summary>
		private Range<Document> GetRange()
		{
			if (StartPosition > EndPosition)
				return new Range<Document>(EndPosition, StartPosition);
			else
				return new Range<Document>(StartPosition, EndPosition);
		}

		#endregion

		#endregion

		#region ToString

		[DebuggerHidden]
		public override string ToString()
		{
			return "Insert: { StartPosition: {" + StartPosition
				+ " }; EndPosition: { " + EndPosition + " } }";
		}
 
		#endregion
	}
}
