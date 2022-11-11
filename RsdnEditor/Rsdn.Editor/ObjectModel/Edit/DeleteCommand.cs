using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace Rsdn.Editor.ObjectModel.Edit
{
	/// <summary>
	/// Команда редактирования удаляющая участок текста.
	/// </summary>
	public class DeleteCommand : ModifyTextCommand, ICommand
	{
		#region Конструкторы

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

			// Оповещаем все view о том, что текст будет изменен.
			foreach (IView view in Document.InternalViews)
				if (!view.BeginTextUpdated(StartPosition, EndPosition))
					return null;

			// Удаляем текст.
			Document.InternalDelete(range.Start, range.End);

			// Оповещаем все view о том, что текст изменился.
			foreach (IView view in Document.InternalViews)
				view.EndTextUpdated(range.Start, range.Start);

			// Восстанавливаем выделение, если требуется.
			if (RestoreSelection && SelectionAfter != Range<Document>.Invalid)
				Document.ActiveView.Selection.Range = SelectionAfter;
			else
				Document.ActiveView.Selection.SetSelection(range.Start, range.Start);

			// Формируем инвертированную команду (для закладки в undo/redo-буфер.
			InsertCommand inverse = new InsertCommand(Document,
				// Формируем строку, содержащую удаляемый фрагмент текста, и 
				// помещаем ее в undo-буфер.
				deletedText, range.Start, 
				Document.ActiveView.Selection.Range, previousSelection);

			// Возвращаем команду обратную текущей (для помещения в undo/redo-буфер).
			return inverse;
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

		[DebuggerHidden]
		public override string ToString()
		{
			return "Insert: { StartPosition: {" + StartPosition
				+ " }; EndPosition: { " + EndPosition + " } }";
		}
 
		#endregion
	}
}
