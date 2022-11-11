using System;
using System.Collections.Generic;
using System.Text;

namespace Rsdn.Editor
{
	interface IFreezeRepaint
	{
		void FreezeRepaint();
		void UnfreezeRepaint();
	}
}
