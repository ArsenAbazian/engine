using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	public static partial class Utils
	{
		#region Константы

		public const string AppName = "Rsdn.Editor";
		public const char TabVisualizer = '»';
		public const char SpaceVisualizer = '·';
		public const string EolVisualizer = "¶";

		public static readonly Size NullSize = new Size();

		public const TextFormatFlags FormatFlags =
			TextFormatFlags.NoPrefix
			| TextFormatFlags.NoFullWidthCharacterBreak // NoOverhangPadding
			//| TextFormatFlags.PreserveGraphicsClipping
			;

		#endregion

		#region ExpandTabs

		internal static string ExpandTabs(string source, int viewPosition,
			int tabSize, bool showInvisibles, bool isEndLine)
		{
			int vp = viewPosition;
			return ExpandTabs(source, viewPosition, tabSize, showInvisibles, isEndLine);
		}

		internal static string ExpandTabs(string source, ref int viewPosition,
			int tabSize, bool showInvisibles, bool isEndLine)
		{
			if (source == null)
				throw new ArgumentNullException("source");

			if (tabSize <= 0 && tabSize > 20)
				throw new ArgumentOutOfRangeException("iSpacePerTab");

			StringBuilder sb = new StringBuilder();

			string spaces = showInvisibles
				? TabVisualizer + new string(' ', tabSize - 1)
				: new string(' ', tabSize);

			char spaceChar = showInvisibles ? SpaceVisualizer : ' ';

			char[] chars = source.ToCharArray();
			for (int i = 0; i < chars.Length; i++)
			{
				char ch = chars[i];

				if (ch == '\t')
				{
					int countSpaces = viewPosition / tabSize * tabSize;
					countSpaces += tabSize - viewPosition;
					sb.Append(spaces, 0, countSpaces);
					viewPosition += countSpaces;
				}
				else
				{
					viewPosition++;
					sb.Append(ch == ' ' ? spaceChar : ch);
				}
			}

			if (showInvisibles && isEndLine)
				sb.Append('¶');

			return sb.ToString();
		}

		[System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Design", "CA1031:DoNotCatchGeneralExceptionTypes")]
		public static string DispChar(char[] chars, int i)
		{
			if (chars == null)
				return "<null>";

			string ret = "{i=" + i + "}";

			for (int k = i - 4; k < Math.Min(i + 10, chars.Length); k++)
			{
				try
				{
					if (k == i)
						ret += "¦" + chars[k] + "¦";
					else
						ret += chars[k];
				}
				catch { }
			}

			return ret;
		}

		#endregion

		#region ToStyledRange

		/// <summary>
		/// Позиция стиля в строке. Используется для преобразования
		/// массива AppliedStyle в массив StyledRange.
		/// </summary>
		private struct StylePosition
		{
			/// <summary>Позиция в тексте.</summary>
			public int PosInText;
			/// <summary>Индекс в массиве AppliedStyle-ей.</summary>
			public int IndexInAppliedStyles;
			/// <summary>
			/// Если true - это описание начальной позиции.
			/// Если false, то конечной.
			/// </summary>
			public bool Start;

			/// <summary>
			/// Стиль оссоциированный с AppliedStyle на котурый 
			/// указывает индекс IndexInAppliedStyles.
			/// Используется длы отладки.
			/// </summary>
			public Style Style;

			public override string ToString()
			{
				return "Start: " + Start + "; PosInText " + PosInText
					+ "; IndexInAs " + IndexInAppliedStyles + "; "
					+ "Style: " + Style + "; "
					;
			}
		}

		// Чтобы облегчить сборщику мусора рабтоу временные массивы
		// используются повторно. Чтобы это не вызвало проблем с 
		// потокобезопасностью они помечаются как локальные для потока.
		// Атрибут ThreadStatic говорит CLR, что нужно создать по отдельной
		// копии переменной для каждого потока.

		/// <summary>
		/// Кэш для временного динамического массива стиливых диапазонов.
		/// Используется в функции ToStyledRange.
		/// </summary>
		[ThreadStatic]
		private static List<StyledRange> _ranges = new List<StyledRange>(16);
		/// <summary>
		/// Кэш для веременного динамического массива индексов стилей.
		/// Используется в функции ToStyledRange.
		/// </summary>
		[ThreadStatic]
		private static List<int> _indices = new List<int>(16);

		/// <summary>
		/// Преобразует массив стилей применяемых к сроке (AppliedStyle[])
		/// в массив стилевых диапазонов (StyledRange[]). В отличии от
		/// AppliedStyle[] в StyledRange[] стили не могут перекрываться.
		/// Если в AppliedStyle[], то в StyledRange[] генерируется стилевой
		/// диапазон содержащий комбинацию стилей.
		/// Более модробно алгоритм этого метода описан в статье "Rsdn.Editor".
		/// </summary>
		/// <param name="styles">
		/// Список стилей применяемых к сроке. Стили в нем могут 
		/// перекрываться.
		/// </param>
		/// <param name="styleDefault">
		/// Стиль применяемый если никаких стилей не заданно. Так же 
		/// используется для задания значений которые не указаны в стиле.
		/// В любой комбинации стилей присутствует и этот стиль.
		/// </param>
		/// <param name="text">
		/// Текст строки к которой принадлежит список стилей.
		/// </param>
		/// <param name="populateStrings">
		/// Говорит о том нужно ли заполнять поле Text в структурах StyledRange.
		/// </param>
		/// <returns>
		/// Список стилевых дипазонов соотвествющий AppliedStyle[].
		/// </returns>
		internal static StyledRange[] ToStyledRange(AppliedStyle[] styles,
			CompleteStyle styleDefault, string text, bool populateStrings)
		{
			int textLen = text.Length;

			// Если это пуская строка в которой присуствует один ситкль,
			// то это особый случай.
			if (textLen == 0 && styles.Length == 1 && styles[0].Length == 0)
			{
				Debug.Assert(styles[0].Pos == 0);

				return new StyledRange[]
				{
					new StyledRange(
						CombinedStyle.Combine(styles[0].Style, styleDefault), 
						text, 0, 0)
				};
			}

			if (styles.Length == 0)
				return new StyledRange[] { new StyledRange(styleDefault, text, 0, textLen - 1) };

			// Преобразуем список стилей в отсортированный список позиций стилей
			// в строке. Для каждого стиля в таком списке добавляется позиция
			// начала его применения в тексте и позиция конца применения.
			StylePosition[] stylePositions = ToStylePosition(styles, textLen);

			// Теперь нужно пройтись по отсортированному списку позиций и
			// сформировать список последовательно идущийх облостей стилей.

			// Список диапазонов стилей. Его формирование и есть цель
			// данной функции.
			List<StyledRange> ranges = _ranges;
			ranges.Clear();
			// Индексы стилей входящих в текущую комбинацию стилей.
			// Когда встречается начальная позиция стиля индекст этого стиля 
			// добавляется в indices. Когда конечная - удаляется.
			// При каждом изменеинии списка стилей производится создание
			// комбинированного стиля (состоящего из комбинаций стилей входящих
			// в данный момент в indices) и который помещается в ranges.
			List<int> indices = _indices;
			indices.Clear();

			// Если первый стиль начинается не в самом начале строки...
			if (stylePositions[0].PosInText != 0)
				// добавляем специальный диапазон-заглушку содержащий
				// стиль используемый по умолчанию.
				ranges.Add(new StyledRange(styleDefault, 0));

			// Индекс последний позиции (точки).
			int endIndex = stylePositions.Length - 1;

			// Перебираем все позиции...
			for (int i = 0; i < stylePositions.Length; i++)
			{
				StylePosition stylePos = stylePositions[i];

				// Если эта позиция открывает новый стилевой диапазон...
				if (stylePos.Start)
					// ...запоминаем индекс применяемого стиля.
					indices.Add(stylePos.IndexInAppliedStyles);
				else
					// Удаляем индекс стиля из списка индексов активных стилей.
					indices.Remove(stylePos.IndexInAppliedStyles);

				// Если последняя позиция или, если позиция не совпадает со
				// следующей позицией и позиция не выходит за пределы текста.
				if ((i == endIndex
							|| stylePos.PosInText != stylePositions[i + 1].PosInText)
						&& stylePos.PosInText < textLen)
				{
					// Формируем комбинированный стиль из стилей накладываемых
					// на добавляемый диапазон.

					Style style = styleDefault;

					for (int j = 0; j < indices.Count; j++)
						style = CombinedStyle.Combine(styles[indices[j]].Style, style);

					// Добавляем новый стилевой диапазон.
					ranges.Add(new StyledRange(style, stylePos.PosInText));
				}
			}

			StyledRange[] styledRanges = ranges.ToArray();

			if (styledRanges.Length > 0)
			{
				// Заполняем конечную позицию диапазонов. Она равна началной
				// позиции следующего диапазона минус еденица.
				for (int i = 1; i < styledRanges.Length; i++)
					styledRanges[i - 1].EndPos = styledRanges[i].StartPos - 1;
				// Последняя позиция должна быть равна длинне текста.
				styledRanges[styledRanges.Length - 1].EndPos = textLen - 1;
			}

			if (populateStrings)
				// Инициализируем поле Text в каждом StyledRange.
				StyledRangeInitText(text, styledRanges);

			return styledRanges;
		}

		/// <summary>
		/// Инициализирует поле Text в StyledRange.
		/// </summary>
		/// <param name="text">Текст строки.</param>
		/// <param name="styledRanges">Список стилевых диапазовнов.</param>
		private static void StyledRangeInitText(string text,
			StyledRange[] styledRanges)
		{
			//TODO: Надо удалить поле Text из StyledRange и убрать код идущий ниже.

			int textLen = text.Length;

			if (styledRanges.Length == 1)
				styledRanges[0].Text = text;
			else
			{
				for (int i = 0; i < styledRanges.Length; i++)
				{
					int start = styledRanges[i].StartPos;
					int next = i + 1;

					if (next < styledRanges.Length)
						styledRanges[i].Text = text.Substring(
							start, styledRanges[next].StartPos - start);
					else
						styledRanges[i].Text = text.Substring(
							start, textLen - start);
				}
			}
		}

		/// <summary>
		/// Преобразует список стилей (AppliedStyle[]) в отсортированный список 
		/// позиций стилей в строке (StylePosition[]). Для каждого стиля в таком 
		/// списке добавляется позиция начала его применения в тексте и позиция 
		/// конца применения.
		/// </summary>
		/// <param name="appliedStyles">
		/// Список стилей которые нужно преобразовать в список позиций 
		/// начала и конца стиля в строке.
		/// </param>
		/// <param name="textLen">Длина строки.</param>
		/// <returns></returns>
		private static StylePosition[] ToStylePosition(
			AppliedStyle[] appliedStyles, int textLen)
		{
			// В этом массиве будут находится ссылки на стили отсортированые
			// по позрастанию позиций в тексете. В этот список войдут как
			// начальные позиции, так и коенченые (начальная плюс длинна).
			// По этому массив должен быть вдвое больше чем appliedStyles.
			StylePosition[] stylePositions = 
				new StylePosition[appliedStyles.Length << 1];

			// Формируем массив позиций стилей в строке.
			for (int i = 0; i < appliedStyles.Length; i++)
			{
				// Формируем позицию начала стиля.
				AppliedStyle appliedStyle = appliedStyles[i];
				StylePosition ndx;
				ndx.PosInText = appliedStyle.Pos;
				ndx.IndexInAppliedStyles = i;
				ndx.Start = true;
				ndx.Style = appliedStyle.Style;
				stylePositions[i] = ndx;
				// Формируем позицию конца стиля.
				ndx.PosInText = appliedStyle.Pos + appliedStyle.Length;
				ndx.IndexInAppliedStyles = i;
				ndx.Start = false;
				ndx.Style = appliedStyle.Style;
				// Вставляем конец стиля во вторую половину массива.
				stylePositions[i + appliedStyles.Length] = ndx;
			}

			return Utils.CountingSort(stylePositions, textLen, 0);
		}

		#endregion

		#region Манипуляции массивами

		public static T[] Insert<T>(T[] array, int index, T value)
		{
			int oldLen = array == null ? 0 : array.Length;

			if (index > oldLen)
				throw new ArgumentException("The index >= array.Length.", "index");

			int newLen = oldLen + 1;

			T[] newArray = new T[newLen];

			if (index != 0)
				Array.Copy(array, 0, newArray, 0, index);

			if (oldLen > 0)
				Array.Copy(array, index, newArray, index + 1, oldLen - index);

			newArray[index] = value;

			return newArray;
		}

		internal static T[] Remove<T>(T[] array, int index)
		{
			if (array == null)
				throw new ArgumentNullException("array");

			int len = array.Length;

			if (len == 0)
				throw new ArgumentException("array.Length == 0.", "array");

			if (index >= len)
				throw new ArgumentException("The index >= array.Length.", "index");

			int newLen = len - 1;

			T[] newArray = new T[newLen];

			if (index != 0)
				Array.Copy(array, 0, newArray, 0, index);

			if (index != newLen)
				Array.Copy(array, index + 1, newArray, index, len - index - 1);

			return newArray;
		}

		#endregion

		#region Swap

		public static void Swap<T>(ref T value1, ref T value2)
		{
			T temp = value1;
			value1 = value2;
			value2 = temp;
		}

		#endregion

		#region IntersectStyle

		public static AppliedStyle[] IntersectStyle(
			AppliedStyle[] styles, AppliedStyle style)
		{
			List<AppliedStyle> retList = new List<AppliedStyle>(styles.Length);

			retList.Add(style);

			// Индекс первого символа виртуальной строки в реальной.
			int insStart = style.Pos;
			// Индекс последнего символа виртуальной строки в реальной.
			int insEnd = style.EndPos;

			for (int i = 0; i < styles.Length; i++)
			{
				AppliedStyle posStyle = styles[i];
				// Индекс окончания стиля в реальной строке.
				int curStyleEnd = posStyle.Pos + posStyle.Length - 1;

				// Вычисляем отрезок (пересечение отрезка стиля и отрезка виртуальной
				// строки. Все значения считаются относительно реальной строки.
				int start = Math.Max(insStart, posStyle.Pos);
				int end = Math.Min(insEnd, curStyleEnd);

				// Если стиль пересекся с диапазоном входящим в виртуальную строку...
				int posStyleLength = end - start + 1;

				// Если стиль пересекся с вставляемым...
				if (posStyleLength > 0)
				{
					// Еесли стиль начался раньше вставляемого.
					if (posStyle.Pos < start)
					{
						AppliedStyle temp = posStyle;
						temp.Length = start - posStyle.Pos;
						retList.Add(temp);
					}

					// Если стиль заканчивается позже вставляемого.
					if (curStyleEnd > insEnd)
					{
						AppliedStyle temp = posStyle;
						temp.Pos = insEnd + 1;
						temp.Length = curStyleEnd - insEnd;
						retList.Add(temp);
					}
				}
				else // Стиль не пересекся с вставляемым.
					retList.Add(posStyle);
			}

			// небольшая оптимизация на случай если стилей нет.
			if (retList.Count == 0)
				return Row._defaultPosStyleArray;

			return retList.ToArray();
		}

		#endregion

		#region GetIPAL

		internal static IPAL GetIPAL(Control control)
		{
			IPAL pal = new WindowsPAL();
			pal.Initialization(control);
			return pal;
		}

		#endregion

		#region Распознование символов

		public static bool IsLetter(char ch)
		{
			//return ch == '_'
			//	|| ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z'
			//	|| ch >= 'А' && ch <= 'Я' || ch >= 'а' && ch <= 'я';
			return char.IsLetter(ch);
		}

		public static bool IsLetterOrDigit(char ch)
		{
			//return IsLetter(ch) || ch >= '0' && ch <= '9' || ch == '_';// char.IsLetterOrDigit(ch);
			return char.IsLetterOrDigit(ch) || ch == '_';
		}

		public static bool IsWhiteSpace(char ch)
		{
			return ch == ' ' || ch <= '\t';//char.IsWhiteSpace(ch);
		}

		public static bool IsPrintable(char ch)
		{
			// Редактор умеет обрабатывать только два управляющих символа: TAB и CR
			return '\t' == ch || '\r' == ch || !char.IsControl(ch);
		}

		[SuppressMessage("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
		public static bool IsPunctuation(char ch)
		{
			switch (ch)
			{
				case '¦':
				case '!':
				case '(':
				case ')':
				case '*':
				case '+':
				case ',':
				case '-':
				case '.':
				case '/':
				case ':':
				case ';':
				case '<':
				case '=':
				case '>':
				case '?':
				case '[':
				case '\\':
				case ']':
				case '^':
				case '{':
				case '|':
				case '}':
				case '~':
				case '­':
					return true;
			}

			return false;
		}

		#endregion

		#region Работа с цветом.

		public static void IncremantKnownColor(ref KnownColor knownColor)
		{
			knownColor = (KnownColor)((int)knownColor + 1);
		}

		#endregion

		#region Поиск и сортировка.

		public static int BinarySearch(List<int> array, int lo, int hi, int value)
		{
			while (lo <= hi)
			{
				int i = (lo + hi) >> 1;
				int cmpResult = array[i] - value;

				if (cmpResult == 0)
					return i;
				else if (cmpResult < 0)
					lo = i + 1;
				else
					hi = i - 1;
			}

			return ~lo;
		}

		static StylePosition[] CountingSort(StylePosition[] array, int max, int min)
		{
			switch (array.Length)
			{
				case 0:
				case 1:
					return array;
				case 2:
					if (array[0].PosInText > array[1].PosInText)
					{
						StylePosition temp = array[0];
						array[0] = array[1];
						array[1] = temp;
					}
					return array;
			}

			int i;
			int range = max - min + 2;
			int[] count = new int[range];
			StylePosition[] scratch = new StylePosition[array.Length];

			/* Set the value of count[i] to the number of
			* elements in array with value i+selectionTop-1. */
			for (i = 0; i < array.Length; i++)
			{
				int c = array[i].PosInText + 1 - min;
				count[c]++;
			}

			/* Update count[i] to be the number of
			* elements with value less than i+selectionTop. */
			for (i = 1; i < count.Length; i++)
				count[i] += count[i - 1];

			/* Copy the elements of array into scratch in
			* sorted order. */
			for (i = 0; i < array.Length; i++)
			{
				int c = array[i].PosInText - min;
				int s = count[c];
				scratch[s] = array[i];
				/* Increment count so that the next element
				* with the same value as the current element
				* is placed into its own position in scratch. */
				count[c]++;
			}

			return scratch;
		}

		#endregion

		public static string GetModulePath(Type type)
		{
			return Path.GetDirectoryName(new Uri(type.Assembly.CodeBase).LocalPath);
		}

		public static string GetModulePath()
		{
			return GetModulePath(typeof(Utils));
		}

		public static bool Between(Position<IView> start, Position<IView> end,
			Position<IView> valueForTest)
		{
			if (start > end)
				return valueForTest >= end && valueForTest <= start;
			else
				return valueForTest >= start && valueForTest <= end;
		}

		public static bool Between(Position<Document> start, Position<Document> end,
			Position<Document> valueForTest)
		{
			if (start > end)
				return valueForTest >= end && valueForTest <= start;
			else
				return valueForTest >= start && valueForTest <= end;
		}

		public static bool Contain(string[] array, string str)
		{
			foreach (string value in array)
				if (value == str)
					return true;

			return false;
		}
	}
}
