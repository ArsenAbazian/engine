using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Rsdn.Editor.Keyboard
{
	public class ShortcutInfo
	{
		public ShortcutInfo()
		{
		}

		Keys _shortcut;

		public Keys Shortcut
		{
			get { return _shortcut; }
			set { _shortcut = value; }
		}

		ViewCommandInfo _viewCommandInfo;

		public ViewCommandInfo ViewCommandInfo
		{
			get { return _viewCommandInfo; }
			set { _viewCommandInfo = value; }
		}
	}
}
