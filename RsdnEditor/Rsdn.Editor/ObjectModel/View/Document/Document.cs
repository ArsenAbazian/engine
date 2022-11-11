using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Drawing.Design;
using Rsdn.Editor.ObjectModel;
using System.Diagnostics;
using Rsdn.Editor.ObjectModel.Edit;
using System.Media;
using System.Collections.ObjectModel;

namespace Rsdn.Editor
{
	[TypeConverter(typeof(DocumentTypeConverter))]
	public class Document
	{
		public Document()
		{
			_viewsReadonly = new ReadOnlyCollection<IView>(_views);
			_rows = new DocumentRowCollection(this);
			_rows.Add(new DocumentRow(string.Empty));
		}

		public Document(string text) : this()
		{
			if (text == null)
				throw new ArgumentNullException("text");

			Rows.Init(text);
		}

		/// <summary>
		/// Возвращает true, если какое-либо свойство не равно значению
		/// по-умолчанию.
		/// </summary>
		internal protected bool ShouldSerialize()
		{
			return ShouldSerializeText()
				|| ShouldSerializeFormatter()
				|| IsDirty != DefaultIsDirty;
		}

		/// <summary>
		/// Сбрасывает все свойства в значения по-умолчанию.
		/// </summary>
		public void Reset()
		{
			ResetFormatter();
			ResetText();
			IsDirty = DefaultIsDirty;
		}

		#region Модификация текста (ExecuteCommand, Undo, Redo...).

		private Stack<ICommand> _undoStack = new Stack<ICommand>();
		private Stack<ICommand> _redoStack = new Stack<ICommand>();

		public static void ExecuteCommand(ICommand command)
		{
			Document document = command.Document;

			//ICommand reversCommand = command.Execute();
			ICommand reversCommand = command.Execute(
				document.ActiveView.Selection.Range);

			if (reversCommand == null)
			{
				SystemSounds.Beep.Play();
				return; 
			}

			ICommand merged = null;

			if (document._undoStack.Count > 0)
				merged = Command.MargeCommands(
					document._undoStack.Peek(), reversCommand);

			if (merged == null)
			{
				document._undoStack.Push(reversCommand);
				document.IncDirty();
			}
			else
			{
				document._undoStack.Pop();
				document._undoStack.Push(merged);
				reversCommand = merged;
			}

			document._redoStack.Clear();

			foreach (IView view in document.InternalViews)
				view.UpdatedView();
		}

		public bool CanUndo
		{ [DebuggerHidden] get { return _undoStack.Count > 0; } }
		public bool CanRedo
		{ [DebuggerHidden] get { return _redoStack.Count > 0; } }

		public void Undo()
		{
			if (CanUndo)
			{
				ICommand popCmd = _undoStack.Pop();
				ICommand reversCmd = popCmd.Execute();

				if (reversCmd == null)
				{
					_redoStack.Push(popCmd);
					SystemSounds.Beep.Play();
				}
				else
				{
					_redoStack.Push(reversCmd);
					DecDirty();
				}
			}
		}

		public void Redo()
		{
			if (CanRedo)
			{
				ICommand popCmd = _redoStack.Pop();

				ICommand reversCmd = popCmd.Execute();
				if (reversCmd == null)
				{
					_undoStack.Push(popCmd);
					SystemSounds.Beep.Play();
				}
				else
				{
					_undoStack.Push(reversCmd);
					IncDirty();
				}
			}
		}

		private void IncDirty()
		{
			bool isDirty = IsDirty;

			_dirtyCounter++;

			if (isDirty != IsDirty)
				OnDirtyChanged();
		}

		private void DecDirty()
		{
			bool isDirty = IsDirty;

			_dirtyCounter--;

			if (isDirty != IsDirty)
				OnDirtyChanged();
		}

		private int _dirtyCounter;
		private const bool DefaultIsDirty = false;

		[DefaultValue(DefaultIsDirty)]
		public bool IsDirty
		{
			[DebuggerHidden]
			get { return _dirtyCounter != 0; }
			//[DebuggerHidden]
			set
			{
				bool oldValue = IsDirty;

				_dirtyCounter = value ? int.MaxValue / 2 : 0;

				if (oldValue != IsDirty)
					OnDirtyChanged();
			}
		}

		public event EventHandler DirtyChanged;

		protected virtual void OnDirtyChanged()
		{
			if (DirtyChanged != null)
				DirtyChanged(this, EventArgs.Empty);
		}

		#endregion

		private List<IView> _views = new List<IView>();
		private ReadOnlyCollection<IView> _viewsReadonly;

		[Browsable(false)]
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		internal List<IView> InternalViews
		{
			[DebuggerHidden]
			get { return _views; }
		}

		public IList<IView> Views
		{
			[DebuggerHidden]
			get { return _viewsReadonly; }
		}

		private IView _activeView;

		public IView ActiveView
		{
			[DebuggerHidden]
			get { return _activeView; }
			[DebuggerHidden]
			internal set { _activeView = value; }
		}

		private IFormatter _formatter;

        protected virtual IFormatter CreateFormatter() { return new DefaultFormatter(); }

		[TypeConverter(typeof(ReferenceConverter))]
		public IFormatter Formatter
		{
			[DebuggerHidden]
			get {
                if (_formatter == null) _formatter = CreateFormatter();
                return _formatter; 
            }
			[DebuggerHidden]
			set { _formatter = value; }
		}

        private bool ShouldSerializeFormatter() {
            return false;
            //return !Formatter.Equals(Formatter);
        }

		private void ResetFormatter()
		{
			Formatter = CreateFormatter();
		}

		private DocumentRowCollection _rows;

		[Browsable(false)]
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public DocumentRowCollection Rows
		{
			[DebuggerHidden]
			get { return _rows; }
			[DebuggerHidden]
			internal set { _rows = value; }
		}

		[Editor("System.ComponentModel.Design.MultilineStringEditor, System.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a", 
			typeof(UITypeEditor)), Localizable(true)
		]
		public string Text
		{
			[DebuggerHidden]
			get { return Rows.ToString(); }
			[DebuggerHidden]
			set { Rows.Init(value); }
		}

		private bool ShouldSerializeText()
		{
			return !string.IsNullOrEmpty(Text);
		}

		private void ResetText()
		{
			Text = String.Empty;
		}

		internal void TextUpdated()
		{
			foreach (IView view in InternalViews)
				view.TextUpdated();
		}

		internal void ResetViewsInfo()
		{
			_undoStack.Clear();
			_redoStack.Clear();

			foreach (IView view in InternalViews)
				view.ResetViewInfo();

			IsDirty = false;
		}

		/// <summary>
		/// Возвращает фрагмент текста (в виде строки) соотвествующий 
		/// переданным координатам.
		/// </summary>
		/// <param name="startPosition">
		/// Начальная координата фрагмента текста.
		/// </param>
		/// <param name="endPosition">
		/// Конечная координата фрагмента текста.
		/// </param>
		/// <returns>Текст находящийся по преданным координатам.</returns>
		public string GetText(Position<Document> startPosition, Position<Document> endPosition)
		{
			if (startPosition > endPosition)
				Utils.Swap(ref startPosition, ref endPosition);

			// Если строка одна просто выделяем из нее текст.
			if (startPosition.Line == endPosition.Line)
				return Rows[startPosition.Line].Text.Substring(
					startPosition.Character,
					endPosition.Character - startPosition.Character);

			// Получам список строк соотвествющих позициям.
			DocumentRow[] range = Rows.GetRange(startPosition.Line,
				endPosition.Line - startPosition.Line + 1);

			string[] lines = new string[range.Length];

			for (int i = 0; i < range.Length; i++)
				lines[i] = range[i].Text;

			if (startPosition.Character > 0)
				lines[0] = lines[0].Substring(startPosition.Character);

			lines[lines.Length - 1] = lines[lines.Length - 1].Substring(0,
				endPosition.Character);

			
			return string.Join(Environment.NewLine, lines);
		}

		internal Position<Document> InternalInsert(Position<Document> startPosition, string[] lines)
		{
			if (lines.Length == 0)
				throw new ArgumentException("lines == 0!");

			DocumentRow row = Rows[startPosition.Line];

			string insText = lines[0];
			string text = row.Text;
			int tailTextLen = text.Length - startPosition.Character;

			if (lines.Length == 1)
			{
				if (startPosition.Character == 0)
					insText = insText + text;
				else if (startPosition.Character == text.Length)
					insText = text + insText;
				else
					insText = text.Substring(0, startPosition.Character) + insText
						+ text.Substring(startPosition.Character, tailTextLen);

				Rows.RemoveAt(startPosition.Line);
				DocumentRow newRow = new DocumentRow(insText);
				newRow.StartStylerState = row.StartStylerState;
				Rows.Insert(startPosition.Line, newRow);
				return new Position<Document>(startPosition.Line,
					startPosition.Character + lines[0].Length);
			}

			int tailCharPos = lines[lines.Length - 1].Length;

			lines[0] = text.Substring(0, startPosition.Character) + insText;
			lines[lines.Length - 1] = lines[lines.Length - 1]
				+ text.Substring(startPosition.Character, tailTextLen);

			DocumentRow[] range = new DocumentRow[lines.Length];

			for (int i = 0; i < lines.Length; i++)
				range[i] = new DocumentRow(lines[i]);

			range[0].StartStylerState = row.StartStylerState;

			Rows.RemoveAt(startPosition.Line);
			Rows.InsertRange(startPosition.Line, range);

			return new Position<Document>(startPosition.Line + range.Length - 1, tailCharPos);
		}

		internal void InternalDelete(Position<Document> startPosition, Position<Document> endPosition)
		{
			// Вычислить что нужно удалять.
			// Сформировать разбиваемые строки (строки в которых удалена только 
			// часть текста, такие строки могут оказаться в начале и в конце.
			if (startPosition > endPosition)
				Utils.Swap(ref startPosition, ref endPosition);

			DocumentRow row = new DocumentRow(
				Rows[startPosition.Line].Text.Substring(0, startPosition.Character)
				+ Rows[endPosition.Line].Text.Substring(endPosition.Character));

			row.StartStylerState = Rows[startPosition.Line].StartStylerState;

			Rows.RemoveRange(startPosition.Line, endPosition.Line - startPosition.Line + 1);
			Rows.Insert(startPosition.Line, row);
		}

		#region DeleteBack

		/// <summary>
		/// Удаляет символ идущий непосредсвенно за переданной позицией.
		/// Заносит инвертированную команду в Undo-буфер.
		/// </summary>
		/// <param name="previousPosition">
		/// Позиция перед которой должен быть удален символ.
		/// </param>
		public void DeleteBack(Position<Document> nextPosition)
		{
			Position<Document> previousPosition = nextPosition;

			if (previousPosition.Character == 0)
			{
				if (previousPosition.Line == 0)
					return;

				previousPosition.Line--;
				previousPosition.Character =
					 Rows[previousPosition.Line].TextLength;
			}
			else
				previousPosition.Character--;

			ExecuteCommand(new DeleteCommand(this,
				previousPosition, nextPosition));
		}
		
		#endregion

		#region Delete

		/// <summary>
		/// Удаляет фрагмент текста.
		/// Заносит инвертированную команду в Undo-буфер.
		/// </summary>
		/// <param name="start">
		/// Начальная позиция удаляемого фрагмента теаста.
		/// </param>
		/// <param name="end">
		/// Конечная позиция удаляемого фрагмента теаста.
		/// </param>
		public void Delete(Position<Document> start, Position<Document> end)
		{
			if (start == end)
			{
				// Если курсор находится в конце строки...
				if (start.Character >=
					Rows[end.Line].TextLength)
				{
					// ... то сливаем эту и следующую строку.
					Position<Document> newEnd = start;
					newEnd.Character = 0;
					newEnd.Line++;
					if (newEnd.Line < Rows.Count)
						ExecuteCommand(new DeleteCommand(this,
							start, newEnd));
				}
				else
				{
					// ... иначе удаляем символ следующий за выделением.
					Position<Document> newEnd = start;
					newEnd.Character++;
					ExecuteCommand(new DeleteCommand(this,
						start, newEnd));
				}
			}
			else
				ExecuteCommand(new DeleteCommand(this,
					start, end));
		}
		
		#endregion

		#region Paste

		/// <summary>
		/// Замещает фрагмент текста новым фрагментом.
		/// Заносит инвертированную команду в Undo-буфер.
		/// </summary>
		/// <param name="text"></param>
		/// <param name="start"></param>
		/// <param name="end"></param>
		public void Replace(string text, Position<Document> start, Position<Document> end)
		{
			if (OnBeforeChangeText(text, start, end))
				return;

			ReplaceDirect(text, start, end);
		}

		public event EventHandler<BeforeChangeTexEventArgs> BeforeChangeText;

		protected bool OnBeforeChangeText(string text,
			Position<Document> selectionStart, Position<Document> selectionEnd)
		{
			if (BeforeChangeText != null)
			{
				BeforeChangeTexEventArgs eventArgs = new BeforeChangeTexEventArgs(
					text, selectionStart, selectionEnd);

				BeforeChangeText(this, eventArgs);

				if (eventArgs.Cancel)
					return true;
			}

			return false;
		}

		public void ReplaceDirect(string text,
			Position<Document> selectionStart, Position<Document> selectionEnd)
		{
			if (selectionStart == selectionEnd)
				Document.ExecuteCommand(
					new InsertCommand(this, text, selectionEnd));
			else
				Document.ExecuteCommand(
					new UpdateCommand(this, text,
					selectionStart, selectionEnd));
		}

		#endregion
	}
}
