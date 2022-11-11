using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.ComponentModel;
using System.Drawing;

namespace Rsdn.Editor
{
	public class CompleteFont : PartialFont, IFont
	{
		public static CompleteFont CreateFont(string name, FontStyle style,
			float size, GraphicsUnit unit)
		{
			CompleteFont newFont = new CompleteFont(name, style, size, unit);
			CompleteFont existsFont = TryGetExists(newFont);

			if (existsFont != null)
				return existsFont;

			lock (_fonts)
			{
				// Так как между попыткой получить существующий шрифт и получением
				// блокировки список фрифтов может изменитья, нужно попытаться получить
				// шрифт повторно...
				if ((existsFont = TryGetExists(newFont)) != null)
					return existsFont;

				// Если такого шрифта нет, добавляем его в список...
				_fonts.Add(newFont, newFont);
				return newFont; // и возвращаем...
			}
		}

		/// <summary>
		/// Производит попытку найти аналогичный шрифт добавленный в список.
		/// </summary>
		/// <param name="newFont">Образец для поиска.</param>
		private static CompleteFont TryGetExists(CompleteFont newFont)
		{
			PartialFont font;

			if (_fonts.TryGetValue(newFont, out font))
			{
				newFont = font as CompleteFont;

				if (newFont == null)
					throw new Exception(
						"CompleteFont-ы должены создаваться самыми первыми!");

				return newFont;
			}

			return null;
		}

		public static CompleteFont CreateFont(string name, float size)
		{
			return CreateFont(name, FontStyle.Regular, size, GraphicsUnit.Point);
		}

		private CompleteFont(
			string name,
			FontStyle style,
			float size,
			GraphicsUnit unit)
			: base(FontExField.All, name, style, size, unit)
		{
		}

		/// <summary>
		/// Gets the em-size of this System.Drawing.Font measured in the units 
		/// specified by the System.Drawing.Font.Unit property.
		/// Returns: The em-size of this System.Drawing.Font.
		/// </summary>
		public new float Size { [DebuggerStepThrough]get { return _size; } }

		/// <summary>
		/// Gets the unit of measure for this System.Drawing.Font.
		/// Returns: A System.Drawing.GraphicsUnit that represents the unit of 
		/// measure for this System.Drawing.Font.
		/// </summary>
		[TypeConverter(typeof(FontConverter.FontUnitConverter))]
		public new GraphicsUnit Unit { [DebuggerStepThrough] get { return _unit; } }

		/// <summary>
		/// Gets style information for this System.Drawing.Font.
		/// Returns: A System.Drawing.FontStyle enumeration that contains style 
		/// information for this System.Drawing.Font.
		/// </summary>
		[Browsable(false)]
		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		public new FontStyle Style { [DebuggerStepThrough] get { return _style; } }

		/// <summary>
		/// Gets a value that indicates whether this System.Drawing.Font is bold.
		/// Returns true if this System.Drawing.Font is bold; otherwise, false.
		/// </summary>
		[DesignerSerializationVisibility(0)]
		public new bool Bold
		{
			[DebuggerStepThrough]
			get { return (_style & FontStyle.Bold) != 0; }
		}

		/// <summary>
		/// Gets a value that indicates whether this System.Drawing.Font is italic.
		/// Returns: true if this System.Drawing.Font is italic; otherwise, false.
		/// </summary>
		[DesignerSerializationVisibility(0)]
		public new bool Italic
		{
			[DebuggerStepThrough]
			get { return (_style & FontStyle.Italic) != 0; }
		}

		/// <summary>
		/// Gets a value that indicates whether this System.Drawing.Font specifies 
		/// a horizontal line through the font.
		/// Returns: true if this System.Drawing.Font has a horizontal line through
		/// it; otherwise, false.
		/// </summary>
		[DesignerSerializationVisibility(0)]
		public new bool Strikeout
		{
			[DebuggerStepThrough]
			get { return (_style & FontStyle.Strikeout) != 0; }
		}

		/// <summary>
		/// Gets a value that indicates whether this System.Drawing.Font is 
		/// underlined.
		/// Returns: true if this System.Drawing.Font is underlined; otherwise, 
		/// false.
		/// </summary>
		[DesignerSerializationVisibility(0)]
		public new bool Underline
		{
			[DebuggerStepThrough]
			get { return (_style & FontStyle.Underline) != 0; }
		}
	}
}
