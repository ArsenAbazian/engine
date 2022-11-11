using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Rsdn.Editor
{
	class FreezeRepaintHelper : IDisposable
	{
		public FreezeRepaintHelper(IFreezeRepaint control)
		{
			control.FreezeRepaint();
			_control = control;
		}

		private IFreezeRepaint _control;

		#region IDisposable Members

		public void Dispose()
		{
			_control.UnfreezeRepaint();
		}

		#endregion
	}
}
