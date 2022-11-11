using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Runtime.InteropServices;

namespace Rsdn.Editor
{
	/// <summary>
	/// Platform Abstraction Layer (слой абстракции от платформы).
	/// В этом классе собраны методы зависимые от платформы.
	/// </summary>
	internal class WindowsPAL : IPAL
	{
		internal WindowsPAL()
		{
		}

		#region Windows Externs

		#region Функции по работе с кареткой (текстовым курсором)

		[DllImport("User32.dll", SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		protected static extern bool ShowCaret(IntPtr hWnd);

		[DllImport("User32.dll", SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		protected static extern bool HideCaret(IntPtr hWnd);

		[DllImport("User32.dll", SetLastError = true, EntryPoint = "DestroyCaret")]
		[return: MarshalAs(UnmanagedType.Bool)]
		protected static extern bool InternalDestroyCaret();

		[DllImport("user32.dll", SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		static extern bool SetCaretPos(int X, int Y);

		[DllImport("user32.dll", SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		static extern bool CreateCaret(IntPtr hWnd, IntPtr hBitmap,
			int nWidth, int nHeight);

		#endregion

		#region Windows Messages

		enum Msg : uint
		{
			WM_SETREDRAW = 11,
		}

		#endregion

		#region SendMessage

		[DllImport("user32.dll", CharSet = CharSet.Auto)]
		static extern IntPtr SendMessage(IntPtr hWnd, Msg msg, IntPtr wParam,
			IntPtr lParam);

		//Overload for string lParam (e.g. WM_GETTEXT)
		[DllImport("user32.dll", CharSet = CharSet.Auto)]
		static extern IntPtr SendMessage(IntPtr hWnd, Msg msg, IntPtr wParam,
			[Out] StringBuilder lParam);

		#endregion

		#endregion

		#region IDisposable Members

		public void Dispose() { }

		#endregion

		#region IPAL Members

		Control _control;

		public void Initialization(Control control)
		{
			_control = control;
		}

		#region Функции по работе с кареткой (текстовым курсором)

		public void ShowCaret()
		{
			ShowCaret(_control.Handle);
		}

		public void HideCaret()
		{
			HideCaret(_control.Handle);
		}

		public void DestroyCaret()
		{
			InternalDestroyCaret();
		}

		void IPAL.SetCaretPos(int x, int y)
		{
			SetCaretPos(x, y);
		}

		public void SetCaretPos(Point location)
		{
			SetCaretPos(location.X, location.Y);
		}

		public void CreateCaret(int width, int height)
		{
			CreateCaret(_control.Handle, IntPtr.Zero, width, height);
		}

		#endregion

		#region FreezeRepaint

		public void FreezeRepaint()
		{
			SendMessage(_control.Handle, Msg.WM_SETREDRAW, (IntPtr)0, IntPtr.Zero);
		}

		public void UnfreezeRepaint()
		{
			SendMessage(_control.Handle, Msg.WM_SETREDRAW, (IntPtr)1, IntPtr.Zero);
		}

		#endregion

		#endregion
	}
}
