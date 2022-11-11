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

		#region ���������� ������������ ���������� ��������.

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
			#region ������������� ����������

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

			// 1. ������� ������������ ������ ������� ��������� ��������
			// 2. ��������� � ���� ������ �����, �������� ����� �������
			//    ������� ������ �� ������� ����� ������������ ������� � ��������� ������ �������
			// 3. ����������� �� ����� ������� � ������������ ������. ���� ������ ���������
			//    ������ ��������� �������, ��������� ������.

			if (textLen <= 0)
				return _rows;

			FillHyphenationPositions();

			#region ���������� ������ ������� ��������� ������������ ���������

			// ��������� � ������ ��������� ��������� ����� ������
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
			{	// ���� ������ �������� �� ���������� � ������ ������, �� ������ 
				// ��������� ������ ���������. ��� �������� �� �������������� � 
				// ��������� ��������������� ������ ������ ���������� � �������.
				if (_hypPoss[0] != 0)
					_hypPoss.Add(0);
			}
			
			#endregion

			// ������ ��������� ������ ��������� ��� ��������� ���������.
			// ��� �������� �� �������������� ��������� ��������.
			_hypPoss.Add(textLen);

			// ��������� ��������.

			#region ���������� ������������ ��� �������� ������.

			// ���������� ������� (������ ���������� ����������� ��������).
			int startPos = 0;
			int width = 0; // ������ ���������� ����������� ��������
			// ���������� ����������� ������� (� ������ ������� ���������).
			int prevVirtualPos = 0;
			int virtualPos = 0; // ������� ����������� ������� 
			int lineStartVirtualPos = 0; // ����������� ������� ������ ������
			int lineStart = 0; // ������� ������ ������� ������
			int rangeIndex = 0; // ������������� ��������� ���������
			// ��������� �������� ��������. ��� ���������� �������������� �������
			// � �������� ���������� ��������� �� �������� �������, � � ��� 
			// ���������� ���������� ��������� ��������.
			StyledRange range = _ranges.Length > 0 ? _ranges[0] : new StyledRange();
			// ������� FontHelper. ����������� ��� �������� ������� �������
			// ���������� �������.
			FontHelper fontHelper = 
				ZoomedFont.Create(_defaultStyle.Font, _zoom).FontHelper;
			Image image = null;

			#endregion

			// ���������� ��������� �������... � ������ �������� ����������� 
			// ������������ ������� "0" � ������� "�� ������ ������". ��� ���������
			// ��������� ������� list[i] - list[i + 1]
			for (int i = 0; i < _hypPoss.Count; i++)
			{
				// ������� ������� � ������. ��� ������������ �������� ������ ���������
				// ������� ������� �������� ��������� ����������� � ������� ������� ���
				// �������� ����������� ������ (������� ����������� �� ���������� ������
				// ��������� �������).
				int endPos = _hypPoss[i];
				int len = endPos - startPos; // ������ ������� ������
				endPos--;
				if (len <= 0) // ���������� ������ �������.
					continue;

				#region ��������� �����.

				if (range.StartPos == startPos) // ���� ������ �����...
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

					#region �������� �������� �� ����� ��������

					DisplayImage displayImage = style.DisplayImageHandler;
					if (displayImage != null)
					{
						endPos = rangeEnd;
						len = rangeEnd - startPos + 1;
						image = displayImage(new string(_chars, startPos, len));

						if (image == null)
							throw new Exception("Style " + style.Name
								+ " return null image reference.");


						// ���������� ������� ��������� �� ��� ���������� �� ������ �����.
						for (int k = i + 1; k < _hypPoss.Count && _hypPoss[k] <= rangeEnd; )
							_hypPoss.RemoveAt(k);
					}
					else
						image = null;
					
					#endregion 

				}

				#endregion

				// ��������� ������ �������.
				int w = image == null
					? fontHelper.MeasureWidth(_chars, startPos, len,
							ref virtualPos, _tabSize, _showInvisibles)
					: image.Width;

				#region ������������ ������� ������ (����� ������� � ������ �� �������).

				// b.	���� ������ ������� ��������� ������ ��������� ������� � 
				// ������� �� ���������� � ����������� ������� ����� ��������� ������.
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
					virtualPos = prevVirtualPos; // �������������� ����������� �������
					// ������������ ������ ������� ������� � �������� ������� �� �� � 
					// ��������� �������...
					for (int j = startPos; j <= endPos; j++)
					{
						w = fontHelper.MeasureWidth(_chars, j, 1,
							ref virtualPos, _tabSize, _showInvisibles);

						width += w;

						// ���� ����� ������� ��������� ������ ��������� �������...
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

				#region ������� ������ �� ��������� ��������.

				// ���� ������ ������� � ����� � ������� ������ �� ������ ������
				// ��������� ������ ��������� ������� � ������� ������� �������
				// �� ������� �� ���������� ��������, �� ����� ���������� ������...
				if (width > _width/* && !Utils.IsWhiteSpace(_chars[startPos])*/)
				{
					// ��������� ������.
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

			// ����� �� ���������� � ���������������� �������� � ������ �����
			// ������� �� �������.
			return _rows;
		}

		private void FillHyphenationPositions()
		{
			_hypPoss.Clear();

			// ������ �� ������... ��������� ����� ����������� � �������� 
			// ���������� ������� ������ ������� TryHyphenate.
			for (int i = 0; i < _chars.Length - 2; i++)
			{
				switch (_curStat)
				{
					case SymType.None: SetNewState(_chars[i]); break;
					case SymType.Word:
						if (!TryCangeStateFromWord(_chars[i]))
						{
							switch (_chars[i + 1])
							{ // �� ���� ������� �� �� ��������� (��� ��� ��������).
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

		#region ������� ����� �����������

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
