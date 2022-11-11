using System;
using System.Collections.Generic;
using System.Drawing;

namespace Rsdn.Editor
{
	class ImagedStyle : SimpleStyle
	{
		#region Конструкторы

		public ImagedStyle(string name, DisplayImage displayImageHandler)
			: base(name)
		{
			_displayImageHandler = displayImageHandler;
		}

		public ImagedStyle(
			string name, 
			DisplayImage displayImageHandler, 
			Color backColor) : base(name)
		{
			_backColor = backColor;
			_displayImageHandler = displayImageHandler;
		}

		#endregion
	}
}
