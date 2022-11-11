using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Rsdn.Editor
{
	class PaintInfo
	{
		public RsdnGraphics RsdnGraphics;
		public Graphics Graphics;
		public string RangeText;
		public Style Style;
		public int ViewPosition;
		public int Width;
		public int TopOffset;
		public Point RowPos;
		public Row Row;
		public int RowViewIndex;
		public int RowMaxHeight;
		public Brush Brush;
		public Brush SelectionBrush;
		public Color SelectionBackColor;
		public Style EndStyle;
		public CompleteStyle DefaultStyle;
		public List<ImageInfo> ImageInfos = new List<ImageInfo>();
#if DEBUG
		public KnownColor CurrentLineColor;
#endif

		public Brush GetBrush(Color backColor)
		{
			return backColor == SelectionBackColor ? SelectionBrush : Brush;
		}
	}
}
