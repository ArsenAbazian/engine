using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

using Rsdn.Editor.Formatter;

namespace Rsdn.Editor
{
	public class SimpleStyle : Style
	{
		#region Констркуторы

		public SimpleStyle(string name, SimpleStyle template, Color foreColor)
			: base(name, name.GetHashCode())
		{
			Init(template);
			_foreColor = foreColor;
		}

		public SimpleStyle(string name, SimpleStyle template, 
			Color foreColor, Color backColor)
			: this(name, template, foreColor)
		{
			Init(template);
			_backColor = backColor;
		}

		//public SimpleStyle(string name, SimpleStyle template, FontEx font,
		//  Color foreColor, Color backColor)
		//  : this(name)
		//{
		//  Init(template);
		//  _backColor = backColor;
		//}

		public SimpleStyle(string name, SimpleStyle template, PartialFont font,
			Color foreColor, Color backColor, Color markColor)
			: base(name, name.GetHashCode())
		{
			Init(template);
			_font = font;
			_foreColor = foreColor;
			_backColor = backColor;
			_markColor = markColor;
		}

		protected void Init(SimpleStyle template)
		{
			_displayImageHandler = template.DisplayImageHandler;
			_backColor           = template._backColor;
			_font                = template._font;
			_foreColor           = template._foreColor;
			_inactiveBackColor   = template._inactiveBackColor;
			_inactiveForeColor   = template._inactiveForeColor;
			_markColor           = template._markColor;
			_styleType           = template._styleType;
		}

		public SimpleStyle(string name) : base(name, name.GetHashCode()) { }

		public SimpleStyle(string name, PartialFont font)
			: this(name)
		{
			_font = font;
		}

		public SimpleStyle(string name, PartialFont font, Color foreColor)
			: this(name, font)
		{
			_foreColor = foreColor;
		}

		public SimpleStyle(string name, PartialFont font, Color foreColor, Color backColor)
			: this(name, font, foreColor)
		{
			_backColor = backColor;
		}

		public SimpleStyle(string name, PartialFont font, Color foreColor, Color backColor, Color markColor)
			: this(name, font, foreColor, backColor)
		{
			_markColor = markColor;
		}

		//----------------------------------------------------------

		public SimpleStyle(string name, Color foreColor)
			: this(name)
		{
			_foreColor = foreColor;
		}

		public SimpleStyle(string name, Color foreColor, Color backColor)
			: this(name, foreColor)
		{
			_backColor = backColor;
		}

		public SimpleStyle(string name, Color foreColor, Color backColor, Color markColor)
			: this(name, foreColor, backColor)
		{
			_markColor = markColor;
		}

		//----------------------------------------------------------

		public SimpleStyle(string name, PartialFont font, KnownColor foreColor)
			: this(name, font)
		{
			_foreColor = Color.FromKnownColor(foreColor);
		}

		public SimpleStyle(string name, PartialFont font, KnownColor foreColor,
			KnownColor backColor)
			: this(name, font, foreColor)
		{
			_backColor = Color.FromKnownColor(backColor);
		}

		public SimpleStyle(string name, PartialFont font, KnownColor foreColor,
			KnownColor backColor, KnownColor markColor)
			: this(name, font, foreColor, backColor)
		{
			_markColor = Color.FromKnownColor(markColor);
		}

		public SimpleStyle(string name, KnownColor foreColor)
			: this(name)
		{
			_foreColor = Color.FromKnownColor(foreColor);
		}

		public SimpleStyle(string name, KnownColor foreColor, KnownColor backColor)
			: this(name, foreColor)
		{
			_backColor = Color.FromKnownColor(backColor);
		}

		public SimpleStyle(string name, KnownColor foreColor, KnownColor backColor,
			KnownColor markColor)
			: this(name, foreColor, backColor)
		{
			_markColor = Color.FromKnownColor(markColor);
		}

		#endregion
	}
}