using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Microsoft.Win32.SafeHandles;
using System.ComponentModel;
using System.Security;
using Rsdn.Interop;

namespace Rsdn.Editor
{
	public class RsdnGraphics : IDisposable
	{
		public RsdnGraphics(IDeviceContext dc)
		{
			_dc = dc;

			if (dc == null)
				throw new ArgumentNullException("dc");

			_hdc = _dc.GetHdc();

			if (_hdc == IntPtr.Zero)
				throw new Exception("Can't obtain the HDC.");

			_savedDc = SaveDC(_hdc);

			if (_savedDc == 0)
				throw new Exception("Can't save DC state.");

		}

		public RsdnGraphics(IDeviceContext dc, Rectangle paintRect)
		{
			_dc = dc;

			if (dc == null)
				throw new ArgumentNullException("dc");

			_hdc = _dc.GetHdc();

			if (_hdc == IntPtr.Zero)
				throw new Exception("Can't obtain the HDC.");

			_doubleBuffering = new DoubleBuffering(_hdc, paintRect);
			_hdc = _doubleBuffering.Hdc;

			_savedDc = SaveDC(_hdc);

			if (_savedDc == 0)
				throw new Exception("Can't save DC state.");

		}

		protected DoubleBuffering _doubleBuffering;

		#region SafeHdcHandle
		/// <summary>
		/// Вспомогательный класс для работы с неуправляемыми хендлами DC
		/// </summary>
		class SafeHdcHandle : SafeHandleZeroOrMinusOneIsInvalid
		{
			public SafeHdcHandle(IDeviceContext dc)
				: base(true)
			{
				SetHandle(dc.GetHdc());
				_dc = dc;
			}

			public static implicit operator IntPtr(SafeHdcHandle hdc)
			{
				return hdc.handle;
			}

			protected override bool ReleaseHandle()
			{
				_dc.ReleaseHdc();
				return true;
			}

			IDeviceContext _dc;
		}
		#endregion

		#region Методы

		public void SetClip(Rectangle rect)
		{
			IntPtr region = Api.CreateRectRgn(
				rect.Left, rect.Top, rect.Right, rect.Bottom);

			if (region == IntPtr.Zero)
				throw new Win32Exception();

			Api.SelectClipRgn(_hdc, region);
			Api.DeleteObject(region);
		}

		/// <summary>
		/// Рисует прямоугольник заданным цветом. Т.е. вывод текста на фоне, но без текста.
		/// </summary>
		/// <param name="rect">Прямоугольник для закрашивания</param>
		/// <param name="backColor">Цвет для закрашивания</param>
		public void FillSolidRect(Rectangle rect, Color backColor)
		{
			RECT cr = rect;

			SetBkColor(_hdc, ColorTranslator.ToWin32(backColor));

			if (!ExtTextOut(_hdc, 0, 0, 2 /* ETO_OPAQUE */, ref cr, null, 0, null))
				throw new Win32Exception();
		}

		public void DrawText(string text, ZoomedFont font,
			Point pt, Color foreColor, Color backColor)
		{
			DrawText(_hdc, text, font, pt, foreColor, backColor);
		}

		protected void DrawText(IntPtr hdc, string text, ZoomedFont font,
			Point pt, Color foreColor, Color backColor)
		{
			if (_prevFont != font)
			{
				_prevFont = font;
				IntPtr hFont = GetHFont(font);
				SelectObject(hdc, hFont);
			}
			//Debug.Assert(foreColor != Color.Empty && backColor != Color.Empty);

			SetTextColor(hdc, ColorTranslator.ToWin32(foreColor));
			SetBkColor(hdc, ColorTranslator.ToWin32(backColor));
			TextOut(hdc, pt.X, pt.Y, text, text.Length);
		}

		public Size GetCharSize(IDeviceContext dc, char ch, ZoomedFont font)
		{
			return GetCharSize(_hdc, ch, font);
		}

		public Size GetStringSize(string text, ZoomedFont font)
		{
			return GetStringSize(_hdc, text, font);
		}

		protected Size GetCharSize(IntPtr hdc, char ch, ZoomedFont font)
		{
			SIZE size;

			if (_prevFont != font)
			{
				IntPtr hFont = GetHFont(font);
				SelectObject(hdc, hFont);
				_prevFont = font;
			}

			if (GetTextExtentPoint32(hdc, ref ch, 1, out size))
				return size;

			throw new Win32Exception();
		}

		protected Size GetStringSize(IntPtr hdc, string text, ZoomedFont font)
		{
			//return new Size(30, 11);


			//FontHelper fontHelper = FontHelper.GetFontHelper(_font, 1);
			//return new Size(fontHelper.MeasureWidth(text), fontHelper.Height);

			SIZE size;

			if (_prevFont != font)
			{
				_prevFont = font;
				IntPtr hFont = GetHFont(font);
				SelectObject(hdc, hFont);
			}

			if (GetTextExtentPoint32(hdc, text, text.Length, out size))
				return size;

			throw new Win32Exception();
		}

		public Size MeasureString(string text, ZoomedFont font)
		{
			return GetStringSize(text, font);
		}


		//public unsafe Size MeasureString(
		//  char[] chars, int startPos, int len, Font font)
		//{
		//  //int arrayLen = chars.Length;

		//  //if (startPos >= arrayLen)
		//  //  throw new ArgumentOutOfRangeException("startPos");

		//  //if (startPos + len > arrayLen)
		//  //  throw new ArgumentOutOfRangeException("len");

		//  SIZE size;

		//  if (_prevFont != font)
		//  {
		//    IntPtr hFont = GetHFont(font);
		//    SelectObject(_hdc, hFont);
		//    _prevFont = font;
		//  }

		//  fixed (char* p = &chars[0])
		//    if (GetTextExtentPoint32(_hdc, p + startPos, len, out size))
		//      return size;

		//  throw new Win32Exception();
		//}

		public Size GetCharSize(char ch, ZoomedFont font)
		{
			return GetCharSize(_hdc, ch, font);
		}


		protected int _savedDc;

		protected ZoomedFont _prevFont;
		protected IntPtr _hdc;

		protected IDeviceContext _dc;

		protected Dictionary<ZoomedFont, IntPtr> _fontHandlerMap =
			new Dictionary<ZoomedFont, IntPtr>();

		protected IntPtr GetHFont(ZoomedFont font)
		{
			IntPtr hFont;

			if (_fontHandlerMap.TryGetValue(font, out hFont))
				return hFont;

			return _fontHandlerMap[font] = font.ToFont().ToHfont();
		}

		#endregion

		#region Нереализованная фигня из TextRenderer

		//public static void DrawText(IDeviceContext dc, string text, Font _font, 
		//  Point pt, Color foreColor);
		//public static void DrawText(IDeviceContext dc, string text, Font _font, 
		//  Rectangle bounds, Color foreColor);
		//public static void DrawText(IDeviceContext dc, string text, Font _font, 
		//  Point pt, Color foreColor, TextFormatFlags flags);
		//public static void DrawText(IDeviceContext dc, string text, Font _font, 
		//  Rectangle bounds, Color foreColor, Color backColor);
		//public static void DrawText(IDeviceContext dc, string text, Font _font, 
		//  Rectangle bounds, Color color, TextFormatFlags flags);
		//public static void DrawText(IDeviceContext dc, string text, Font _font, 
		//  Point pt, Color foreColor, Color backColor, TextFormatFlags flags);
		//public static void DrawText(IDeviceContext dc, string text, Font _font, 
		//  Rectangle bounds, Color foreColor, Color backColor, TextFormatFlags flags);

		#endregion

		#region Interop

		/// <summary>
		/// The SaveDC function saves the current state of the specified device 
		/// context (DC) by copying data describing selected objects and graphic 
		/// modes (such as the bitmap, brush, palette, _font, pen, region, drawing 
		/// mode, and mapping mode) to a context stack.
		/// </summary>
		/// <param name="hdc">handle to DC</param>
		/// <returns>
		/// If the function succeeds, the return value identifies the saved state. 
		/// If the function fails, the return value is zero.
		/// </returns>
		[DllImport("gdi32.dll", SetLastError = true)]
		[SuppressUnmanagedCodeSecurity]
		protected static extern int SaveDC(IntPtr hdc);

		/// <summary>
		/// The RestoreDC function restores a device context (DC) to the specified
		/// state. The DC is restored by popping state information off a stack
		/// created by earlier calls to the SaveDC function. 
		/// </summary>
		/// <param name="hdc">handle to DC</param>
		/// <param name="nSavedDC">restore state</param>
		/// <returns>
		/// If the function succeeds, the return value is nonzero.
		/// If the function fails, the return value is zero.
		/// </returns>
		[DllImport("gdi32.dll", SetLastError = true)]
		[SuppressUnmanagedCodeSecurity]
		protected static extern bool RestoreDC(IntPtr hdc, int nSavedDC);

		[DllImport("user32.dll")]
		[SuppressUnmanagedCodeSecurity]
		protected static extern IntPtr GetDesktopWindow();

		[System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Design", "CA1060:MovePInvokesToNativeMethodsClass"), DllImport("user32.dll")]
		[SuppressUnmanagedCodeSecurity]
		protected static extern IntPtr GetDC(IntPtr hWnd);

		[DllImport("user32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		protected static extern bool ReleaseDC(IntPtr hWnd, IntPtr hDC);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		protected static extern bool DeleteObject(IntPtr hObject);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		protected static extern IntPtr SelectObject(IntPtr hdc, IntPtr hgdiobj);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		protected static extern bool TextOut(IntPtr hdc, int xStart, int yStart,
			string text, int textLen);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		protected static extern bool ExtTextOut(IntPtr hdc, int xStart, int yStart,
			uint options, ref RECT lpRect, string text, int textLen, int[] dx);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		protected static extern bool TextOutW(IntPtr hdc, int xStart, int yStart,
			char[] text, int textLen);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		protected static extern bool TextOutA(IntPtr hdc, int xStart, int yStart,
			byte[] text, int textLen);

		[DllImport("user32.dll", CharSet = CharSet.Unicode)]
		[SuppressUnmanagedCodeSecurity]
		protected static extern int DrawText(IntPtr hDC, string lpString, int nCount,
			ref RECT lpRect, uint uFormat);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		protected static extern uint SetBkColor(IntPtr hdc, int crColor);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		protected static extern uint SetTextColor(IntPtr hdc, int crColor);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		protected static extern bool GetTextExtentPoint32(IntPtr hdc, string text,
			int len, out SIZE lpSize);

		//[DllImport("gdi32.dll")]
		//[SuppressUnmanagedCodeSecurity]
		//[return: MarshalAs(UnmanagedType.Bool)]
		//protected unsafe static extern bool GetTextExtentPoint32(IntPtr hdc, 
		//  char* text, int len, out SIZE lpSize);

		[DllImport("gdi32.dll", CharSet = CharSet.Auto)]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		protected static extern bool GetTextExtentPoint32(IntPtr hdc, ref char ch,
			int len, out SIZE lpSize);

		[StructLayout(LayoutKind.Sequential)]
		public struct RECT
		{
			public int left;
			public int top;
			public int right;
			public int bottom;


			public RECT(int left, int top, int right, int bottom)
			{
				this.left = left;
				this.top = top;
				this.right = right;
				this.bottom = bottom;
			}

			// Handy methods for converting to a System.Drawing.Rectangle
			public static RECT FromRectangle(Rectangle r)
			{
				return new RECT(r.Left, r.Top, r.Right, r.Bottom);
			}

			public static implicit operator Rectangle(RECT r)
			{
				return Rectangle.FromLTRB(r.left, r.top, r.right, r.bottom);
			}

			public static implicit operator RECT(Rectangle r)
			{
				return RECT.FromRectangle(r);
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		protected struct SIZE
		{
			public int cx;
			public int cy;

			public SIZE(int cx, int cy)
			{
				this.cx = cx;
				this.cy = cy;
			}

			// Handy methods for converting to a System.Drawing.Size
			public static SIZE FromSize(Size sz)
			{
				return new SIZE(sz.Width, sz.Height);
			}

			public static implicit operator Size(SIZE sz)
			{
				return new Size(sz.cx, sz.cy);
			}

			public static implicit operator SIZE(Size sz)
			{
				return SIZE.FromSize(sz);
			}
		}

		#endregion

		#region IDisposable Members

		public virtual void Dispose()
		{
			if (_savedDc != 0)
				RestoreDC(_hdc, _savedDc);

			foreach (IntPtr hFont in _fontHandlerMap.Values)
				DeleteObject(hFont);

			if (_doubleBuffering != null)
				_doubleBuffering.Dispose();

			if (_dc != null)
				_dc.ReleaseHdc();
		}

		#endregion
	}
}