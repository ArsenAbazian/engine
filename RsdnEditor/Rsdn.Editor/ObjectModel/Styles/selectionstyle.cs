using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Rsdn.Editor
{
	class SelectionStyle : SimpleStyle
	{
		#region Констркуторы

		public SelectionStyle()
			: base("selection", KnownColor.HighlightText, KnownColor.Highlight)
		{
			_styleType = StyleType.Selection;
			_inactiveBackColor = Color.FromKnownColor(KnownColor.InactiveCaption);
			_inactiveForeColor = Color.FromKnownColor(KnownColor.InactiveCaptionText);
		}

		public SelectionStyle(Color foreColor, Color backColor)
			: base("selection", foreColor, backColor)
		{
			_styleType = StyleType.Selection;
		}

		#endregion
	}
}
