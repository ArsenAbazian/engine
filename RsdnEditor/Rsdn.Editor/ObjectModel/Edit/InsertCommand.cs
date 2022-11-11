using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace Rsdn.Editor.ObjectModel.Edit
{
	/// <summary>
	/// Команда редактирования вставляющая текст в поределенную позицию.
	/// </summary>
	public class InsertCommand : ModifyTextCommand, ICommand
	{
		#region Конструкторы
		
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
			// Преобразуем строку в набор линий (строк).
			string[] lines = DocumentRowCollection.Parse(Text);

			// Рассчитываем кокой будет координата последнего вставляемого символа.
			Position<Document> endPosition = new Position<Document>(0, 0);
			endPosition.Line = StartPosition.Line + lines.Length - 1;
			endPosition.Character = StartPosition.Character
				+ lines[lines.Length - 1].Length;

			// Оповещаем все view о том, что текст будет изменен.
			foreach (IView view in Document.InternalViews)
				if (!view.BeginTextUpdated(StartPosition, StartPosition))
					return null;

			// Вставляем текст.
			endPosition = Document.InternalInsert(StartPosition, lines);

			// Оповещаем все view о том, что текст изменился.
			foreach (IView view in Document.InternalViews)
				view.EndTextUpdated(StartPosition, endPosition);

			IView activeView = Document.ActiveView;
			
			if (AutoSelect)
				activeView.Selection.SetSelection(StartPosition, endPosition);
			// Восстанавливаем выделение, если требуется.
			else if (RestoreSelection && SelectionAfter != Range<Document>.Invalid)
				activeView.Selection.Range = SelectionAfter;
			else
				activeView.Selection.SetSelection(endPosition, endPosition);

			// Формируем инвертированную команду (для закладки в undo/redo-буфер.
			return new DeleteCommand(Document, StartPosition, endPosition,
				activeView.Selection.Range, previousSelection);
		}

		public ICommand Execute()
		{
			return Execute(SelectionBefor);
		}

		#endregion

		#region Конструкторы

		public override string ToString()
		{
			return "Insert: StartPosition: { " + StartPosition
				+ " }; Text = '" + Text + "'";
		}
 
		#endregion
	}
}
