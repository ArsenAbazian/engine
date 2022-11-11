using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Rsdn.Editor
{
	[Flags]
	public enum FontExField
	{
		Bold      = FontStyle.Bold,
		Italic    = FontStyle.Italic,
		Underline = FontStyle.Underline,
		/// <summary>Text with a line through the middle.</summary>
		Strikeout = FontStyle.Strikeout,
		Name      = 0x10,
		Size      = 0x20,
		All       = Bold | Italic | Underline | Strikeout | Name | Size
	}
}
