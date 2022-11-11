// Преобразование координат текста (строки/символы).

using System;
using System.Drawing;

using Rsdn.Editor.Formatter;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	partial class View
	{
		#region ToDocument (Преобразует Position<IView> в Position<Document>)

		/// <summary>
		/// Преобразует Position<IView> в Position<Document>.
		/// </summary>
		/// <param name="viewPosition">Виртуальная позиция 
		/// (в строках и символах представления)</param>
		/// <returns>Позиция в документе.</returns>
		public Position<Document> ToDocument(Position<IView> viewPosition)
		{
			Row viewRow = Rows[viewPosition.Line];

			return new Position<Document>(
				Document.Rows.IndexOf(viewRow.RealDocumentRow),
				viewPosition.Character + viewRow.OffsetInDocumentRow);
		}

		/// <summary>
		/// Преобразует координаты view в Position<Document>.
		/// Если преобразование невозможно генерирует исключение.
		/// </summary>
		/// <param name="viewPosition">Координаты представления.</param>
		/// <returns>Позиция в документе.</returns>
		public Position<Document> ToDocument(Point location)
		{
			return ToDocument(ToView(location));
		}

		#endregion

		#region ToView (Преобразует Range<Document> в Position<IView>)

		/// <summary>
		/// Преобразует Position<Document> в Position<IView>.
		/// </summary>
		/// <param name="documentPosition">Позиция в документе.</param>
		/// <returns>Позиция в представлении.</returns>
		public Position<IView> ToView(Position<Document> documentPosition)
		{
			RowCollection rows = Rows;
			DocumentRow docRow = Document.Rows[documentPosition.Line];

			// Индекс строки в представлении.
			int viewIndex = rows.IndexOfDocumentRow(docRow);

			if (viewIndex < 0)
				throw new ArgumentOutOfRangeException("documentPosition",
						documentPosition, "The Line of position out of range.");

			// Позиция символа в документе.
			int docChPos = documentPosition.Character;
			Row viewRow;

			// В представлении присуствуют несколько строк соотвествующих 
			// строке документа. Ищем троку представления сорержащую позицию 
			// символа соотвествующую переданной позиции.
			for (int i = viewIndex, count = rows.Count; i < count; i++)
			{
				viewRow = rows[i];

				if (viewRow.RealDocumentRow != docRow)
				{
					int line = documentPosition.Line;

					if (line < 0 || line > Document.Rows.Count)
						throw new Exception("Incorrect Line in the documentPosition.");

					int ch = documentPosition.Character;

					if (ch < 0 || ch > Document.Rows[line].TextLength)
						throw new Exception(
							"Incorrect Character in the documentPosition.");

					throw new Exception(
						"The implementation of IFormatter. DocumentToViewRows is bad.");
				}


				// Отступ сивола строки представления в строке документа.
				int start = viewRow.OffsetInDocumentRow;

				int viewPos = docChPos - start;

				// Если нашли строку представления... 
				if (viewPos <= viewRow.TextLength)
					return new Position<IView>(i, viewPos);
			}

			throw new ArgumentOutOfRangeException("documentPosition",
				documentPosition, "The Character of position out of range.");
		}

		/// <summary>
		/// Преобразует Position<Document> в Position<IView>.
		/// </summary>
		/// <param name="range">Диапазон в документе.</param>
		/// <returns>Диапазон в представлении.</returns>
		public Range<IView> ToView(Range<Document> range)
		{
			Position<IView> start = ToView(range.Start);

			if (range.Start == range.End)
				return new Range<IView>(start, start);
			else
				return new Range<IView>(start, ToView(range.End));
		}

		#endregion

		#region ToLocation (Переводит Position<IView> в координаты View)

		/// <summary>
		/// Вычисляет X-координаты заданного символа для заданной строки.
		/// </summary>
		/// <param name="viewPosition">Позиция текста в представлении.</param>
		/// <returns>Координаты в представлении.</returns>
		public Rectangle? ToLocation(Position<IView> viewPosition)
		{
			#region Инициализация переменных.

			int line = viewPosition.Line;
			int pos = viewPosition.Character;
			int topRow = FirstVisibleRow;
			Row row;
			RowCollection rows = Rows;
			FontHelper fontHelper = 
				ZoomedFont.Create(DefaultStyle.Font, Zoom).FontHelper;
			int height = 0;
			int textAreaHeight = TextAreaRectangle.Height;

			#endregion

			if (line < topRow)
				return null;

			if (rows == null)
				return null;

			for (int i = topRow, count = rows.Count; i < count; i++)
			{
				row = rows[i];
				int h = row.CalcRowHeight(this);
				height += h;

				// Если нашли строку...
				if (line == i)
				{
					// Заполняем координаты по вертикали...
					int y = height;
					// и вычисляем положение символа.

					#region Рассчет координат символа (по оси X).

					int textLen = row.TextLength;
					int wOffcet = StateBarWidth;

					if (textLen == 0)
						return new Rectangle(wOffcet - HorizontalScrollOffset,
							y - fontHelper.Height, 0, h);

					if (pos > textLen)
						throw new ArgumentOutOfRangeException("viewPosition",
							viewPosition, "The Character of position out of range.");

					StyledRange[] styledRanges = Utils.ToStyledRange(row.AppliedStyles, 
						DefaultStyle, row.Text, true);

					int virtualOffset = row.ViewPosition;
					int lenAll = 0;

					foreach (StyledRange styledRange in styledRanges)
					{
						#region Расчет позиции символа для диапазона с единым стилем.

						Style style = styledRange.Style;
						string text = styledRange.Text;
						int len = text.Length;
						lenAll += len;

						// Если стиль позволяет получить картинку...
						if (style.DisplayImageHandler != null)
						{	// то вычисляем ширину стилевого диапазона по картинке.
							Image img = style.DisplayImageHandler(text);

							// Если нашли диапазон в который попадает искомая 
							// позиция символа...
							if (pos < len)
							{
								// текущий сдвиг это и есть искомаях X-координата.
								int x = wOffcet;
								// Если позиция указывает на конец строки.
								// иначе высчитываем ширину символа идущего за позицией.
								int w = pos >= len ? 0 : img.Width;


								return new Rectangle(x - HorizontalScrollOffset,
									y - img.Height, w, img.Height);
							}

							wOffcet += img.Width;
						}
						else // иначе рассчет позицци символа посимвольно.
						{
							//TODO: Надо бы все же сделать так чтобы в StyledRange хнанились
							// CompleteStyle (с CompleteFont).
							fontHelper = 
								ZoomedFont.Create((CompleteFont)style.Font, Zoom).FontHelper;
							char[] chars = text.ToCharArray();

							// Если нашли диапазон в который попадает искомая 
							// позиция символа...
							if (pos < len)
							{
								// Вычисляем шируну остактка диапазона (по позицию).
								wOffcet += fontHelper.MeasureWidth(chars, 0, pos,
									ref virtualOffset, TabSize, ShowInvisibles);

								// текущий сдвиг это и есть искомаях X-координата.
								int x = wOffcet;
								// Если позиция указывает на конец строки.
								int w = pos >= len ? 0
									// иначе высчитываем ширину символа идущего за позицией.
									: fontHelper.MeasureWidth(chars, pos, 1,
										ref virtualOffset, TabSize, ShowInvisibles);

								return new Rectangle(x - HorizontalScrollOffset,
									y - fontHelper.Height, w, fontHelper.Height);
							}

							wOffcet += fontHelper.MeasureWidth(chars, 0, len,
								ref virtualOffset, TabSize, ShowInvisibles);
						}

						pos -= len;

						#endregion
					}

					return new Rectangle(wOffcet - HorizontalScrollOffset,
						y - fontHelper.Height, 0, fontHelper.Height);

					#endregion
				}

				if (height >= textAreaHeight)
					return null;
			}

			return null;
		}
		
		#endregion

		#region HitTest (Возвращает информацию о том что пересекается с некоторой точкой)

		public HitTestInfo HitTest(Point point, bool isUseBestFit)
		{
			#region Инициализация

			if (Rows.Count == 0)
				return new HitTestInfo(ViewArea.BelowText);

			int y = point.Y;
			int x = point.X + HorizontalScrollOffset;
			Rectangle textAreaRect = TextAreaRectangle;
			bool isInTextArea = textAreaRect.Contains(point);

			#endregion

			#region Рассчет индекса строки

			if (!isInTextArea && !isUseBestFit)
				return new HitTestInfo(ViewArea.StatBar);

			int index;

			if (isUseBestFit)
			{
				if (point.Y <= textAreaRect.Top)
					index = FirstVisibleRow;
				else if (point.Y > textAreaRect.Bottom)
					index = FirstVisibleRow + CalcCurrentPageRowCount();
				else
					index = GetViewRowIndexByTopOffset(y);

				index = index < 0 || index >= Rows.Count ? Rows.Count - 1 : index;
			}
			else
			{
				index = GetViewRowIndexByTopOffset(y);

				if (index < 0)
					return new HitTestInfo(ViewArea.BelowText);
			}

			#endregion

			#region Рассчет позицци символа (по горизонтали).

			Row row = Rows[index];

			StyledRange[] styledRanges = Utils.ToStyledRange(row.AppliedStyles, 
				DefaultStyle, row.Text, true);

			int viewPosition = row.ViewPosition;
			int width = StateBarWidth;

			foreach (StyledRange styledRange in styledRanges)
			{
				#region Рассчет позицци символа для диапазазона с еденым стилем.

				string text = styledRange.Text;
				Style style = styledRange.Style;

				// Если стиль позволяет получить картинку...
				if (style.DisplayImageHandler != null)
				{	// то вычисляем ширину стилевого диапазона по картинке.
					Image img = style.DisplayImageHandler(text);
					int w = img.Width;

					if (width + w >= x)
						return MakeViewPosition(index, row, x - width,
							styledRange.StartPos, 0, w,
							styledRange.EndPos - styledRange.StartPos + 1);

					width += w;
				}
				else // иначе рассчет позицци символа посимвольно.
				{
					char[] chars = text.ToCharArray();
					//TODO: CompleteFont...
					FontHelper fontHelper = 
						ZoomedFont.Create((CompleteFont)style.Font, Zoom).FontHelper;

					for (int i = 0; i < chars.Length; i++)
					{
						// Посимвольно подсчитываем длинну строки пока не найдем 
						// место где x укажет на нужный символ.
						int w = fontHelper.MeasureWidth(chars, i, 1,
							ref viewPosition, TabSize, ShowInvisibles);

						if (width + w >= x)
							return MakeViewPosition(index, row, x - width,
								styledRange.StartPos, i, w, 1);

						width += w;
					}
				}

				#endregion
			}

			if (isUseBestFit)
				return new HitTestInfo(ViewArea.Text,
					new Position<IView>(index, row.TextLength), row);
			else
				return new HitTestInfo(ViewArea.LeftHandOfText,
					new Position<IView>(index, -1), row);

			#endregion
		}

		private static HitTestInfo MakeViewPosition(
			int viewLineIndex, Row row, int offset,
			int startPos, int i, int charWidth, int chadsCount)
		{
			bool useNextChar = offset >= charWidth >> 1;
			int chIndex = useNextChar ? i + startPos + chadsCount
				: i + startPos;

			return new HitTestInfo(ViewArea.Text,
				new Position<IView>(viewLineIndex, chIndex), row);
		}

		private int GetViewRowIndexByTopOffset(int y)
		{
			int topOffset = 0;

			RowCollection rows = Rows;
			FontHelper fontHelper = 
				ZoomedFont.Create(DefaultStyle.Font, Zoom).FontHelper;

			for (int i = FirstVisibleRow, count = rows.Count; i < count; i++)
			{
				int h = rows[i].CalcRowHeight(this);

				topOffset += h;

				if (topOffset >= y)
					return i;

				PaintDebugMarker(ref topOffset);
			}

			return -1;
		}

		#endregion

		#region TestPosition (Проверяет Position<IView> на корректность)

		void TestPosition(Position<IView> position, string positionName)
		{
			RowCollection rows = Rows;

			if (position.Line < 0
				|| position.Line >= rows.Count
				|| position.Character < 0
				|| position.Character > rows[position.Line].TextLength)
				ThrowInvalidPosition(position, positionName);
		}

		private static void ThrowInvalidPosition(
			Position<IView> position, string positionName)
		{
			throw new ArgumentException("The position '"
					+ positionName + "' is invalid (" + position + ").", positionName);
		}
		
		#endregion
	}
}
