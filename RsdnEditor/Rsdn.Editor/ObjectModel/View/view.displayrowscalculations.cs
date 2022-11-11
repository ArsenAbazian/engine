// Подсчеты количества строк на странице.

using Rsdn.Editor.Formatter;

namespace Rsdn.Editor
{
	partial class View
	{
		#region CalcNextPageRowCount (Рассчет количества строк на следующей странице).

		/// <summary>
		/// Подсчитывает количество строк полностью влезающих на страницу 
		/// отображаемую в данный момент.
		/// </summary>
		public int CalcCurrentPageRowCount()
		{
			return CalcNextPageRowCount(FirstVisibleRow);
		}

		/// <summary>
		/// Подсчитывает количество строк полностью влезающих на страницу 
		/// (в даный вид) начинающуюся с виртуальной строки с индексом start.
		/// </summary>
		/// <param name="start">
		/// Номер виртуальной которая должна являться первой видимой строкой для 
		/// этой сраницы. То есть строки начиная с котрой нужно произвести 
		/// подсчет.
		/// </param>
		/// <returns>Количество строк полностью влезающих во вью.</returns>
		public int CalcNextPageRowCount(int start)
		{
			int textAreaHeight = TextAreaRectangle.Height;
			RowCollection rows = Rows;
			FontHelper fontHelper = 
				ZoomedFont.Create(DefaultStyle.Font, Zoom).FontHelper;
			int h = 0;

			int i = start;

			for (int count = rows.Count; i < count; i++)
			{
				h += rows[i].CalcRowHeight(this);

				if (h >= textAreaHeight)
					return i - start;
			}

			return i - start;
		}

		#endregion

		#region CalcPreviousPageRowCount (Рассчет количества строк на следующей странице).
		
		/// <summary>
		/// Подсчитывает сколько виртуальных строк нужно пролистать вверх, 
		/// чтобы отобразить предыдущую страницу.
		/// </summary>
		/// <returns></returns>
		public int CalcPreviousPageRowCount()
		{
			return CalcPreviousPageRowCount(FirstVisibleRow - 1);
		}

		/// <summary>
		/// Подсчитывает количество строк полностью влезающих на предыдущую 
		/// страницу (в даный вид) начинающуюся с виртуальной строки с индексом start.
		/// Другими словами функция подсчитывает скольк строк нужно 
		/// пролистать вверх чтобы перейти на предыдущую страницу.		/// </summary>
		/// <param name="start">
		/// Номер виртуальной которая должна являться следующей за последней
		/// видимой строкой для предыдущей сраницы.
		/// </param>
		/// <returns>Количество строк которые нужно пролистать вверх.</returns>
		public int CalcPreviousPageRowCount(int start)
		{
			int textAreaHeight = TextAreaRectangle.Height;
			RowCollection rows = Rows;
			FontHelper fontHelper = 
				ZoomedFont.Create(DefaultStyle.Font, Zoom).FontHelper;
			int h = 0;

			int i = start;

			for (; i >= 0; i--)
			{
				h += rows[i].CalcRowHeight(this);

				if (h >= textAreaHeight)
					return start - i;
			}

			return start - i;
		}
		
		#endregion

		#region CalcTopPageRowCount (Рассчет количества строк на первой странице).

		/// <summary>
		/// Подсчитывает количество строк вмещается в данный вид в первой 
		/// странице текущего документа.
		/// </summary>
		public int CalcTopPageRowCount()
		{
			return CalcNextPageRowCount(0);
		}
		
		#endregion

		#region CalcBottomPageRowCount (Рассчет количества строк на последней странице).

		/// <summary>
		/// Подсчитывает количество строк вмещается в данный вид в последней 
		/// странице текущего документа.
		/// </summary>
		public int CalcBottomPageRowCount()
		{
			return CalcPreviousPageRowCount(Rows.Count - 1);
		}

		#endregion
	}
}
