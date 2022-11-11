using System;
using System.Collections.Generic;
using System.Drawing;

using Rsdn.Interop;
using System.ComponentModel;

namespace Rsdn.Editor
{
	public class DoubleBuffering : IDisposable
	{
		public DoubleBuffering(IntPtr hdc, Rectangle rect)
		{
			if (hdc == IntPtr.Zero)
				throw new ArgumentException("The hdc can't be null.", "hdc");

			_sourceHdc = hdc;
			_paintRect = rect;
			Api.Verify(_memHdc = Api.CreateCompatibleDC(hdc));

			// Create a bitmap big enough for our client rect
			Api.Verify(_dcBitmap = Api.CreateCompatibleBitmap(
				hdc, _paintRect.Width, _paintRect.Height));

			// Select the bitmap into the off-screen dc
			_oldDcBitmap = Api.SelectObject(_memHdc, _dcBitmap);

			Api.Verify(Api.SetViewportOrgEx(
				_memHdc, -_paintRect.left, -_paintRect.top));
		}

		private IntPtr _memHdc;

		public IntPtr Hdc
		{
			get { return _memHdc; }
		}

		private IntPtr _sourceHdc;

		public IntPtr SourceHdc
		{
			get { return _sourceHdc; }
		}

		private IntPtr _dcBitmap;
		private IntPtr _oldDcBitmap;
		private Api.RECT _paintRect;

		#region IDisposable Members

		public void Dispose()
		{
			if (_memHdc != IntPtr.Zero)
			{
				Api.BitBlt(_sourceHdc,
					_paintRect.left, _paintRect.top,
					_paintRect.Width,
					_paintRect.Height,
					_memHdc,
					_paintRect.left, _paintRect.top,
					Api.Rop.SrcCopy);
				// Done with off screen bitmap and dc
				Api.SelectObject(_memHdc, _oldDcBitmap);
				Api.Verify(Api.DeleteObject(_dcBitmap));
				Api.Verify(Api.DeleteDC(_memHdc));
			}
		}

		#endregion

	}
}
