using System;
namespace Rsdn.Editor
{
	interface IFont
	{
		string Name { get; }
		bool Bold { get; }
		bool Italic { get; }
		float Size { get; }
		bool Strikeout { get; }
		System.Drawing.FontStyle Style { get; }
		bool Underline { get; }
		System.Drawing.GraphicsUnit Unit { get; }
	}
}
