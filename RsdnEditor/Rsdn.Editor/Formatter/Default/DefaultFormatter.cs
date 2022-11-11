using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

using Rsdn.Editor.Formatter;
using Rsdn.Editor.DefaultStyler;
using Rsdn.Editor.ObjectModel;
using System.Diagnostics;

namespace Rsdn.Editor
{
	public class DefaultFormatter : IFormatter
	{
        public DefaultFormatter() {
            this._styler = CreateStyler();
        }
        #region IFormatter Members

		public RowCollection DocumentToViewRows(IView view, DocumentRowCollection documentRows)
		{
			RowCollection virtualRows = new RowCollection((int)(documentRows.Count * 1.2));
			BreakLine breakLine = new BreakLine(view);

			foreach (DocumentRow row in documentRows)
			{
				ICollection<Row> hypRows = breakLine.Do(row);

				if (hypRows.Count == 0)
					virtualRows.Add(row);
				else
					virtualRows.AddRange(hypRows);
			}

			return virtualRows;
		}

		private IStyler _styler = null;
        protected virtual IStyler CreateStyler() {
            return new RsdnStyler();
        }
        public IStyler Styler
		{
			get { return _styler ?? (_styler = CreateStyler()); }
			set { _styler = value; }
		}

		public void StartParse(DocumentRowCollection rows, int startState)
		{
			Styler.StartParse(rows, startState);
		}

		public void PrepareRows(DocumentRowCollection rows)
		{
			StartParse(rows, State.Default);

			for (int i = 0, len = rows.Count; i < len; i++)
				Styler.GetStyles(i);
		}

		/// <summary>
		/// Обрабатывает стайлером диапазон строк документа. 
		/// Возвращает количество обработанных строк. Обратите внимание
		/// на то что количество строк входящих в диапазон может не совпадать
		/// с количеством обработанных строк, так как в следствии перетекания
		/// стилей на другие строки диапазон может быть автоматически расширен.
		/// </summary>
		/// <param name="rows">
		/// Коллекция содержацая строки документа (реальные строки).
		/// </param>
		/// <param name="start">Начало обрабатываемого диапазаона.</param>
		/// <param name="count">Количество строк входящий в диапазон.</param>
		/// <returns>Количество рельно обработанных строк.</returns>
		public int PrepareRows(DocumentRowCollection rows, int start, int count)
		{
			int lenAll = rows.Count;
			Debug.Assert(start + count <= lenAll && start < lenAll);

			// Начальное значение состояния стайлера берется из первой строки.
			// Это состояние которое лыло в конце предыдущей строки.
			StartParse(rows, rows[start].StartStylerState);

			// end указывает на последнюю строку диапазана которую обязательно 
			// ужно обработать.
			int end = start + count - 1;

			// Обрабатываем строки входящие в диапазон который обязательно 
			// нужно обработать.
			int i = start;
			for (; i <= end; i++)
				Styler.GetStyles(i);

			// Может оказаться так, что перетекание стиля продолжается за пределами
			// обрабатываемого диапазаона, так что нужно обработать все строки
			// StartStylerState которых не совпадает с состоянием стайлера возникающем
			// в конце предыдущей строки.
			// В начале цикла i указывает на строку следующую за диапазоном.
			for (; i < lenAll; i++)
			{
				DocumentRow row = rows[i];

				if (row.StartStylerState == Styler.CurrentState)
					// Стиль совпал, так что строку обрабатывать не надо.
					break;

				Styler.GetStyles(i);
			}

			// Возвращаем количество реально обработанных строк.
			return i - start;
		}

		enum WordType
		{
			None,
			WhiteSpace,
			Alphanumeric,
			Punctuation,
			Eol,
		}

		private static WordType GetCharType(char ch)
		{
			if (Utils.IsWhiteSpace(ch))
				return WordType.WhiteSpace;

			if (Utils.IsLetterOrDigit(ch))
				return WordType.Alphanumeric;

			return WordType.Punctuation;
		}

		/// <summary>
		/// Находит позицию (слева) в которой сменяется тип токена.
		/// Всего типов может быть три: число/символ, пробельные символы
		/// и знаки пунктуации.
		/// </summary>
		/// <param name="rows">
		/// Коллекция строк по которой осуществляется поиск.
		/// </param>
		/// <param name="start"></param>
		/// <returns>Позиция с которой осуществляется поиск.</returns>
		public Position<Document> GetPreviousWordPosition(
			DocumentRowCollection rows, Position<Document> start)
		{
			#region Объявление и инициализация переменных.

			int charIndex;
			int rowIndex;
			string text;
			WordType startWordType;
			WordType wordType;

			if (start.Character == 0)
			{
				// Для пустой строки стартовый символ нужно передвинуть на предыдущую
				// строку и установить его тип в WordType.None.

				if (start.Line == 0)
					return start;

				startWordType = WordType.None;
				rowIndex = start.Line - 1;
				startWordType = WordType.None;
				text = rows[rowIndex].Text;
				charIndex = text.Length - 1;
			}
			else
			{
				// Для не пустой строки нужно вычислить стартовый символ и его тип.

				rowIndex = start.Line;
				charIndex = start.Character - 1;
				text = rows[rowIndex].Text;
				startWordType = GetCharType(text[charIndex--]);
			}
			
			#endregion

			// Бежим по символам текущей строки в обратном направлении...
			for (int i = charIndex; i >= 0; i--)
			{
				// ... и сравниваем их тип с типом первого символа...
				wordType = GetCharType(text[i]);
				
				if (wordType != startWordType)
				{
					// Если типы символов не совпадают, и тип стартового символа пробельных,
					// то присваеваем стартовому типу текущий. Это нужно для того чтобы
					// пропускать пробелы идущие в конце слов (так принято в редакторах).
					if (startWordType == WordType.WhiteSpace)
						startWordType = wordType;
					else
					{
						// Иначе возвращаем текущую позицию.
						return new Position<Document>(rowIndex, i + 1);
					}
				}
			}

			// Если ничего не найдено возвращаем позицию начала строки.
			return new Position<Document>(rowIndex, 0);
		}

		public Position<Document> GetNextWordPosition(
			DocumentRowCollection rows, Position<Document> start)
		{
			#region Объявление и инициализация переменных.

			int charIndex;
			int rowIndex;
			string text = rows[start.Line].Text;
			WordType startWordType;
			WordType wordType;

			if (start.Character == text.Length)
			{
				// Для пустой строки стартовый символ нужно передвинуть на предыдущую
				// строку и установить его тип в WordType.None.

				if (start.Line == rows.Count - 1)
					return start;
				else
					return new Position<Document>(start.Line + 1, 0);
			}

			// Для не пустой строки нужно вычислить стартовый символ и его тип.

			rowIndex = start.Line;
			charIndex = start.Character;
			startWordType = GetCharType(text[charIndex++]);

			#endregion

			// Бежим по символам текущей строки...
			for (int i = charIndex; i < text.Length; i++)
			{
				// ... и сравниваем их тип с типом первого символа...
				wordType = GetCharType(text[i]);

				if (wordType != startWordType)
				{
					// Если типы символов не совпадают, и тип текущего символа пробельный...
					if (wordType == WordType.WhiteSpace)
					{
						// .. то возвращаем позицию первого непробельного символа.
						for (++i; i < text.Length; i++)
							if (GetCharType(text[i]) != WordType.WhiteSpace)
								return new Position<Document>(rowIndex, i);
					}
					else
						// Иначе возвращаем текущую позицию.
						return new Position<Document>(rowIndex, i);
				}
			}

			// Если ничего не найдено возвращаем позицию начала строки.
			return new Position<Document>(rowIndex, text.Length);
		}

		public Range<Document> GetWordRange(
			DocumentRowCollection rows, Position<Document> start)
		{
			Position<Document> end = GetNextWordPosition(rows, start);

			Row row = rows[end.Line];
			string text = row.Text;

			// Пропускаем конечные пробельне символы.
			for (int i = end.Character - 1; i >= 0; i--)
			{
				if (!char.IsWhiteSpace(text[i]))
				{
					end.Character = i + 1;
					break;
				}
			}

			return new Range<Document>(GetPreviousWordPosition(rows, start), end);
		}
		#endregion
	}
}
