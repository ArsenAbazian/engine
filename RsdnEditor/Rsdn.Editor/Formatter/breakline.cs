using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor.Formatter
{
	struct BreakLine
	{
		public BreakLine(IView view)
			: this()
		{
			_view = view;

			_width = view.TextAreaWidth;
			_defaultStyle = view.DefaultStyle;

			if (_width < 20)
				_width = 20;

			_tabSize = view.TabSize;
			_showInvisibles = view.ShowInvisibles;
			_rows = new List<Row>(10);
			_hypPoss = new List<int>(100);
			_zoom = _view.Zoom;
		}

		#region Переменные используемые алгоритмом переноса.

		private IView _view;
		private CompleteStyle _defaultStyle;
		private int _tabSize;
		private bool _showInvisibles;
		private SymType _curStat;
		private List<Row> _rows;
		private int _width;
		private char[] _chars;
		private DocumentRow _documentRow;
		private StyledRange[] _ranges;
		private int _end;
		private List<int> _hypPoss;
		private float _zoom;

		#endregion


		public List<Row> Do(DocumentRow documentRow)
		{
			#region Инициализация переменных

			_rows.Clear();
			_documentRow = documentRow;
			string text = documentRow.Text;
			int textLen = text.Length;
			_end = textLen - 1;
			_chars = new char[textLen + 2];
			text.CopyTo(0, _chars, 0, textLen);
			_ranges = Utils.ToStyledRange(documentRow.AppliedStyles, _defaultStyle,
				text, false);

			#endregion

			// 1. Создаем динамический массив позиций возмоного переноса
			// 2. Добавляем в этот массив стили, создавая таким образом
			//    отрезки текста по которым можно производитль перенос и выводимые единым шрифтом
			// 3. Пробегаемся по этому массиву и подсчитываем ширину. Если ширина привысила
			//    ширину текстовой области, переносим строку.

			if (textLen <= 0)
				return _rows;

			FillHyphenationPositions();

			#region Дополнение списка позиций переносов специальными позициями

			// Добавляем в массив возможных переносво смену стилей
			int end = _hypPoss.Count - 1;
			int index;
			for (int i = 0; i < _ranges.Length; i++)
			{
				int pos = _ranges[i].StartPos;
				index = Utils.BinarySearch(_hypPoss, 0, end, pos);
				if (index < 0)
				{
					_hypPoss.Insert(~index, pos);
					end++;
				}
			}

			if (end >= 0)
			{	// Если первый диапазон не начинается в начале строки, то вводим 
				// фиктинвое начало диапазона. Это позволит не заморачиваться с 
				// переводом полседовательно идущие начала диапазонов в отрезки.
				if (_hypPoss[0] != 0)
					_hypPoss.Add(0);
			}
			
			#endregion

			// Вводим фиктивное начало диапазона для упрощения алгоритма.
			// это позволит не контролировать хвостовой диапазон.
			_hypPoss.Add(textLen);

			// Вычисляем переносы.

			#region Переменные используемые при переносе строки.

			// Предыдущая позиция (начало диапазаона подлежащего переносу).
			int startPos = 0;
			int width = 0; // ширина диапазаона подлежащего переносу
			// Предыдущая виртуальная позиция (с учетом размера табуляции).
			int prevVirtualPos = 0;
			int virtualPos = 0; // Текущая виртуальная позиция 
			int lineStartVirtualPos = 0; // виртуальная позиция начала строки
			int lineStart = 0; // позиция начала текущей строки
			int rangeIndex = 0; // идинтификатор стилевого диапазона
			// Следующий стилевой диапазон. При совпадении обрабатываемой позиции
			// с позицией следующего диапазано он делается текущим, а в эту 
			// переменную помещается следующий диапазон.
			StyledRange range = _ranges.Length > 0 ? _ranges[0] : new StyledRange();
			// Текущий FontHelper. Испоьзуется для рассчета текущей позиции
			// текстового отрезка.
			FontHelper fontHelper = 
				ZoomedFont.Create(_defaultStyle.Font, _zoom).FontHelper;
			Image image = null;

			#endregion

			// Перебираем текстовые отрезки... В списке отрезков обязательно 
			// присутствует позиция "0" и позиция "за предам строки". Это позволяет
			// вычислять отрезки list[i] - list[i + 1]
			for (int i = 0; i < _hypPoss.Count; i++)
			{
				// Текущая позиция в строке. Она одновременно является концом текстовго
				// отрезка который возможно прийдется переностить и началом отрезка для
				// которого вычисляется ширина (которые проверяется на превышение ширины
				// текстовой области).
				int endPos = _hypPoss[i];
				int len = endPos - startPos; // Длинна отрезка строки
				endPos--;
				if (len <= 0) // Пропускаем пустые отрезки.
					continue;

				#region Обработка стиля.

				if (range.StartPos == startPos) // пора менять стиль...
				{
					Style style = range.Style;
					fontHelper = 
						ZoomedFont.Create((CompleteFont)style.Font, _zoom).FontHelper;
					int rangeEnd;

					rangeIndex++;
					if (rangeIndex < _ranges.Length)
					{
						range = _ranges[rangeIndex];
						rangeEnd = range.StartPos - 1;
					}
					else
						rangeEnd = textLen;

					#region Пытаемся получить от стиля картинку

					DisplayImage displayImage = style.DisplayImageHandler;
					if (displayImage != null)
					{
						endPos = rangeEnd;
						len = rangeEnd - startPos + 1;
						image = displayImage(new string(_chars, startPos, len));

						if (image == null)
							throw new Exception("Style " + style.Name
								+ " return null image reference.");


						// Перебираем позиции выкидывая те что приходятся на данный стиль.
						for (int k = i + 1; k < _hypPoss.Count && _hypPoss[k] <= rangeEnd; )
							_hypPoss.RemoveAt(k);
					}
					else
						image = null;
					
					#endregion 

				}

				#endregion

				// Вычисляем ширину отрезка.
				int w = image == null
					? fontHelper.MeasureWidth(_chars, startPos, len,
							ref virtualPos, _tabSize, _showInvisibles)
					: image.Width;

				#region Посимвольный перенос строки (когда отрезки в строку не влезают).

				// b.	Если ширина отрезка превышает ширину текстовой области и 
				// отрезок не начинается с пробельного символа нужно перенести строку.
				if (w > _width/* && !Utils.IsWhiteSpace(_chars[startPos])*/)
				{
					if (startPos - lineStart > 0)
					{
						_rows.Add(new HyphenationRow(_documentRow, lineStart,
												startPos - lineStart, lineStartVirtualPos, false));

						lineStart = startPos;
						width = 0;
					}

					int segmentWidth = w;
					virtualPos = prevVirtualPos; // востанавливаем виртуальную позицию
					// Рассчитываем ширину каждого символа и проверям влезает ли он в 
					// текстовую область...
					for (int j = startPos; j <= endPos; j++)
					{
						w = fontHelper.MeasureWidth(_chars, j, 1,
							ref virtualPos, _tabSize, _showInvisibles);

						width += w;

						// Если часть отрезка превышает ширину текстовой области...
						if (width > _width)
						{
							w = 0;
							startPos = j;
							endPos = j - 1;
							i--;
							virtualPos = prevVirtualPos;
							break;
						}

						prevVirtualPos = virtualPos;
					}
				}
				width += w;
				
				#endregion

				#region Перенос строки по текстовым отрезкам.

				// Если ширина отрезка в сумме с шириной текста от начала строки
				// превышает ширину текстовой области и приэтом текущий отрезок
				// не состоит из пробельных символов, то нужно переносить строку...
				if (width > _width/* && !Utils.IsWhiteSpace(_chars[startPos])*/)
				{
					// Переносим строку.
					_rows.Add(new HyphenationRow(_documentRow, lineStart,
						startPos - lineStart, lineStartVirtualPos, false));

					lineStartVirtualPos = prevVirtualPos;
					lineStart = startPos;
					width = w;
				}

				startPos = endPos + 1;
				prevVirtualPos = virtualPos;
			}
			
			#endregion

			if (_rows.Count != 0)
				_rows.Add(new HyphenationRow(_documentRow, lineStart,
					textLen - lineStart, lineStartVirtualPos, true));

			// Чтобы не обломаться с многопоточностью забываем о списке перед
			// выходом из функции.
			return _rows;
		}

		private void FillHyphenationPositions()
		{
			_hypPoss.Clear();

			// Бегаем по строке... переходим между состояниями и пытаемся 
			// произвести перенос строки функцей TryHyphenate.
			for (int i = 0; i < _chars.Length - 2; i++)
			{
				switch (_curStat)
				{
					case SymType.None: SetNewState(_chars[i]); break;
					case SymType.Word:
						if (!TryCangeStateFromWord(_chars[i]))
						{
							switch (_chars[i + 1])
							{ // На этих символы не не переносим (так так красивее).
								case ';':
								case ')':
								case ',':
								case '>':
								case ']': i++; break;
							}

							continue;
						}

						//if (_curStat != SymType.Space)
						_hypPoss.Add(i);
						break;
					case SymType.Punctuation:
						if (!TryCangeStateFromPunctuation(_chars[i]))
							continue;
						//if (_curStat != SymType.Space)
						_hypPoss.Add(i);
						break;
					case SymType.Space:
						if (!TryCangeStateFromSpace(_chars[i]))
							continue;

						_hypPoss.Add(i);
						break;
					default: throw new Exception();
				}
			}
		}

		#region Переход между состояниями

		private void SetNewState(char ch)
		{
			if (Utils.IsWhiteSpace(ch))
				_curStat = SymType.Space;
			else if (Utils.IsPunctuation(ch))
				_curStat = SymType.Punctuation;
			else if (Utils.IsLetterOrDigit(ch))
				_curStat = SymType.Word;
		}

		private bool TryCangeStateFromWord(char ch)
		{
			if (Utils.IsLetterOrDigit(ch))
				return false;
			if (Utils.IsPunctuation(ch))
				_curStat = SymType.Punctuation;
			else if (Utils.IsWhiteSpace(ch))
				_curStat = SymType.Space;
			else
				_curStat = SymType.None;

			return true;
		}

		private bool TryCangeStateFromSpace(char ch)
		{
			if (Utils.IsWhiteSpace(ch))
				return false;
			if (Utils.IsPunctuation(ch))
				_curStat = SymType.Punctuation;
			else if (Utils.IsLetterOrDigit(ch))
				_curStat = SymType.Word;
			else
				_curStat = SymType.None;

			return true;
		}

		private bool TryCangeStateFromPunctuation(char ch)
		{
			if (Utils.IsPunctuation(ch))
				return false;
			if (Utils.IsWhiteSpace(ch))
				_curStat = SymType.Space;
			else if (Utils.IsLetterOrDigit(ch))
				_curStat = SymType.Word;
			else
				_curStat = SymType.None;

			return true;
		}

		#endregion
	}
}
