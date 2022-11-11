// Команды - публичне методы управления состоянием вида.

using System;
using System.Drawing;
using System.Diagnostics;
using System.Windows.Forms;
using Rsdn.Editor.ObjectModel;
using Rsdn.Editor.Keyboard;

namespace Rsdn.Editor
{
	partial class View
	{
		#region Прокрутка.

		[DebuggerHidden, ViewCommand("Прокрутка на строку вверх")]
		public void ScrollLineUp() { VScroll(-1); }
		[DebuggerHidden, ViewCommand("Прокрутка на строку вниз")]
		public void ScrollLineDowd() { VScroll(1); }
		[DebuggerHidden, ViewCommand("Прокрутка на страницу вверх")]
		public void ScrollPageUp() { VScroll(-CalcPreviousPageRowCount()); }
		[DebuggerHidden, ViewCommand("Прокрутка на страницу вниз")]
		public void ScrollPageDowd() { VScroll(CalcCurrentPageRowCount()); }
		[DebuggerHidden, ViewCommand("Прокрутка в начало документа")]
		public void ScrollDocumentHome() { FirstVisibleRow = 0; }
		[DebuggerHidden, ViewCommand("Прокрутка в конец документа")]
		public void ScrollDocumentEnd()
		{
			VScroll(Rows.Count - CalcBottomPageRowCount());
		}

		[ViewCommand("Прокрутка на символ влево")]
		private void ScrollColLeft() { HScroll(-1); }
		[ViewCommand("Прокрутка на символ вправо")]
		private void ScrollColRight() { HScroll(1); }


		#endregion

		#region Навигация текстовым курсором.

		#region CaretRealLineHome[Extend] & CaretRealLineEnd[Extend]

		[DebuggerHidden, ViewCommand("Переход в начало абзаца")]
		public void CaretRealLineHome()
		{
			InternalCaretRealLineEndOrHome(false, true);
		}

		[DebuggerHidden, ViewCommand("Переход выделением до начала абзаца")]
		public void CaretRealLineHomeExtend()
		{
			InternalCaretRealLineEndOrHome(true, true);
		}

		[DebuggerHidden, ViewCommand("Переход в конец абзаца")]
		public void CaretRealLineEnd()
		{
			InternalCaretRealLineEndOrHome(false, false);
		}

		[DebuggerHidden, ViewCommand("Переход выделением до конца абзаца")]
		public void CaretRealLineEndExtend()
		{
			InternalCaretRealLineEndOrHome(true, false);
		}

		private void InternalCaretRealLineEndOrHome(bool isExtendSelection, bool isHome)
		{
			//TODO: Непротестировано!
			Position<Document> realPos = _selectionEndDocument;
			realPos.Character = isHome ? 0 : Document.Rows[realPos.Line].TextLength;

			Position<IView> virtPos = ToView(realPos);

			EnsureVisible(virtPos);
			SetSelection(isExtendSelection ? _selectionStartView : virtPos, virtPos);
		}
 
		#endregion

		#region CaretViewLineHome & CaretViewLineHomeExtend

		[DebuggerHidden, ViewCommand("Переход в начало строки")]
		public void CaretViewLineHome()
		{
			InternalCaretVirtualLineHome(false);
		}

		[DebuggerHidden, ViewCommand("Выделение до начала строки")]
		public void CaretViewLineHomeExtend()
		{
			InternalCaretVirtualLineHome(true);
		}

		private void InternalCaretVirtualLineHome(bool isExtendSelection)
		{
			Position<IView> virtPos = _selectionEndView;
			virtPos.Character = 0;

			SmartHomeNavigation(ref virtPos);

			EnsureVisible(virtPos);
			SetSelection(isExtendSelection ? _selectionStartView : virtPos, virtPos);
		}

		#region SmartHomeNavigation

		/// <summary>
		/// Корректирует виртуальную позицию так, чтобы она была или на первом
		/// непробельном символе виртуальной строки, или в ее начале (если 
		/// до этого позиция была на первом непробельном символе).
		/// </summary>
		/// <param name="virtPos">Корректируемая позиция.</param>
		private void SmartHomeNavigation(ref Position<IView> virtPos)
		{
			// Если строка начинается с пробельных символов, то переходим не к начале 
			// строки, а к первому непробельному символу.

			Row row = Rows[virtPos.Line];
			string text = row.Text;

			for (int i = 0; i < text.Length; i++)
			{
				if (!char.IsWhiteSpace(text[i]))
				{
					virtPos.Character = i;
					break;
				}
			}

			// Если мы и так были на первом непробельном символе, то встаем в начало строки.
			if (_selectionEndView.Character == virtPos.Character)
				virtPos.Character = 0;
		}

		#endregion
		
		#endregion

		#region CaretViewLineEnd & CaretViewLineEndExtend

		[DebuggerHidden, ViewCommand("Переход в конец строки")]
		public void CaretViewLineEnd()
		{
			InternalCaretVirtualLineEnd(false);
		}

		[DebuggerHidden, ViewCommand("Выделение до конца строки")]
		public void CaretViewLineEndExtend()
		{
			InternalCaretVirtualLineEnd(true);
		}

		private void InternalCaretVirtualLineEnd(bool isExtendSelection)
		{
			Position<IView> virtPos = _selectionEndView;
			virtPos.Character = Rows[virtPos.Line].TextLength;

			EnsureVisible(virtPos);
			SetSelection(isExtendSelection ? _selectionStartView : virtPos, virtPos);
		}
		
		#endregion

		#region CaretViewLineDown & CaretViewChangeLine

		[DebuggerHidden, ViewCommand("Переход на строку вниз")]
		public void CaretViewLineDown()
		{
			CaretViewChangeLine(false, 1);
		}

		[DebuggerHidden, ViewCommand("Переход на строку вниз с выделением")]
		public void CaretViewLineDownExtend()
		{
			CaretViewChangeLine(true, 1);
		}

		[DebuggerHidden, ViewCommand("Переход на строку вверх")]
		public void CaretViewLineUp()
		{
			CaretViewChangeLine(false, -1);
		}

		[DebuggerHidden, ViewCommand("Переход на строку вверх с выделением")]
		public void CaretViewLineUpExtend()
		{
			CaretViewChangeLine(true, -1);
		}

		public void CaretViewChangeLine(bool isExtendSelection, int lines)
		{
            // 1. Получаем координаты текущей виртуальной позиции.
			// 2. Сдвигаем координату на высоту текущей строки вверх.
			// 3. Рассчитываем новую виртуальную позицию.
            if (!_viewOwner.ShouldChangeLine(isExtendSelection, lines)) return;
            
			Position<IView> virtPos = _selectionEndView;

			virtPos.Line += lines;

			if (virtPos.Line < 0)
			{
				if (_selectionEndView.Line == 0)
					return;

				virtPos.Line = 0;
			}

			int lastLine = Rows.Count - 1;

			if (virtPos.Line > lastLine)
			{
				if (_selectionEndView.Line == lastLine)
					return;

				virtPos.Line = lastLine;
			}

			EnsureVisible(virtPos.Line);
			virtPos.Character = 0;

			Rectangle? nullableRect = ToLocation(virtPos);

			if (!nullableRect.HasValue)
			{
				if (lines < 0)
					CaretDocumentHome();
				else
					CaretDocumentEnd();
				return;
			}

			Point location = nullableRect.Value.Location;
			location.Y++;
			location.X = _caretPixelOffset;

			HitTestInfo info = HitTest(location, true);

			if (info.ViewArea == ViewArea.Text)
			{
				EnsureVisible(info.Position);
				SetSelection(isExtendSelection ? _selectionStartView : info.Position,
					info.Position, false);
			}
		}
		
		#endregion

		#region CaretDocumentHome[Extend] & CaretDocumentEnd[Extend]

		[DebuggerHidden, ViewCommand("Переход в начало документа")]
		public void CaretDocumentHome()
		{
			InternalCaretDocumentHomeOrEnd(false, true);
		}

		[DebuggerHidden, ViewCommand("Выделение до начала документа")]
		public void CaretDocumentHomeExtend()
		{
			InternalCaretDocumentHomeOrEnd(true, true);
		}

		[DebuggerHidden, ViewCommand("Переход в конец документа")]
		public void CaretDocumentEnd()
		{
			InternalCaretDocumentHomeOrEnd(false, false);
		}

		[DebuggerHidden, ViewCommand("Выделение до конца документа")]
		public void CaretDocumentEndExtend()
		{
			InternalCaretDocumentHomeOrEnd(true, false);
		}

		private void InternalCaretDocumentHomeOrEnd(bool isExtendSelection, bool isHome)
		{
			if (!Rows.IsEmpty)
			{
				Position<IView> virtPos = isHome ? new Position<IView>(0, 0)
					: virtPos = new Position<IView>(
							Rows.Count - 1, Rows.Last.TextLength);

				EnsureVisible(virtPos);
				SetSelection(isExtendSelection ? _selectionStartView : virtPos, virtPos);
			}
		}
		
		#endregion

		#region CaretLeft & CaretLeftExtend

		[DebuggerHidden, ViewCommand("Переход на символ влево")]
		public void CaretLeft()
		{
			InternalCaretLeft(false, 1);
		}

		[DebuggerHidden, ViewCommand("Выделение на символ влево")]
		public void CaretLeftExtend()
		{
			InternalCaretLeft(true, 1);
		}

		[DebuggerHidden, ViewCommand("Переход на слово влево")]
		public void CaretWordLeft()
		{
			TryMoveCaret(Document.Formatter.GetPreviousWordPosition(
				Document.Rows, _selectionEndDocument), false);
		}

		[DebuggerHidden, ViewCommand("Выделение на слово влево")]
		public void CaretWordLeftExtend()
		{
			TryMoveCaret(Document.Formatter.GetPreviousWordPosition(
				Document.Rows, _selectionEndDocument), true);
		}

		private void InternalCaretLeft(bool isExtendSelection, int charsOffset)
		{
			Debug.Assert(charsOffset > 0);

			Position<Document> realPos = _selectionEndDocument;

			if (isExtendSelection || _selectionStartDocument == _selectionEndDocument)
			{
				realPos.Character -= charsOffset;

				SkipPicture(ref realPos, false);

				if (realPos.Character < 0)
				{
					if (realPos.Line == 0)
						realPos.Character = 0;
					else
					{
						realPos.Line--;
						realPos.Character = Document.Rows[realPos.Line].TextLength;
					}
				}
			}

			Position<IView> virtPos = ToView(realPos);
			HyphenationRow hypRow = Rows[virtPos.Line] as HyphenationRow;

			if (hypRow != null && !hypRow.IsEndLine
				&& virtPos.Character == hypRow.TextLength)
			{
				virtPos.Line++;
				virtPos.Character = 0;
			}

			SetSelection(isExtendSelection ? _selectionStartView : virtPos, virtPos);
		}

		#endregion

		#region CaretRight & CaretRightExtend

		[DebuggerHidden, ViewCommand("Переход на символ вправо")]
		public void CaretRight()
		{
			InternalCaretRight(false, 1);
		}

		[DebuggerHidden, ViewCommand("Выделение на символ вправо")]
		public void CaretRightExtend()
		{
			InternalCaretRight(true, 1);
		}

		[DebuggerHidden, ViewCommand("Переход на слово вправо")]
		public void CaretWordRight()
		{
			TryMoveCaret(Document.Formatter.GetNextWordPosition(
				Document.Rows, _selectionEndDocument), false);
		}

		private void TryMoveCaret(Position<Document> position, bool isExtendSelection)
		{
			if (position == _selectionEndDocument)
				return;

			Position<IView> virtPos = ToView(position);

			if (isExtendSelection)
				SetSelection(_selectionStartView, virtPos);
			else
				SetSelection(virtPos, virtPos);
		}

		[DebuggerHidden, ViewCommand("Выделение на слово вправо")]
		public void CaretWordRightExtend()
		{
			TryMoveCaret(Document.Formatter.GetNextWordPosition(
				Document.Rows, _selectionEndDocument), true);
		}

		private void InternalCaretRight(bool isExtendSelection, int charsOffset)
		{
			Debug.Assert(charsOffset > 0);

			Position<Document> realPos = _selectionEndDocument;
			DocumentRow row = Document.Rows[realPos.Line];

			if (isExtendSelection || _selectionEndDocument == _selectionStartDocument)
			{
				realPos.Character += charsOffset;
				SkipPicture(ref realPos, true);

				if (realPos.Character > Document.Rows[realPos.Line].TextLength)
				{
					if (realPos.Line == Document.Rows.Count - 1)
					{
						realPos.Character = Document.Rows[realPos.Line].TextLength - 1;
						if (realPos.Character < 0)
							realPos.Character = 0;
					}
					else
					{
						realPos.Character = 0;
						realPos.Line++;
					}
				}
			}
			
			Position<IView> virtPos = ToView(realPos);
			HyphenationRow hypRow = Rows[virtPos.Line] as HyphenationRow;

			if (hypRow != null && !hypRow.IsEndLine
				&& virtPos.Character == hypRow.TextLength)
			{
				virtPos.Line++;
				virtPos.Character = 0;
			}

			SetSelection(isExtendSelection ? _selectionStartView : virtPos, virtPos);
		}

		#endregion

		#region SkipPicture

		/// <summary>
		/// Пропускает подстроку которой при отображении соотвествует картинка.
		/// </summary>
		/// <param name="realPos">
		/// Позиция в кодумете которая проверяется на пересечение с картинкой,
		/// и если необходимо модифицируется, так чтобы не попадать на картинку.
		/// </param>
		/// <param name="isNext">
		/// Направление. Если true устанавливать realPos на позицию следующую 
		/// за картинокой. Иначе на позицию где начинается картинка.
		/// </param>
		private void SkipPicture(ref Position<Document> realPos, bool isNext)
		{
			DocumentRow row = Document.Rows[realPos.Line];

			if (row.AppliedStyles.Length > 0 && realPos.Character > 0)
			{
				StyledRange[] ranges = Utils.ToStyledRange(row.AppliedStyles, 
					DefaultStyle, row.Text, false);

				int pos = StyledRange.BinarySearch(ranges, realPos.Character);

				if (pos < 0)
				{
					pos = ~pos - 1;
					if (pos < 0)
						pos = 0;

					StyledRange range = ranges[pos];
					if (range.Style.DisplayImageHandler != null)
						if (isNext)
							realPos.Character = ranges[pos].EndPos + 1;
						else
							realPos.Character = ranges[pos].StartPos;
				}
			}
		}
		
		#endregion

		[DebuggerHidden, ViewCommand("Переход на страницу вверх")]
		public void PageUp() { InternalPageUp(false); }
		[DebuggerHidden, ViewCommand("Переход на страницу вниз")]
		public void PageDowd() { InternalPageDowd(false); }
		[DebuggerHidden, ViewCommand("Выделение на страницу вверх")]
		public void PageUpExtend() { InternalPageUp(true); }
		[DebuggerHidden, ViewCommand("Выделение на страницу вниз")]
		public void PageDowdExtend() { InternalPageDowd(true); }

		private void InternalPageUp(bool isExtendSelection)
		{
			int offset = -CalcPreviousPageRowCount();
			VScroll(offset);
			CaretViewChangeLine(isExtendSelection, offset);
		}

		private void InternalPageDowd(bool isExtendSelection)
		{
			int offset = CalcCurrentPageRowCount();
			VScroll(offset);
			CaretViewChangeLine(isExtendSelection, offset);
		}


		[DebuggerHidden, ViewCommand("Выделение всего документа")]
		public void SelectAll()
		{
			int end = Rows.Count - 1;
			
			if (end < 0)
				return;

			SetSelection(
				new Position<IView>(0, 0), 
				new Position<IView>(end, Rows[end].TextLength));
			EnsureVisible(_selectionEndView);
		}

		#endregion
		
		#region EnsureVisible (Заставляет сделать строку видимой на экране).

		/// <summary>
		/// Проверят видима ли строка, переданная в качестве параметра. Если 
		/// строка не видна во view, то она делается видимой. Если строка
		/// уже видна во view, то ничего не делается.
		/// </summary>
		/// <param name="virtualLine">Строка, которая должна быть видимой.</param>
		/// <returns>True, если был произведен скролинг.</returns>
		public bool EnsureVisible(int virtualLine)
		{
			if (virtualLine >= Rows.Count)
				throw new ArgumentOutOfRangeException("virtualLine");

			// Проверить, не видна ли строка на экране в данный момент.
			// Если не видна, прокрутить вид так, чтобы она стала видна.
			// Иначе ничего не делаем.

			if (virtualLine < FirstVisibleRow)
			{
				FirstVisibleRow = virtualLine;
				return true;
			}

			int pageRowCount = CalcCurrentPageRowCount();

			if (virtualLine >= FirstVisibleRow + pageRowCount)
			{
				FirstVisibleRow = virtualLine - pageRowCount + 1;
				return true;
			}

			return false;
		}

		/// <summary>
		/// Проверят, видима ли позиция, переданная в качестве параметра. Если 
		/// позиция не видна во view, то она делается видимой. Если позиция 
		/// уже видна во view, то ничего не делается.
		/// </summary>
		/// <param name="virtualLine">Позиция, которая должна быть видимой.</param>
		/// <returns>True, если был произведен скролинг.</returns>
		public bool EnsureVisible(Position<IView> position)
		{
			int newFirstVisibleCol = FirstVisibleCol;
			bool result = EnsureVisible(position.Line);
			Rectangle rect = ToLocation(position) ?? default(Rectangle);

			if (rect.Right > TextAreaRectangle.Right)
			{
				int offset = rect.Right - TextAreaRectangle.Right;
				newFirstVisibleCol = FirstVisibleCol + 6 + offset / HorizontalScrollStep;
				result = true;
			}
			else if (rect.X < TextAreaRectangle.X)
			{
				int offset = TextAreaRectangle.X - rect.X;
				int firstVisibleCol = FirstVisibleCol
					- (4 + offset / HorizontalScrollStep);
				newFirstVisibleCol = firstVisibleCol < 0 ? 0 : firstVisibleCol;
				result = true;
			}

			if (result && newFirstVisibleCol != FirstVisibleCol)
			{
				FirstVisibleCol = newFirstVisibleCol;
				return true;
			}

			return false;
		}

		/// <summary>
		/// Проверят, видима ли позиция, переданная в качестве параметра. Если 
		/// позиция не видна во view, то она делается видимой. Если позиция 
		/// уже видна во view, то ничего не делается.
		/// </summary>
		/// <param name="virtualLine">Позиция, которая должна быть видимой.</param>
		/// <returns>True, если был произведен скролинг.</returns>
		public bool EnsureVisible(Position<Document> position)
		{
			return EnsureVisible(ToView(position));
		}

		#endregion
	}
}
