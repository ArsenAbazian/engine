using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace Rsdn.Editor.ObjectModel.Edit
{
	public delegate ICommand MargeCommands(ICommand command1, ICommand command2);

	public abstract class Command
	{
		#region ������������

		//[DebuggerHidden]
		public Command(Document document,
			Range<Document> selectionBefor, Range<Document> selectionAfter,
			bool restoreSelection)
		{
			_document = document;
			_selectionBefor = selectionBefor;
			_selectionAfter = selectionAfter;
			_restoreSelection = restoreSelection;
		}
		
		#endregion

		#region RestoreSelection property

		/// <summary>
		/// ���������� ������ �� �������������� ��������� ����� ���������� �������.
		/// </summary>
		private bool _restoreSelection;

		public bool RestoreSelection
		{
			[DebuggerHidden]
			get { return _restoreSelection; }
		}
		
		#endregion

		#region SelectionBefor property

		private Range<Document> _selectionBefor;

		/// <summary>
		/// ���� ������� ������ �������������� ���������, ������ ����������
		/// ��������� (������� ����� ����������� ����� ���������� �������).
		/// </summary>
		public Range<Document> SelectionBefor
		{
			[DebuggerHidden]
			get { return _selectionBefor; }
		}

		#endregion

		#region SelectionAfter property

		private Range<Document> _selectionAfter;

		/// <summary>
		/// ���� ������� ������ �������������� ���������, ������ ����������
		/// ��������� (������� ����� ����������� ����� ���������� �������).
		/// </summary>
		public Range<Document> SelectionAfter
		{
			[DebuggerHidden]
			get { return _selectionAfter; }
		}

		#endregion

		#region Document property

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		private Document _document;

		public Document Document
		{
			[DebuggerHidden]
			get { return _document; }
		}
 
		#endregion

		#region MargeCommands (������ ����������� �����).

		private static MargeCommands _margeCommands = MargeCommandsDefault;

		public static MargeCommands MargeCommands
		{
			[DebuggerHidden]
			get { return _margeCommands; }
			[DebuggerHidden]
			set { _margeCommands = value; }
		}

		/// <summary>
		/// ���������� ������� ����� ��� �������. ���� ������� ����������
		/// �����, ������������ �������. ����� ���������� null.
		/// </summary>
		/// <remarks>
		/// �����
		/// </remarks>
		/// <param name="command1"></param>
		/// <param name="command2"></param>
		/// <returns></returns>
		public static ICommand MargeCommandsDefault(ICommand command1, ICommand command2)
		{
			DeleteCommand delCmd1 = command1 as DeleteCommand;

			if (delCmd1 == null)
				return null;

			DeleteCommand delCmd2 = command2 as DeleteCommand;

			if (delCmd2 == null || delCmd1.EndPosition != delCmd2.StartPosition
				|| delCmd1.Document != delCmd2.Document
				|| delCmd1.RestoreSelection != delCmd2.RestoreSelection)
				return null;

			string text2 = delCmd1.Document.GetText(
				delCmd1.StartPosition, delCmd1.EndPosition);

			if (text2.Contains("\n") || text2.Contains("\r") || text2.StartsWith("\t"))
				return null;

			if (delCmd1.RestoreSelection)
				return new DeleteCommand(delCmd1.Document, delCmd1.StartPosition,
					delCmd2.EndPosition, delCmd2.SelectionBefor, delCmd1.SelectionAfter);
			else
				return new DeleteCommand(delCmd1.Document, delCmd1.StartPosition,
					delCmd2.EndPosition);
		}
	} 

	#endregion
}
