using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace Rsdn.Editor.ObjectModel
{
	partial class ViewScrollDecorator : ViewDecorator
	{
		public ViewScrollDecorator()
		{
			SetStyle(ControlStyles.ResizeRedraw, true);
			InitializeComponent();
		}

		protected override IView Child
		{
			get { return _activeView; }
			set { _activeView = (View)value; }
		}

		[DefaultValue(10)]
		public int HorizontalScrollOffset
		{
			get { return _activeView.HorizontalScrollStep; }
			set { _activeView.HorizontalScrollStep = value; }
		}

		const int SplitterGripSize = 6;

		protected override void OnSizeChanged(EventArgs e)
		{
			int sBarSize = _hScrollBar.Height;
			Rectangle clientRect = ClientRectangle;

			_hScrollBar.SetBounds(0, clientRect.Height - sBarSize,
				clientRect.Width - sBarSize, sBarSize);
			_vScrollBar.SetBounds(clientRect.Width - sBarSize, SplitterGripSize,
				sBarSize, clientRect.Height - sBarSize - SplitterGripSize);
			_activeView.SetBounds(0, 0, _vScrollBar.Left, _hScrollBar.Top);

			base.OnSizeChanged(e);
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);

			// Рисуем хваталку для сплитера
			ControlPaint.DrawButton(e.Graphics, GetSplitterBounds(),
				ButtonState.Normal);
		}

		Rectangle GetSplitterBounds()
		{
			int barSize = _hScrollBar.Height;
			Rectangle rect = ClientRectangle;
			rect.X = rect.Width - barSize;
			rect.Width = barSize;
			rect.Height = SplitterGripSize;
			return rect;
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			if (GetSplitterBounds().Contains(e.Location))
				OnSplitterGripClicked();

			base.OnMouseDown(e);
		}

		protected override void WndProc(ref Message m)
		{
			const int WM_SETCURSOR = 0x20;

			// Windows предлагает установить курсор...
			if (m.Msg == WM_SETCURSOR && m.WParam == Handle && (short)m.LParam == 1)
			{	// ...и курсор находится в позиции кнопки сплитера...
				if (GetSplitterBounds().Contains(PointToClient(Control.MousePosition)))
				{	// Устанавливаем курсор горизонтального сплитера.
					Cursor.Current = Cursors.HSplit;
					m.Result = (IntPtr)1;
					return;
				}
			}

			base.WndProc(ref m);
		}

		protected override void OnCursorChanged(EventArgs e)
		{
			base.OnCursorChanged(e);
		}

		protected virtual void OnSplitterGripClicked()
		{
			if (SplitterGripClicked != null)
				SplitterGripClicked(this, EventArgs.Empty);
		}

		public event EventHandler SplitterGripClicked;

		private void _activeView_ViewChanged(object sender, EventArgs e)
		{
			View view = (View)sender;

			_vScrollBar.Minimum = 0;
			_vScrollBar.Maximum = view.Rows.Count - 1;
			_vScrollBar.LargeChange =
				Math.Min(view.CalcBottomPageRowCount(),
				view.Rows.Count);
			_vScrollBar.SmallChange = 1;
			_vScrollBar.Enabled = view.Rows.Count - 1 > view.CalcTopPageRowCount();
			if (_activeView.FirstVisibleRow <= _vScrollBar.Maximum)
				_vScrollBar.Value = _activeView.FirstVisibleRow;

			if (_activeView.FirstVisibleCol <= _hScrollBar.Maximum)
				_hScrollBar.Value = _activeView.FirstVisibleCol;
			_hScrollBar.Enabled = _vScrollBar.Enabled;
		}

		private void _vScrollBar_Scroll(object sender, ScrollEventArgs e)
		{
			FirstVisibleRow = e.NewValue;
		}

		private void _hScrollBar_Scroll(object sender, ScrollEventArgs e)
		{
			FirstVisibleCol = e.NewValue;
		}
	}
}
