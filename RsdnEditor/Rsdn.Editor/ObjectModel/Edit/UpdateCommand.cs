using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace Rsdn.Editor.ObjectModel.Edit
{
	/// <summary>
	/// Команда редактирования заменяющая участок текста другим 
	/// (содержащимся в команде).
	/// </summary>
	public class UpdateCommand : ModifyTextCommand, ICommand
	{
		#region Конструкторы

		public UpdateCommand(Document document, string text,
			Position<Document> startPosition, Position<Document> endPosition,
			Range<Document> selectionBefore, Range<Document> selectionAfter)
			: base(document, startPosition, selectionBefore, selectionAfter)
		{
			_text = text;
			_endPosition = endPosition;
		}

		public UpdateCommand(Document document, string text,
			Position<Document> startPosition, Position<Document> endPosition)
			: base(document, startPosition)
		{
			_text = text;
			_endPosition = endPosition;
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

		#region EndPosition property

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		private Position<Document> _endPosition;

		public Position<Document> EndPosition
		{
			[DebuggerHidden]
			get { return _endPosition; }
		}

		#endregion

		#region ICommand MembersB

		public ICommand Execute(Range<Document> previousSelection)
		{
			Range<Document> range = GetRange();

			// Формируем строку, содержащую удаляемый фрагмент текста, и 
			// помещаем ее в undo-буфер.
			string deletedText = Document.GetText(range.Start, range.End);
			// Преобразуем строку в набор линий (строк).
			string[] lines = DocumentRowCollection.Parse(Text);

			// Оповещаем все view о том, что текст будет изменен.
			foreach (IView view in Document.InternalViews)
				if (!view.BeginTextUpdated(StartPosition, EndPosition))
					return null;

			// Изменяем текст.
			Document.InternalDelete(range.Start, range.End);
			Position<Document> endPosition = Document.InternalInsert(range.Start, lines);
			
			// Оповещаем все view о том, что текст изменился.
			foreach (IView view in Document.InternalViews)
				view.EndTextUpdated(range.Start, endPosition);

			ISelection selection = Document.ActiveView.Selection;

			// Восстанавливаем выделение, если требуется.
			if (RestoreSelection && SelectionAfter != Range<Document>.Invalid)
				selection.Range = SelectionAfter;
			else
				selection.SetSelection(endPosition, endPosition);

			// Формируем инвертированную команду (для закладки в undo/redo-буфер.
			return new UpdateCommand(Document, deletedText,
				range.Start, endPosition, selection.Range, previousSelection);
		}

		public ICommand Execute()
		{
			return Execute(SelectionBefor);
		}

		#region GetRange

		/// <summary>
		/// Возвращает в первом параметере минимальную координату веделения, а 
		/// во втором максимальную.
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
		public override string ToString()
		{
			return "Update: StartPosition: { " + StartPosition
				+ " }; EndPosition: { " + EndPosition
				+ " }; Text = '" + Text + "'";
		}
 
		#endregion
	}
}
