using System;
using System.Collections.Generic;
using System.Text;

namespace Rsdn.Editor.Keyboard
{
	public class ViewCommandAttribute : Attribute
	{
		public ViewCommandAttribute(string displayText)
		{
			_displayText = displayText;
		}

		string _displayText;

		public string DisplayText
		{
			get { return _displayText; }
		}
	}
}
