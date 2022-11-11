// Работа с выделением (Selection-ом).

using System;
using System.Diagnostics;
using System.Drawing;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	partial class View
	{
		private Position<IView> _selectionStartView = default(Position<IView>);
		private Position<IView> _selectionEndView = default(Position<IView>);
		private Position<Document> _selectionStartDocument = 
			default(Position<Document>);
		private Position<Document> _selectionEndDocument = 
			default(Position<Document>);

		[DebuggerHidden]
		private void InitSelection()
		{
			_selection = new SelectionImpl(this);
		}

		private void RecalcSelection()
		{
			_selectionStartView = ToView(_selectionStartDocument);
			_selectionEndView = ToView(_selectionEndDocument);
		}

		#region ISelection property

		private ISelection _selection;

		/// <summary>
		/// Предоставляет публичный интерфейс к выделению.
		/// </summary>
		public ISelection Selection
		{
			[DebuggerHidden]
			get { return _selection; }
		}

		private class SelectionImpl : ISelection
		{
			[DebuggerHidden]
			public SelectionImpl(View view)
			{
				_view = view;
			}

			View _view;

			#region ISelection Members

			public Position<Document> Start
			{
				[DebuggerHidden]
				get { return _view._selectionStartDocument; }
				set
				{
					Position<IView> virtStart = _view.ToView(value);
					_view.SetSelection(virtStart, _view._selectionEndView);
				}
			}

			public Position<Document> End
			{
				[DebuggerHidden]
				get { return _view._selectionEndDocument; }
				set
				{
					Position<IView> virtEnd = _view.ToView(value);
					_view.SetSelection(_view._selectionStartView, virtEnd);
				}
			}

			public Range<Document> Range
			{
				[DebuggerHidden]
				get { return new Range<Document>(Start, End); }
				set
				{
					Position<IView> virtStart = _view.ToView(value.Start);
					if (value.Start == value.End)
					{
						_view.SetSelection(virtStart, virtStart);
						return;
					}

					Position<IView> virtEnd = _view.ToView(value.End);
					_view.SetSelection(virtStart, virtEnd);
				}
			}


			public void SetSelection(Position<Document> start, Position<Document> end)
			{
				Position<IView> virtStart = _view.ToView(start);
				Position<IView> virtEnd = start == end ? virtStart : _view.ToView(end);
				_view.SetSelection(virtStart, virtEnd);
			}

			[DebuggerHidden]
			public void SetSelection(Position<IView> start, Position<IView> end)
			{
				_view.SetSelection(start, end);
			}

			public string Text
			{
				[DebuggerHidden]
				get { return _view.Document.GetText(Start, End); }
				set { _view.Document.Replace(value, Start, End); }
			}
		}

		#endregion ISelection Members

		#endregion ISelection property

		/// <summary>
		/// Позволяет задать выделение. Обратите внимание, что selectionEnd должен
		/// указывать на смвол идущий за последним сиволом выделения.
		/// Так пустое выделение можно задать установив позицю:
		/// ((L:0, C:0), (L:0, C:0)).
		/// Выделение нулевого символа нулевой строки:
		/// ((L:0, C:0), (L:0, C:1)).
		/// Выделение нулевой строки вместе с концом строки:
		/// ((L:0, C:0), (L:1, C:0)).
		/// </summary>
		/// <param name="selectionStart">
		/// Позиция первого символа выделения.</param>
		/// <param name="selectionEnd">
		/// Позиция за последним символом выделения.</param>
		private bool SetSelection(
			Position<IView> selectionStart, 
			Position<IView> selectionEnd)
		{
			return SetSelection(selectionStart, selectionEnd, true);
		}

		/// <summary>
		/// Позволяет задать выделение. Обратите внимание, что selectionEnd должен
		/// указывать на смвол идущий за последним сиволом выделения.
		/// </summary>
		/// <param name="selectionStart">
		/// Позиция первого символа выделения.</param>
		/// <param name="selectionEnd">
		/// Позиция за последним символом выделения.</param>
		/// <param name="updateCaretOffset"></param>
		bool SetSelection(
			Position<IView> selectionStart,
			Position<IView> selectionEnd, 
			bool updateCaretOffset)
		{
			Position<Document> selectionStartReal = ToDocument(selectionStart);
			Position<Document> selectionEndReal = ToDocument(selectionEnd);
			SelectionChangingEventArgs e = new SelectionChangingEventArgs(
				selectionStartReal, selectionEndReal, this);

			OnSelectionChanging(e);

			if (e.Cancel)
				return false;

			using (_imageAnimatorHelper.Freeze())
			{
				Position<IView> carretPos = selectionEnd;

				TestPosition(selectionStart, "selectionStart");
				TestPosition(selectionEnd, "selectionEnd");

				if (_selectionStartView == selectionStart 
					&& _selectionEndView == selectionEnd)
					return true;

				bool isNeedRepaint = false;

				// Если до этого было выделение, то удаляем соответствующие стили.
				if (_selectionStartView != _selectionEndView)
					isNeedRepaint = true;

				_selectionStartView = selectionStart;
				_selectionEndView = selectionEnd;

				_selectionStartDocument = selectionStartReal;
				_selectionEndDocument = selectionEndReal;

				// Если требуется добавляем стили выделения к строкам.
				if (selectionStart != selectionEnd)
					isNeedRepaint = true;

				if (!EnsureVisible(selectionEnd) && isNeedRepaint && !IsFreezeRepaint)
				{
						Invalidate();
						Update();
				}

				if (updateCaretOffset)
					UpdateCaretOffset(carretPos);

				if (!IsFreezeRepaint)
					SetCarretPosition(selectionEnd);

				OnSelectionChanged(new SelectionChangedEventArgs(
					selectionStartReal, selectionEndReal, this));
			}

			return true;
		}

		private void SetSelection(Point location, bool checkChange)
		{
			HitTestInfo info = HitTest(location, true);

			if (info.ViewArea == ViewArea.Text)
			{
				if (checkChange && info.Position == _selectionEndView)
					return;

				SetSelection(_selectionStartView, info.Position);
			}
		}

		private Position<IView> ToView(Point location)
		{
			HitTestInfo info = HitTest(location, true);

			if (info.ViewArea != ViewArea.Text)
				throw new ArgumentOutOfRangeException("location");

			return info.Position;
		}

		private void SetSelection(Point location)
		{
			SetSelection(location, false);
		}

		/// <summary>
		/// Отступ в пикселях от начала виртуальной строки.
		/// Этот отступ используется для рессчета позиции курсора при 
		/// переходе курсора на другую строку.
		/// </summary>
		private int _caretPixelOffset;

		/// <summary>
		/// Обновляет отсуп в пикселях относительно начала виртуальной строки.
		/// Этот отступ используется для рессчета позиции курсора при 
		/// переходе курсора на другую строку.
		/// </summary>
		/// <param name="viewPosition">
		/// Виртуальная позция для которой нужно рассчитать оступ.
		/// </param>
		private void UpdateCaretOffset(Position<IView> viewPosition)
		{
			Rectangle rect = ToLocation(viewPosition) ?? default(Rectangle);
			_caretPixelOffset = rect.X;
		}

		SimpleStyle _selectionStyle = new SelectionStyle();
	}
}
