using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Diagnostics;

namespace Rsdn.Editor
{
	public class CompleteStyle : SimpleStyle
	{
		public CompleteStyle(string name, CompleteFont font)
			: this(name, font, KnownColor.WindowText, KnownColor.Window,
			KnownColor.InactiveCaption, KnownColor.InactiveCaptionText, 
			KnownColor.Coral)
		{
		}

		public CompleteStyle(string name, CompleteFont font,
			KnownColor foreColor, KnownColor backColor)
			: this(name, font, foreColor, backColor,
				backColor, foreColor,
				KnownColor.Coral)
		{
		}

		public CompleteStyle(string name, CompleteFont font,
			KnownColor foreColor, KnownColor backColor, KnownColor inactiveBackColor,
			KnownColor inactiveForeColor, KnownColor markColor)
			: this(name, font,
				Color.FromKnownColor(foreColor), 
				Color.FromKnownColor(backColor), 
				Color.FromKnownColor(inactiveBackColor), 
				Color.FromKnownColor(inactiveForeColor), 
				Color.FromKnownColor(markColor))
		{
		}

		public CompleteStyle(string name, CompleteFont font, 
			Color foreColor, Color backColor, Color inactiveBackColor, 
			Color inactiveForeColor,
			Color markColor)
			: base(name)
		{
			if (backColor == Color.Empty)
				ThrowDefaultStyleException("BackColor");
			if (foreColor == Color.Empty)
				ThrowDefaultStyleException("ForeColor");
			if (inactiveBackColor == Color.Empty)
				ThrowDefaultStyleException("InactiveBackColor");
			if (inactiveForeColor == Color.Empty)
				ThrowDefaultStyleException("InactiveForeColor");
			if (foreColor == Color.Empty)
				ThrowDefaultStyleException("ForeColor");
			if (markColor == Color.Empty)
				ThrowDefaultStyleException("MarkColor");

			_font = font;
			_foreColor = foreColor;
			_backColor = backColor;
			_markColor = markColor;
			_inactiveBackColor = inactiveBackColor;
			_inactiveForeColor = inactiveForeColor;
		}

		void ThrowDefaultStyleException(string propertyName)
		{
			throw new ArgumentNullException("value", "В стиле используемом по "
				+ "умолчанию должны быть заданы все свойства." + Environment.NewLine
				+ "Свойство " + propertyName + " не задано изи задано неверно!");
		}

		public new CompleteFont Font
		{
			[DebuggerHidden]
			get { return (CompleteFont)_font; }
		}
	}
}
