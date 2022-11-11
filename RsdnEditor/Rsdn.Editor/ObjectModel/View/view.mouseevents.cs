// Mouse Events.

using System.Windows.Forms;
using System.Drawing;
using System;
using System.Diagnostics;
using System.Reflection;
using System.Text;
using Rsdn.Editor.ObjectModel.Edit;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	partial class View
	{
		private void SetOldMouseLocation(Point newMouseLocation)
		{
			_oldMouseLocation = newMouseLocation;
		}

		private void ResetOldMouseLocation()
		{
			_oldMouseLocation = new Point(-1, -1);
		}

		private bool IsMouseMoved(Point mouseLocation)
		{
			Size dblClkSize = SystemInformation.DoubleClickSize;
			Point pt = _oldMouseLocation;
			Rectangle dblClicArea = new Rectangle(
				 pt.X - dblClkSize.Width / 2, pt.Y - dblClkSize.Height / 2,
				 dblClkSize.Width, dblClkSize.Height);

			return dblClicArea.Contains(mouseLocation);
		}

		#region OnMouseDown

		protected override void OnMouseDown(MouseEventArgs e)
		{

			// Данная функция эмулирует логику одинарного клика и 
			// двойного клика мышью. Такая эмуляция потребовалась, так-как
			// стандартный двойной клик срабатывает на отпускание мыши, а
			// для логики пословного выделения нужно отлавливать двойной клик
			// еще при отпускании мыши.

			// Промежуток времени в котором два нажатия кнопки мыши
			// считаются double click-ом.
			TimeSpan doubleClickTimeSpan = TimeSpan.FromMilliseconds(
				SystemInformation.DoubleClickTime);

			// Если с момента последнего нажатия кнопки мыши прошло времени
			// больше чем doubleClickTimeSpan...
			if (DateTime.Now - _previousClickTime <= doubleClickTimeSpan)
			{
				// Проверяем не сдвинулся ли курсор мыши из области в которой
				// двойное нажатие считается double click-ом...

				if (IsMouseMoved(e.Location))
				{
					// ... мышь не вышла. Попер double click...

					// Задаем заведомо неверную координату для _oldMouseLocation, чтобы
					// следующее нажатие кнопки мыши случайно небыло воспринято как
					// double click.
					ResetOldMouseLocation();

					InternalMouseDoubleClick(e);

					base.OnMouseDown(e);

					return;
				}
			}

			// Мышь вышла из области double click-а. Попер обычный click.

			// Заполняем переменные служащие для отслеживания double click-а.
			SetOldMouseLocation(e.Location);
			_previousClickTime = DateTime.Now;

			InternalMouseDown(e);
				
			base.OnMouseDown(e);
	}
 
		#endregion

		private DateTime _previousClickTime;
		private Point _oldMouseLocation = new Point(-1, -1);
		private bool _selectByWordMode = false;

		protected void InternalMouseDown(MouseEventArgs e)
		{
			Debug.WriteLineIf(TraceMouse.TraceVerbose, "InternalMouseDown: Click");
			Capture = true;

			HitTestInfo info = HitTest(e.Location, true);

			if (info.ViewArea == ViewArea.Text)
			{
				if (Control.ModifierKeys == Keys.Shift)
				{
					SetSelection(_selectionStartView, info.Position);
				}
			
				if (Utils.Between(_selectionStartView, _selectionEndView, info.Position)
					&& _selectionStartView != _selectionEndView)
				{
					_beforeDrag = true;
					_brginDragTimer.Start();
					SetOldMouseLocation(e.Location);
					return;
				}

				SetSelection(info.Position, info.Position);
			}
		}

		private bool _beforeDrag;

		protected void InternalMouseDoubleClick(MouseEventArgs e)
		{
			Capture = true;

			Range<Document> documentRange = Document.Formatter.GetWordRange(
				Document.Rows, _selectionEndDocument);

			Range<IView> range = ToView(documentRange);
			SetSelection(range.Start, range.End);
			_selectByWordMode = true;
			Debug.WriteLineIf(TraceMouse.TraceVerbose, "InternalMouseDoubleClick: _selectByWordMode = true;");
		}

		protected override void OnMouseWheel(MouseEventArgs e)
		{
			int rowsToScroll = -e.Delta
				* SystemInformation.MouseWheelScrollLines / 120;

			VScroll(rowsToScroll);
		}

		private void _brginDragTimer_Tick(object sender, EventArgs e)
		{
			_brginDragTimer.Stop();

			if (_beforeDrag)
			{
				_beforeDrag = false;
				BeginDrag(Control.MousePosition);
			}
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			if (Capture)
			{
				Capture = false;
				Debug.WriteLineIf(TraceMouse.TraceVerbose, string.Format("OnMouseUp: _selectByWordMode = {0};", _selectByWordMode));

				if (_beforeDrag)
				{
					_beforeDrag = false;
					_brginDragTimer.Stop();

					Position<IView> position = ToView(e.Location);
					SetSelection(position, position);
					base.OnMouseUp(e);
					return;
				}

				if (!_selectByWordMode)
					SetSelection(e.Location);
			}

			_selectByWordMode = false;
			
			base.OnMouseUp(e);
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
			if (Capture)
			{
				#region Capture == true

				if (_beforeDrag)
				{
					_beforeDrag = false;

					if ((Control.ModifierKeys & Keys.Shift) == 0)
					{
						_brginDragTimer.Stop();
						BeginDrag(e.Location);
						return;
					}
				}


				if (e.Location.Y < TextAreaRectangle.Top)
				{
					_vScrollTimer.Interval =
						CalcScrollSpeed(TextAreaRectangle.Top - e.Location.Y);
					_vScrollTimer.Start();
					return;
				}

				if (e.Location.Y > TextAreaRectangle.Bottom)
				{
					_vScrollTimer.Interval =
						CalcScrollSpeed(e.Location.Y - TextAreaRectangle.Bottom);
					_vScrollTimer.Start();
					return;
				}

				if (_selectByWordMode)
				{
					Position<IView> end = ToView(e.Location);
					Range<IView> range = ToView(Document.Formatter.GetWordRange(
						Document.Rows, ToDocument(end)));

					end = _selectionStartView > end ? range.Start : range.End;

					if (end != _selectionEndView)
						SetSelection(_selectionStartView, end);
				}
				else
					SetSelection(e.Location, true);

				#endregion
			}
			else
			{
				#region Capture == false
				
				if (_selectionStartView != _selectionEndView
					&& !TextAreaRectangle.Contains(e.Location))
				{
					if (this.Cursor != InverseArrawCursor)
						this.Cursor = InverseArrawCursor;
					return;
				}

				Position<IView> current = ToView(e.Location);

				Cursor cursor = 
					Utils.Between(_selectionStartView, _selectionEndView, current)
					&& _selectionStartView != _selectionEndView
					? Cursors.Arrow : Cursors.IBeam;

				if (cursor != this.Cursor)
					this.Cursor = cursor;
 
				#endregion
			}

			base.OnMouseMove(e);
		}

		private System.Windows.Forms.Timer _brginDragTimer;
	}
}
