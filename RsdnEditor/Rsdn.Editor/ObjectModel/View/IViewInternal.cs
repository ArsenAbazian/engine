using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Rsdn.Editor.ObjectModel
{
	internal interface IViewInternal
	{
		IViewOwner ViewOwner { get; set; }
	}
}
