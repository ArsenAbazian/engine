// Изменение состояния вида (пересчеты, скролинг и т.п.).

using System;
using System.Drawing;
using System.Windows.Forms;
using Rsdn.Editor.ObjectModel;
using System.Diagnostics;

namespace Rsdn.Editor
{
	partial class View
	{
		#region Size.

		private int _oldWidth;

		protected override void OnSizeChanged(EventArgs e)
		{
			if (Width > 0 && Width != _oldWidth && Rows != null && !Rows.IsEmpty)
			{
				using (new CaretHideHelper(this))
				{
					_oldWidth = Width;
					TextUpdated();
					RecalcSelection();
				}
			}
		}
		
		#endregion

		#region OnVisibleChanged

		protected override void OnVisibleChanged(EventArgs e)
		{
			RecalcCarrenCaretPosition();
			OnSelectionChanged(new SelectionChangedEventArgs(
				_selectionStartDocument, _selectionEndDocument, this));
		}
		
		#endregion

		#region Пересчет состояния при изменении размеров, содержимого и т.п.

		private void UpdateView()
		{
			OnViewChanged();

			Invalidate();
			Update();
		}

		public event EventHandler ViewChanged;

		protected void OnViewChanged()
		{
			if (ViewChanged != null)
				ViewChanged(this, EventArgs.Empty);

			if (Rows != null && Rows.Count > 0)
			{
				if (_firstVisibleVirtualRow < 0)
					_firstVisibleVirtualRow = 0;
				if (_firstVisibleVirtualRow >= Rows.Count)
					_firstVisibleVirtualRow = Rows.Count - 1;
			}
		}
		
		#endregion

		#region IView Members

		/// <summary>
		/// Производит пересчеты внутри представления.
		/// Нужно вызывать при изменении текста или состояния представления.
		/// </summary>
		public void TextUpdated()
		{
			if (Document == null)
				return;

			#region Запоминание верхней строки

			DocumentRow topRealRow;
			GetRealRowVisitor visitor = new GetRealRowVisitor();

			if (_rows != null && _rows.Count > FirstVisibleRow)
				topRealRow = _rows[FirstVisibleRow].RealDocumentRow;
			else
				topRealRow = null;

			#endregion

			if (WordWrap)
				// Преобразуем реальные строки в виртуальные.
				// При этом происходит разбиение на строки и организация 
				// схлопывающхися областей.
				_rows = Document.Formatter.DocumentToViewRows(this, Document.Rows);
			else
				(_rows = new RowCollection(Document.Rows.Count + 10)).AddRange(Document.Rows);

			#region Востановление верхней строки

			if (topRealRow != null)
			{
				// ... то ищим соотвествюующую ей строку в списке виртуальных строк.
				int topVirtRowIndex = _rows.IndexOfDocumentRow(topRealRow);
				if (topVirtRowIndex >= 0)
					_firstVisibleVirtualRow = topVirtRowIndex;
			}

			#endregion

			// Если документ обновился, то нужно проверить не выходит ли текущая позиция
			// View за пределы этого документа...
			if (FirstVisibleRow >= _rows.Count)
			{
				_firstVisibleVirtualRow = 0;
				_firstVisibleCol = 0;
			}

			RecalcSelection();
			UpdateView();
		}

		public void ResetViewInfo()
		{
			_firstVisibleVirtualRow = 0;
			_firstVisibleCol = 0;
			_selectionStartView = _selectionEndView = default(Position<IView>);
			_selectionEndDocument = _selectionStartDocument = default(Position<Document>);
			RecalcCarrenCaretPosition();
		}

		#region Zoom

		float _zoom = 1.0F;

		public float Zoom
		{
			get { return _zoom; }
			set
			{
				_zoom = value;
				TextUpdated();
			}
		}
 
		#endregion

		#endregion

		#region Вертикальный скролинг.

		#region VScroll (Относительная прокрутка).

		/// <summary>
		/// Прокручивает вид на rowsToScroll строк.
		/// </summary>
		/// <param name="rowsToScroll">
		/// Количество строк на которое можно прокрутить вид. 
		/// Может быть отрицательным.
		/// </param>
		public void VScroll(int rowsToScroll)
		{
			int topRow = FirstVisibleRow + rowsToScroll;
			int bottomRow = Rows.Count - CalcBottomPageRowCount();

			if (topRow < 0)
				FirstVisibleRow = 0;
			else if (topRow > bottomRow)
				FirstVisibleRow = bottomRow;
			else
				FirstVisibleRow = topRow;
		}

		private void HScroll(int colsToScroll)
		{
			if (WordWrap)
				return;

			int firstCol = FirstVisibleCol + colsToScroll;

			if (firstCol == FirstVisibleCol)
				return;

			if (firstCol > 0)
			{
				FirstVisibleCol = firstCol;
				return;
			}

			FirstVisibleCol = 0;
		}

		#endregion

		#region Вертикальный автоскролинг.

		/// <summary>
		/// Максимальная частота повторения прокрутки.
		/// </summary>
		const int SlowAutoScrollInterval = 300;
		/// <summary>
		/// Минимальная частота повторения прокрутки.
		/// </summary>
		const int FastAutoScrollInterval = 10;

		/// <summary>
		/// Вычисляет скорость прокрутки в зависимости от растояния
		/// до края текстовой области.
		/// </summary>
		/// <param name="distance">
		/// Рассояние между курсором мыши и текстовй областью.
		/// </param>
		/// <returns>Интервал прокрутки в милисикундах.</returns>
		private static int CalcScrollSpeed(int distance)
		{
			return distance - distance + 20;
		}

		/// <summary>
		/// Таймер срабатывающий когда вид находится в стостоянии автоматической
		/// прокрутки по вертикали.
		/// </summary>
		private Timer _vScrollTimer;

		private void InitScrolling()
		{
			_vScrollTimer = new Timer();
			_vScrollTimer.Enabled = false;
			_vScrollTimer.Interval = 10;
			_vScrollTimer.Tick += VScrollTimerTick;
		}

		/// <summary>
		/// Событие генерируемое таймером в режиме вертикальной автопрогтутки.
		/// </summary>
		void VScrollTimerTick(object sender, EventArgs e)
		{
			if (Capture)
			{
				Point location = PointToClient(Cursor.Position);
				// Если курсор выше текстовой области...
				if (location.Y < TextAreaRectangle.Top)
					using (new FreezeRepaintHelper(this))
					{
						ScrollLineUp();
						SetSelection(location);
					}
				// Если курсор ниже текстовой области.
				else if (location.Y > TextAreaRectangle.Bottom)
					using (new FreezeRepaintHelper(this))
					{
						ScrollLineDowd();
						SetSelection(location);
					}

				return;
			}

			// Если курсор по вертикали находится в текстовой области...
			_vScrollTimer.Stop();
		}

		#endregion

		#endregion
	}
}
