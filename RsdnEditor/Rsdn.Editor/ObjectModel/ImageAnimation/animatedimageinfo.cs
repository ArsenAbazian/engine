using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Rsdn.Editor
{
	internal struct ImageInfo
	{
		public ImageInfo(Image image, Rectangle bounds, Color backColor)
		{
			Image = image;
			Bounds = bounds;
			BackColor = backColor;
		}

		public Image Image;
		public Rectangle Bounds;
		public Color BackColor;
	}
}
