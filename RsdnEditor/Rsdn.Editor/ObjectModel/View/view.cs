using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

using Rsdn.Editor.ObjectModel;
using System.Diagnostics;

namespace Rsdn.Editor
{
	internal partial class View : Control, IView, IViewInternal, IFreezeRepaint
	{
		//!!! Внимание! Это partial-класс. 
		// Код этого класса разнесен по отдельным файлам.
		// Если вы используете VS, воспользуйтесь плюсиком в ClassView или 
		// комбобоксом Members (сверху редактора).

		#region Конструкторы

		public View()
		{
			SetStyle(
				ControlStyles.AllPaintingInWmPaint
				| ControlStyles.UserPaint
				//| ControlStyles.OptimizedDoubleBuffer
				| ControlStyles.Opaque
				| ControlStyles.Selectable
				| ControlStyles.UserMouse, true);

			InitCursors();
			InitializeComponent();
			InitKeyboard();
			InitScrolling();
			InitPaint();
			_pal = Utils.GetIPAL(this);
			InitSelection();
		}

		#endregion

		#region Pal property

		private IPAL _pal;

		/// <summary>
		/// Тип инкапсулирующий платформно-зависимые вызовы.
		/// </summary>
		internal IPAL Pal
		{
			get { return _pal; }
		}

		#endregion

		//***********************************************************************
		#region Переопределенные свойства базового класса

		#region BackColor property

		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public override Color BackColor
		{
			get { return base.BackColor; }
			set { throw new NotImplementedException(); }
		}

		#endregion

		#region ForeColor property

		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public override Color ForeColor
		{
			get { return base.ForeColor; }
			set { throw new NotImplementedException(); }
		}

		#endregion

		#region Font property

		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public override Font Font
		{
			get { return base.Font; }
			set { throw new NotImplementedException(); }
		}

		#endregion

		#endregion

		#region Работа с фокусом ввода

		protected override void OnGotFocus(EventArgs e)
		{
			Debug.WriteLine("OnGotFocus");
			base.OnGotFocus(e);
			Invalidate();
			Update();
			Document.ActiveView = this;
			RecalcCarrenCaretPosition();
			Pal.ShowCaret();
		}

		protected override void OnLostFocus(EventArgs e)
		{
			Debug.WriteLine("OnLostFocus");
			Invalidate();
			Update();
			base.OnLostFocus(e);
			Pal.HideCaret();
		}

		#endregion

		#region RescanDocumentLines (Пересчет стилевой информации и переносов)

		/// <summary>
		/// Обрабатывает диапазон реальных строк (строк документа) обновляя 
		/// стилевую информацию и преобразуя их в виртуальные строки (строки 
		/// вида). Сканирование ведется как минимум в диапазоне реальных строк 
		/// задаваемом параметрами start и count. Поесле окончания диапазона 
		/// сканирование ведется до тех пор пока состояние Styler-а после 
		/// обработки строки не совпадет с сотоянием следующей строки.
		/// Далее (если включен режим переноса строк) производится перенос рельных 
		/// строк так чтобы они влезали в текстовую область вида и производится
		/// замена диапазона старых виртуальных строк на новые.
		/// </summary>
		/// <param name="start">
		/// Индекс первой строки диапазона рельных строк для котрых нужно обновить
		/// стилевую информацию.</param>
		/// <param name="count">
		/// Количество реальных строк входящих вобрабатываемый диапазон.
		/// </param>
		void RescanDocumentLines(int start, int count)
		{
			DocumentRowCollection docRows = Document.Rows;
			DocumentRow startDocumentRow = docRows[start];

			// Обрабатываем строки стайлером.
			count = Document.Formatter.PrepareRows(docRows, start, count);

			int end = start + count - 1;
			DocumentRow endRealRow = docRows[end];
			DocumentRowCollection documentRows = docRows.GetRangeCollection(start, count);


			RowCollection viewRows;

			if (WordWrap)
				// Преобразуем реальные строки в виртуальные.
				// При этом происходит разбиение на строки и организация 
				// схлопывающхися областей.
				viewRows = Document.Formatter.DocumentToViewRows(this, documentRows);
			else
				(viewRows = new RowCollection(documentRows.Count + 10)).AddRange(documentRows);

			RowCollection rows = Rows;
			int virtStart = rows.IndexOfDocumentRow(startDocumentRow);
			int virtEnd = rows.IndexOfDocumentRow(endRealRow);

			for (int i = virtEnd + 1, rowsEnd = rows.Count; i < rowsEnd; i++, virtEnd++)
				if (rows[i].RealDocumentRow != endRealRow)
					break;

			if (virtStart < 0)
				throw new Exception("virtStart < 0");

			if (virtEnd < 0)
				throw new Exception("virtEnd < 0");

			rows.RemoveRange(virtStart, virtEnd - virtStart + 1);
			rows.InsertRange(virtStart, viewRows);
		}


		int _virtStart;
		int _virtEnd;

		public bool BeginTextUpdated(Position<Document> startPosition, Position<Document> endPosition)
		{
			if (_selectionStartView != _selectionEndView)
				if (!SetSelection(_selectionEndView, _selectionEndView))
					return false;
			
			if (startPosition > endPosition)
				Utils.Swap(ref startPosition, ref endPosition);

			DocumentRowCollection docRows = Document.Rows;
			DocumentRow startRealRow = docRows[startPosition.Line];
			DocumentRow endRealRow = docRows[endPosition.Line];
			RowCollection rows = Rows;
			_virtStart = rows.IndexOfDocumentRow(startRealRow);
			_virtEnd = rows.IndexOfDocumentRow(endRealRow);

			// Находим последнее вхождение строки во View.
			_virtEnd = rows.SkipRealRows(_virtEnd, 1) - 1;

			return true;
		}

		public void EndTextUpdated(Position<Document> startPosition, Position<Document> endPosition)
		{
			int start = startPosition.Line;
			int count = endPosition.Line - startPosition.Line + 1;
			DocumentRowCollection docRows = Document.Rows;
			RowCollection rows = Rows;

			// Обрабатываем строки стайлером.
			int newCount = Document.Formatter.PrepareRows(docRows, start, count);

			//int countRealRowToSkip = count - (_virtEnd - _virtStart + 1);

			if (count != newCount)
				_virtEnd = Rows.SkipRealRows(_virtEnd + 1, newCount - count) - 1;

			DocumentRowCollection documentRows = docRows.GetRangeCollection(start, newCount);

			RowCollection virtRows;

			if (WordWrap)
				// Преобразуем реальные строки в виртуальные.
				// При этом происходит разбиение на строки и организация 
				// схлопывающхися областей.
				virtRows = Document.Formatter.DocumentToViewRows(this, documentRows);
			else
				(virtRows = new RowCollection(documentRows.Count + 10)).AddRange(documentRows);

			if (_virtStart < 0)
				throw new Exception("virtStart < 0");

			if (_virtEnd < 0)
				throw new Exception("virtEnd < 0");

			rows.RemoveRange(_virtStart, _virtEnd - _virtStart + 1);
			rows.InsertRange(_virtStart, virtRows);

			Invalidate();

			//TODO: Нужно модифицировать селекшон так чтобы если изменение произошло 
			// внутри него, чтобы оно соотвествующим образом изменялос.
			// Если изменение перекрывает селекшон, нужно удалять селекшон.
			// Все это нужно делть только если вью не является активным.

			//Position<IView> virtSelectionEnd = ToView(selectionEnd);
			//Position<IView> virtSelectionStart = selectionStart == selectionEnd
			//	? virtSelectionEnd : ToView(selectionStart);
			//
			//SetSelection(virtSelectionStart, virtSelectionEnd);
		}

		public void UpdatedView()
		{
			Update();
		}

		#endregion
	}
}
