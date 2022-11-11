using System;
using System.Collections.Generic;
using System.Text;
using Rsdn.Editor.ObjectModel;
using System.Windows.Forms;

namespace Rsdn.Editor
{
	internal interface IViewOwner
	{
		CompleteStyle DefaultStyle { get; set; }
		bool ShowTextCaret { get; }
		IView ActiveView { get; }
        bool ShouldChangeLine(bool isExtendSelection, int line);
        void ProcessViewKeyPress(KeyPressEventArgs e);
	}
}
