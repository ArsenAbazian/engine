using System;
using System.Collections.Generic;
using System.Text;

namespace Rsdn.Editor.Keyboard
{
	public class ViewCommandInfo
	{
		public ViewCommandInfo()
		{
			_commandName = _displayText = "";
		}

		public ViewCommandInfo(string commandName, string displayText)
		{
			_commandName = commandName;
			_displayText = displayText;
		}

		string _commandName;

		public string CommandName
		{
			get { return _commandName; }
		}

		string _displayText;

		public string DisplayText
		{
			get { return _displayText + " (" + CommandName + ")"; }
		}

		public override string ToString()
		{
			return DisplayText;
		}
	}
}
