using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace Rsdn.Editor.ObjectModel.Edit
{
	/// <summary>
	/// ������� �������������� ����������� ����� � ������������ �������.
	/// </summary>
	public class InsertCommand : ModifyTextCommand, ICommand
	{
		#region ������������
		
		public InsertCommand(Document document, string text,
			Position<Document> startPosition,
			Range<Document> selectionBefore, Range<Document> selectionAfter)
			: base(document, startPosition, selectionBefore, selectionAfter)
		{
			_text = text;
		}

		public InsertCommand(Document document, string text,
			Position<Document> startPosition)
			: base(document, startPosition)
		{
			_text = text;
		}

		public InsertCommand(Document document, string text,
			Position<Document> startPosition, bool autoSelect)
			: base(document, startPosition, autoSelect)
		{
			_text = text;
		}

		#endregion

		#region Text property

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		string _text;

		public string Text
		{
			[DebuggerHidden]
			get { return _text; }
		}

		#endregion

		#region ICommand Members

		public ICommand Execute(Range<Document> previousSelection)
		{
			// ����������� ������ � ����� ����� (�����).
			string[] lines = DocumentRowCollection.Parse(Text);

			// ������������ ����� ����� ���������� ���������� ������������ �������.
			Position<Document> endPosition = new Position<Document>(0, 0);
			endPosition.Line = StartPosition.Line + lines.Length - 1;
			endPosition.Character = StartPosition.Character
				+ lines[lines.Length - 1].Length;

			// ��������� ��� view � ���, ��� ����� ����� �������.
			foreach (IView view in Document.InternalViews)
				if (!view.BeginTextUpdated(StartPosition, StartPosition))
					return null;

			// ��������� �����.
			endPosition = Document.InternalInsert(StartPosition, lines);

			// ��������� ��� view � ���, ��� ����� ���������.
			foreach (IView view in Document.InternalViews)
				view.EndTextUpdated(StartPosition, endPosition);

			IView activeView = Document.ActiveView;
			
			if (AutoSelect)
				activeView.Selection.SetSelection(StartPosition, endPosition);
			// ��������������� ���������, ���� ���������.
			else if (RestoreSelection && SelectionAfter != Range<Document>.Invalid)
				activeView.Selection.Range = SelectionAfter;
			else
				activeView.Selection.SetSelection(endPosition, endPosition);

			// ��������� ��������������� ������� (��� �������� � undo/redo-�����.
			return new DeleteCommand(Document, StartPosition, endPosition,
				activeView.Selection.Range, previousSelection);
		}

		public ICommand Execute()
		{
			return Execute(SelectionBefor);
		}

		#endregion

		#region ������������

		public override string ToString()
		{
			return "Insert: StartPosition: { " + StartPosition
				+ " }; Text = '" + Text + "'";
		}
 
		#endregion
	}
}
