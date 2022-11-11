using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Security;
using System.Diagnostics;
using System.ComponentModel;

namespace Rsdn.Interop
{
	internal class Api
	{
		#region Импорт Win API

		/// <summary>
		/// Creates a rectangular region.
		/// </summary>
		/// <param name="nLeftRect">x-coordinate of upper-left corner</param>
		/// <param name="nTopRect">y-coordinate of upper-left corner</param>
		/// <param name="nRightRect">x-coordinate of lower-right corner</param>
		/// <param name="nBottomRect">y-coordinate of lower-right corner</param>
		/// <returns>
		/// If the function succeeds, the return value is the handle to the region.
		/// If the function fails, the return value is IntPtr.Zero (NULL).
		/// </returns>
		[DllImport("gdi32.dll", SetLastError = true)]
		[SuppressUnmanagedCodeSecurity]
		public static extern IntPtr CreateRectRgn(
			int nLeftRect, int nTopRect, int nRightRect, int nBottomRect);


		public enum RegionType : int
		{
			Error   = 0,
			Null    = 1,
			Simple  = 2,
			Complex = 3,
		}

		/// <summary>
		/// Selects a region as the current clipping region for the specified 
		/// device context.
		/// </summary>
		/// <param name="hdc">Handle to the device context.</param>
		/// <param name="hrgn">Handle to the region to be selected.</param>
		/// <returns>
		/// The return value specifies the region's complexity and can be one of 
		/// the following values.
		/// NULLREGION    - Region is empty. 
		/// SIMPLEREGION  - Region is a single rectangle. 
		/// COMPLEXREGION - Region is more than one rectangle. 
		/// ERROR         - An error occurred. (The previous clipping region 
		///                 is unaffected.) 
		/// </returns>
		[DllImport("gdi32.dll", SetLastError = true)]
		[SuppressUnmanagedCodeSecurity]
		public static extern RegionType SelectClipRgn(IntPtr hdc, IntPtr hrgn);

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
		public static extern int SaveDC(IntPtr hdc);

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
		public static extern bool RestoreDC(IntPtr hdc, int nSavedDC);

		[DllImport("user32.dll")]
		[SuppressUnmanagedCodeSecurity]
		public static extern IntPtr GetDesktopWindow();

		[System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Design", "CA1060:MovePInvokesToNativeMethodsClass"), DllImport("user32.dll")]
		[SuppressUnmanagedCodeSecurity]
		public static extern IntPtr GetDC(IntPtr hWnd);

		[DllImport("user32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool ReleaseDC(IntPtr hWnd, IntPtr hDC);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool DeleteObject(IntPtr hObject);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="hdc">handle to DC</param>
		/// <returns>
		/// If the function succeeds, the return value is nonzero.
		/// If the function fails, the return value is zero. 
		/// </returns>
		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool DeleteDC(IntPtr hdc);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		public static extern IntPtr SelectObject(IntPtr hdc, IntPtr hgdiobj);

		[DllImport("gdi32.dll", CharSet = CharSet.Auto)]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool TextOut(IntPtr hdc, int xStart, int yStart,
			string text, int textLen);

		[DllImport("gdi32.dll", CharSet = CharSet.Auto)]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool ExtTextOut(IntPtr hdc, int xStart, int yStart,
			uint options, ref RECT lpRect, string text, int textLen, int[] dx);

		[DllImport("gdi32.dll", CharSet = CharSet.Unicode)]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool TextOutW(IntPtr hdc, int xStart, int yStart,
			char[] text, int textLen);

		[DllImport("gdi32.dll", CharSet = CharSet.Ansi)]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool TextOutA(IntPtr hdc, int xStart, int yStart,
			byte[] text, int textLen);

		[DllImport("user32.dll", CharSet = CharSet.Auto)]
		[SuppressUnmanagedCodeSecurity]
		public static extern int DrawText(IntPtr hDC, string lpString, int nCount,
			ref RECT lpRect, uint uFormat);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		public static extern uint SetBkColor(IntPtr hdc, int crColor);

		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		public static extern uint SetTextColor(IntPtr hdc, int crColor);

		[DllImport("gdi32.dll", CharSet = CharSet.Auto)]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool GetTextExtentPoint32(IntPtr hdc, string text,
			int len, out SIZE lpSize);

		[DllImport("gdi32.dll", CharSet = CharSet.Auto)]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool GetTextExtentPoint32(IntPtr hdc, ref char ch,
			int len, out SIZE lpSize);

		/// <summary>
		/// Creates a memory device context (DC) compatible with the specified 
		/// device. 
		/// </summary>
		/// <param name="hdc">handle to DC</param>
		/// <returns>handle to compatible DC</returns>
		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		public static extern IntPtr CreateCompatibleDC(IntPtr hdc);

		/// <summary>
		/// Creates a bitmap compatible with the device that is associated with 
		/// the specified device context.
		/// </summary>
		/// <param name="hdc">handle to DC</param>
		/// <param name="width">width of bitmap, in pixels</param>
		/// <param name="height">height of bitmap, in pixels</param>
		/// <returns></returns>
		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		public static extern IntPtr CreateCompatibleBitmap(IntPtr hdc,
			int width, int height);

		/// <summary>
		/// Specifies which device point maps to the window origin (0,0).
		/// </summary>
		/// <param name="hdc">handle to device context</param>
		/// <param name="X">new x-coordinate of viewport origin</param>
		/// <param name="Y">new y-coordinate of viewport origin</param>
		/// <param name="lpPoint">original viewport origin</param>
		/// <returns></returns>
		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool SetViewportOrgEx(
			IntPtr hdc, int x, int y, ref Point point);

		/// <summary>
		/// Specifies which device point maps to the window origin (0,0).
		/// </summary>
		/// <param name="hdc">handle to device context</param>
		/// <param name="X">new x-coordinate of viewport origin</param>
		/// <param name="Y">new y-coordinate of viewport origin</param>
		/// <param name="lpPoint">original viewport origin</param>
		/// <returns></returns>
		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		private static extern bool SetViewportOrgEx(
			IntPtr hdc, int x, int y, IntPtr pPoint);

		public static bool SetViewportOrgEx(IntPtr hdc, int x, int y)
		{
			return SetViewportOrgEx(hdc, x, y, IntPtr.Zero);
		}

		/// <summary>
		/// The BitBlt function performs a bit-block transfer of the color data 
		/// corresponding to a rectangle of pixels from the specified source 
		/// device context into a destination device context.
		/// </summary>
		/// <param name="hdcDest">handle to destination DC</param>
		/// <param name="xDest">x-coord of destination upper-left corner</param>
		/// <param name="yDest">y-coord of destination upper-left corner</param>
		/// <param name="width">width of destination rectangle</param>
		/// <param name="height">height of destination rectangle</param>
		/// <param name="hdcSrc">handle to source DC</param>
		/// <param name="xSrc">x-coordinate of source upper-left corner</param>
		/// <param name="ySrc">y-coordinate of source upper-left corner</param>
		/// <param name="dwRop">raster operation code</param>
		/// <returns></returns>
		[DllImport("gdi32.dll")]
		[SuppressUnmanagedCodeSecurity]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool BitBlt(IntPtr hdcDest, int xDest, int yDest,
			int width, int height, IntPtr hdcSrc, int xSrc, int ySrc, Rop rop);


		/// <summary>
		/// Ternary raster operation codes
		/// </summary>
		[Flags]
		public enum Rop : uint
		{
			/// <summary>dest = source</summary>
			SrcCopy = 0x00CC0020,
			/// <summary>dest = source OR dest</summary>
			SrcPaint = 0x00EE0086,
			/// <summary>dest = source AND dest</summary>
			SrcAND = 0x008800C6,
			/// <summary>dest = source XOR dest</summary>
			SrcInvert = 0x00660046,
			/// <summary>dest = source AND (NOT dest )</summary>
			SrcErase = 0x00440328,
			/// <summary>dest = (NOT source)</summary>
			NotSrcCopy = 0x00330008,
			/// <summary>dest = (NOT src) AND (NOT dest)</summary>
			NotSrcErase = 0x001100A6,
			/// <summary>dest = (source AND pattern)</summary>
			MergeCopy = 0x00C000CA,
			/// <summary>dest = (NOT source) OR dest</summary>
			MergePaint = 0x00BB0226,
			/// <summary>dest = pattern</summary>
			PatCopy = 0x00F00021,
			/// <summary>dest = DPSnoo</summary>
			PatPaint = 0x00FB0A09,
			/// <summary>dest = pattern XOR dest</summary>
			PatInvert = 0x005A0049,
			/// <summary>dest = (NOT dest)</summary>
			DstInvert = 0x00550009,
			/// <summary>dest = BLACK
			/// <para>Fills the destination rectangle using the color associated with 
			/// index 0 in the physical palette. (This color is black for the 
			/// default physical palette.)</para>
			/// </summary>
			Blackness = 0x00000042,
			/// <summary>dest = WHITE</summary>
			Whiteness = 0x00FF0062,
			/// <summary>Do not Mirror the bitmap in this call</summary>
			NoMirrorBitmap = 0x80000000,
			/// <summary>Include layered windows</summary>
			CaptureBlt = 0x40000000,
		}

		/* Quaternary raster codes */
		//#define MAKEROP4(fore,back) (DWORD)((((back) << 8) & 0xFF000000) | (fore))

		[StructLayout(LayoutKind.Sequential)]
		public struct RECT
		{
			public int left;
			public int top;
			public int right;
			public int bottom;

			public RECT(Rectangle r)
			{
				this.left = r.Left;
				this.top = r.Top;
				this.right = r.Right;
				this.bottom = r.Bottom;
			}

			public RECT(int left, int top, int right, int bottom)
			{
				this.left = left;
				this.top = top;
				this.right = right;
				this.bottom = bottom;
			}

			public static RECT FromXYWH(int x, int y, int width, int height)
			{
				return new RECT(x, y, x + width, y + height);
			}

			public int Width { get { return right - left; } }
			public int Height { get { return bottom - top; } }
			public Size Size { get { return new Size(right - left, bottom - top); } }

			public Rectangle ToRectangle()
			{
				return new Rectangle(left, top, Width, Height);
			}

			public static RECT FromRectangle(Rectangle r)
			{
				return new RECT(r.Left, r.Top, r.Right, r.Bottom);
			}

			public static implicit operator Rectangle(RECT rect)
			{
				return rect.ToRectangle();
			}

			public static implicit operator RECT(Rectangle rect)
			{
				return FromRectangle(rect);
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct SIZE
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

		#region Вспомогательные методы

		/// <summary>
		/// Проверяет значение параметра <paramref name="test"/> и возбуждает
		/// исключение Win32Exception если оно равно false.
		/// </summary>
		[DebuggerHidden]
		public static void Verify(bool test)
		{
			if (!test)
				throw new Win32Exception();
		}

		[DebuggerHidden]
		public static void Verify(IntPtr test)
		{
			if (test == IntPtr.Zero)
				throw new Win32Exception();
		}

		#endregion
	}
}
