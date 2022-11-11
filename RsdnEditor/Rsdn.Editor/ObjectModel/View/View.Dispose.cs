// Dispose.

using System;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using Rsdn.Editor.Properties;

namespace Rsdn.Editor
{
	partial class View
	{
		Cursor _inverseArrawCursor;

		internal Cursor InverseArrawCursor
		{
			get { return _inverseArrawCursor; }
		}

		private void InitCursors()
		{
			using(MemoryStream stream = new MemoryStream(Resources.InversedArrow))
				_inverseArrawCursor = new Cursor(stream);
		}

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">
		/// true if managed resources should be disposed; otherwise, false.
		/// </param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}

			_inverseArrawCursor.Dispose();
			_imageAnimatorHelper.Dispose();
			_vScrollTimer.Dispose();

			base.Dispose(disposing);
		}
	}
}
