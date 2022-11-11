using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

using CharSizeType = System.SByte;
using System.Windows.Forms;
using System.Diagnostics;

namespace Rsdn.Editor
{
	/// <summary>
	/// Этот класс позволяет ускорить и упростить рассчет размера строк и 
	/// отдельных символов.
	/// </summary>
	/// <remarks>
	/// Принцип действия этого класса прост. Класс кэшируем
	/// размеры символов диапазонами (массивами) по 128 символов.
	/// Это позволяет не тратя много памяти получить довольно серьезное 
	/// ускорение рассчета размеров символов (около 40 раз).
	/// Используйте статическую функцию GetFontHelper чтобы получить
	/// закэшированную версию хэлпера. Класс неизменяемый так что он
	/// полностью потокобезопасен.
	/// </remarks>
	/// <example>
	/// int width = GetFontHelper(Font).MeasureWidth("Мама папа я!");
	/// </example>
	public class FontHelper
	{
		static FontHelper()
		{

			Graphics graphics = Graphics.FromHwnd(
				Rsdn.Interop.Api.GetDesktopWindow());

			Trace.Assert(graphics != null);

			_sharedGraphics = new RsdnGraphics(graphics);
		}

		public FontHelper(ZoomedFont font, RsdnGraphics graphics)
		{
			if (font == null)
				throw new ArgumentNullException("_font");

			_font = font;

			_graphics = graphics;

			//Size size = TextRenderer.MeasureText("M", _font,
			//	Utils.NullSize, Utils.FormatFlags);
			Size size = _graphics.GetCharSize('M', _font);

			_height = size.Height;

			_tabWizWidth = GetCharWidth(Utils.TabVisualizer);
			_spaceWizWidth = GetCharWidth(Utils.SpaceVisualizer);
			_spaceWidth = GetCharWidth(' ');
		}

		private static RsdnGraphics _sharedGraphics;
		private RsdnGraphics _graphics;

		public FontHelper(ZoomedFont font) : this(font, _sharedGraphics) { }

		private int _height;

		public int Height
		{
			[DebuggerHidden]
			get { return _height; }
		}

		int _tabWizWidth;
		int _spaceWizWidth;
		int _spaceWidth;

		/// <summary>
		/// Рассчитывает суммарную ширину диапазаона символов в 
		/// массиве символов.
		/// </summary>
		/// <param name="chars">
		/// Массив символов в котором нужно рассчитать ширину символов.
		/// </param>
		/// <param name="start">
		/// Начало диапазана для которого нужно рассчитать ширину.
		/// </param>
		/// <param name="len">Длинна диапазаона.</param>
		/// <param name="curVirtPos">
		/// Текущая позиция с учетом раскрытия табуляций и замены невидимых символов
		/// на символы визуализации.
		/// </param>
		/// <param name="tabSize">
		/// Количество пробельных символов на которое будет заменена табуляция.
		/// </param>
		/// <param name="showInvisibles">
		/// Заменять ли невидимые символы (пробелы и табуляции) на 
		/// символы визуализации.
		/// </param>
		/// <returns></returns>
		public int MeasureWidth(char[] chars, int start, int len,
			ref int curVirtPos, int tabSize, bool showInvisibles)
		{
			int width = 0;
			int tabWizWidth;
			int spaceWizWidth;

			if (showInvisibles)
			{
				tabWizWidth = _tabWizWidth;
				spaceWizWidth = _spaceWizWidth;
			}
			else
			{
				spaceWizWidth = _spaceWidth;
				tabWizWidth = spaceWizWidth;
			}

			len += start;

			for (int i = start; i < len; i++)
			{
				char ch = chars[i];
				if (ch == '\t')
				{
					int countSpaces = curVirtPos / tabSize * tabSize;
					countSpaces += tabSize - curVirtPos;
					// - 1 - это чтобы учесть семвол визуализации
					width += tabWizWidth + (countSpaces - 1) * spaceWizWidth;
					curVirtPos += countSpaces;
				}
				else
				{
					width += ch == ' ' ? spaceWizWidth : GetCharWidth(ch);
					curVirtPos++;
				}
			}

			return width;
		}

		public int MeasureWidth(string text, int start, int len,
			ref int curVirtPos, int tabSize, bool showInvisibles)
		{
			char[] chars = new char[len];

			text.CopyTo(start, chars, 0, len);

			return MeasureWidth(chars, 0, len, ref curVirtPos, tabSize, showInvisibles);
		}

		/// <summary>
		/// Рассчитывает ширину строки.
		/// </summary>
		/// <param name="text">Строка ширину которой нужно рассчитать.</param>
		/// <returns></returns>
		public int MeasureWidth(string text)
		{
			return MeasureWidth(text, 0, text.Length);
		}

		/// <summary>
		/// Рассчитывает суммарную ширину диапазаона символов в строке.
		/// </summary>
		/// <param name="chars">
		/// Строка в котором нужно рассчитать ширину символов.
		/// </param>
		/// <param name="start">
		/// Начало диапазана для которого нужно рассчитать ширину.
		/// </param>
		/// <param name="len">Длинна диапазаона.</param>
		/// <returns></returns>
		public int MeasureWidth(string text, int start, int len)
		{
			int width = 0;

			len += start;

#if DEBUG_FontHelper
			for (int i = start; i < len; i++)
			{
				char ch = text[i];
				int chW = GetCharWidth(ch);
				Size size = TextRenderer.MeasureText("" + ch, Font,
					Utils.NullSize, Utils.FormatFlags);
				if (size.Width != chW)
				{
					chW = size.Width;
				}
				System.Diagnostics.Debug.Assert(size.Width == chW);
				width += chW;
			}
#else
			for (int i = start; i < len; i++)
				width += GetCharWidth(text[i]);
#endif

			return width;
		}

		///// <summary>
		///// Мап шрифтов на хэлпер-объекты. Кэширует готовые хэлпер-объекты.
		///// </summary>
		//private static Hashtable _fontHelpers = new Hashtable();

		/// <summary>
		/// Шрифт для которого создан хэлпер-объект.
		/// </summary>
		private ZoomedFont _font;

		/// <summary>
		/// Шрифт для которого создан хэлпер-объект.
		/// </summary>
		[DebuggerHidden]
		public ZoomedFont Font
		{
			get { return _font; }
		}

		/// <summary>
		/// Размер бакета хранящего ширины для диапазона сивоволв.
		/// </summary>
		private const int BucketSize = 128;
		/// <summary>
		/// Количество бакетов хранящих ширины для диапазана символов.
		/// </summary>
		/// <remarks>
		/// Юникодный ширфт позволяет хранить 65535 (0xFFFF, 64Кб) символов.
		/// Реально приложение используется конкретным индивидумом работающим 
		/// с одним-тремя языками одновремнно. При этом большая часть из 
		/// ахаватываемого Юникодом диапазона символов попросту не используется.
		/// Кэшировать ширины всех символов для всех фрифтов расточительно и 
		/// бессмысленно. Даже если тратить по байту на ширину каждого символа, 
		/// то на каждый фрифт будет уходить по 64Кб памяти.
		/// 
		/// </remarks>
		private const int BucketCount = char.MaxValue / BucketSize + 1;

		CharSizeType[][] _charSizeMap =
			new CharSizeType[BucketCount][];

		public int GetCharWidth(char ch)
		{
			int buckedIndex = ch / BucketSize;

			CharSizeType[] bucked = _charSizeMap[buckedIndex];

			if (bucked == null)
				bucked = InitBucked(buckedIndex);

			int charIndex = ch % BucketSize;

			return bucked[charIndex];
		}

		private CharSizeType[] InitBucked(int buckedIndex)
		{
			CharSizeType[] bucked = _charSizeMap[buckedIndex] = new sbyte[BucketSize];
			for (int i = 0; i < BucketSize; i++)
			{
				//Size size = TextRenderer.MeasureText(
				//	new string((char)(buckedIndex * BucketSize + i), 1), // символ
				//	Font, Utils.NullSize, Utils.FormatFlags);

				Size size = _graphics.GetCharSize(
	(char)(buckedIndex * BucketSize + i), Font);

				bucked[i] = (CharSizeType)size.Width;
			}

			return bucked;
		}
	}
}
