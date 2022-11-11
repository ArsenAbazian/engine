using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Rsdn.Editor
{
	class CaretHideHelper : IDisposable
	{
		public CaretHideHelper(View view)
		{
			_view = view;
			IPAL pal = _view.Pal;
			pal.HideCaret();
			pal.DestroyCaret();
		}

		View _view;

		#region IDisposable Members

		public void Dispose()
		{
			_view.RecalcCarrenCaretPosition();
		}

		#endregion
	}
}
