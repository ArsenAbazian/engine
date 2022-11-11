using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	/// <summary>
	/// Виртуальная строка полученная в результате перетекания обычной строки.
	/// </summary>
	public class HyphenationRow : Row
	{
		/// <summary>
		/// Описывает строку образованную в результате переноса строки.
		/// </summary>
		/// <param name="viewRow">
		/// Реальная строка на которую отоборажается данная.
		/// </param>
		/// <param name="startOffset">
		/// Отступ в раельной строке с которого начичитаются данная строка.
		/// </param>
		/// <param name="len">
		/// Длинна данной строки..
		/// </param>
		/// <param name="virtualPos">
		/// Виртуальная позиция начала данной строки.
		/// Виртуальная позиция - это подразумеваемая позиция которая которая
		/// получилась если бы все табуляции и нивидимые знаки были бы заменены
		/// на соотвествующие подстановочные символы.
		/// </param>
		public HyphenationRow(DocumentRow row, int startOffset, int len, int virtualPos)
			: this(row, startOffset, len, virtualPos, false)
		{
		}

		public HyphenationRow(DocumentRow row, int startOffset, int len,
			int virtualPos, bool isEndLine)
		{
			_documentRow = row;
			_startOffset = startOffset;
			_len = len;
			_virtualPos = virtualPos;
			_isEndLine = isEndLine;
		}

		private DocumentRow _documentRow;

		public override DocumentRow RealDocumentRow
		{
			[DebuggerHidden]
			get { return _documentRow; }
		}

		private int _startOffset;
		private int _len;
		private int _virtualPos;

		private bool _isEndLine;

		internal override bool IsEndLine
		{
			[DebuggerHidden]
			get { return _isEndLine; }
		}

		public override string GetExpandedText(IView view)
		{
			return Utils.ExpandTabs(GetText(), _virtualPos, view.TabSize,
				view.ShowInvisibles, _isEndLine);
		}

		public override int ViewPosition
		{
			[DebuggerHidden]
			get { return _virtualPos; }
		}

		[DebuggerHidden]
		private string GetText()
		{
			return _documentRow.Text.Substring(_startOffset, _len);
		}

		public override string Text
		{
			[DebuggerHidden]
			get { return GetText(); }
			set
			{
				throw new NotSupportedException(
					"Modification Text property of HyphenationRow not suported.");
			}
		}

		public override int TextLength
		{
			[DebuggerHidden]
			get { return _len; }
		}

		public override int OffsetInDocumentRow
		{
			[DebuggerHidden]
			get { return _startOffset; }
		}

		public override AppliedStyle[] AppliedStyles
		{
			get
			{
				// Формируем список стилей попадающих в диапазон виртуальной строки.
				// Виртуальная строка занимает только часть (диапазон) реальной строки
				// стили же хранятся для реальной строки. Поэтому их нужно отфильтровать
				// и скоректировать так, чтобы они не выходили за пределы виртуальной 
				// строки. Так же нужно скоректировать позиции стилей, чтобы пользователь
				// смог работать с ними как будто он рабоатет с реальной строкой.
				AppliedStyle[] styles = _documentRow.InternalPosStyles;

				List<AppliedStyle> retList = new List<AppliedStyle>(styles.Length);

				// Индекс первого символа виртуальной строки в реальной.
				int strStart = _startOffset;
				// Индекс последнего символа виртуальной строки в реальной.
				int strEnd = _startOffset + _len - 1;

				for (int i = 0; i < styles.Length; i++)
				{
					AppliedStyle posStyle = styles[i];
					// Индекс окончания стиля в реальной строке.
					int styleEnd = posStyle.Pos + posStyle.Length - 1;

					// Вычисляем отрезок (пересечение отрезка стиля и отрезка виртуальной
					// строки. Все значения считаются относительно реальной строки.
					int start = Math.Max(strStart, posStyle.Pos);
					int end = Math.Min(strEnd, styleEnd);

					// Если стиль пересекся с диапазоном входящим в виртуальную строку...
					int posStyleLength = end - start + 1;

					if (posStyleLength > 0)
					{
						// Кооректируем позицию и длинну стиля, чтобы он отсчитывался 
						// относительно виртуальной, а н реальной строки...
						posStyle.Pos = start - strStart;
						posStyle.Length = posStyleLength;
						retList.Add(posStyle); // и заносим его в список.
					}
				}

				// небольшая оптимизация на случай если стилей нет.
				if (retList.Count == 0)
					return _defaultPosStyleArray;

				return retList.ToArray();
			}

			set { throw new Exception("The method or operation is not implemented."); }
		}

		private void CompensateOffset(ref AppliedStyle appliedStyle, int endLine)
		{
			if (appliedStyle.Pos > _startOffset)
				appliedStyle.Pos -= _startOffset;
			else
			{
				appliedStyle.Length -= _startOffset - appliedStyle.Pos;
				appliedStyle.Pos = 0;
			}

			if (appliedStyle.EndPos > endLine)
				appliedStyle.EndPos = endLine;
		}

		[DebuggerHidden]
		public override void AcceptVisitor(IRowVisitor visitor)
		{
			visitor.Visit(this);
		}
	}
}
