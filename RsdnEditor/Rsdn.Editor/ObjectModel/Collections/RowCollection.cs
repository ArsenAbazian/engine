using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;
using System.ComponentModel;

namespace Rsdn.Editor
{
	public class RowCollection : Collection<Row>
	{
		internal RowCollection(int capacity)
		{
			List<Row> list = (List<Row>)Items;
			list.Capacity = capacity;
		}

		protected override void ClearItems()
		{
			base.ClearItems();
		}

		public override string ToString()
		{
			StringBuilder cb = new StringBuilder(1000);

			bool isFirst = true;

			foreach (Row row in Items)
			{
				if (isFirst)
					isFirst = false;
				else
					cb.Append(Environment.NewLine);

				cb.Append(row.Text);
			}

			return cb.ToString();
		}

		internal void AddRange(RowCollection rows)
		{
			List<Row> list = (List<Row>)Items;
			list.AddRange(rows);
		}

		internal void AddRange(DocumentRowCollection rows)
		{
			List<Row> dest = (List<Row>)Items;

			Row[] array = rows.ToArray();
			dest.AddRange(array);
		}

		internal void AddRange(IEnumerable<Row> rows)
		{
			List<Row> list = (List<Row>)Items;
			list.AddRange(rows);
		}

		public new int IndexOf(Row row)
		{
			List<Row> list = (List<Row>)Items;
			for (int i = 0, length = list.Count; i < length; i++)
				if (list[i] == row)
					return i;

			return -1;
		}


		/// <summary>Находит индекс виртуальной строки по реальной строке.</summary>
		/// <param name="viewRow">
		/// Реальная строка для которой нужно найти соотвествующий 
		/// индекс виртуальной.
		/// </param>
		/// <returns>
		/// Индекс соотвествующей виртуальной строки 
		/// или null в случае неудчаного поиска.
		/// </returns>
		public int IndexOfDocumentRow(DocumentRow row)
		{
			return IndexOfDocumentRow(0, row);
		}

		/// <summary>
		/// Находит индекс виртуальной строки по реальной строке.
		/// </summary>
		/// <param name="startIndex">
		/// Индекс с которого будте производиться поиск.
		/// </param>
		/// <param name="viewRow">
		/// Реальная строка для которой нужно найти соотвествующий 
		/// индекс виртуальной.
		/// </param>
		/// <returns>
		/// Индекс соотвествующей виртуальной строки 
		/// или null в случае неудчаного поиска.
		/// </returns>
		public int IndexOfDocumentRow(int startIndex, DocumentRow row)
		{
			List<Row> list = (List<Row>)Items;

			for (int i = startIndex, count = list.Count; i < count; i++)
				if (list[i].RealDocumentRow == row)
					return i;

			return -1;
		}

		/// <summary>Находит виртуальную строку по реальной.</summary>
		/// <param name="viewRow">
		/// Реальная строка для которой нужно найти соотвествующую виртуальную.
		/// </param>
		/// <returns>
		/// Соотвествующая виртуальная строка или null в случае неудчаного поиска.
		/// </returns>
		public Row FindByDocumentRow(DocumentRow row)
		{
			List<Row> list = (List<Row>)Items;
			for (int i = 0, length = list.Count; i < length; i++)
			{
				Row viewRow = list[i];
				if (viewRow.RealDocumentRow == row)
					return viewRow;
			}

			return null;
		}

		internal void RemoveRange(int index, int coint)
		{
			List<Row> list = (List<Row>)Items;
			list.RemoveRange(index, coint);
		}

		internal void InsertRange(int index, RowCollection virtRows)
		{
			List<Row> list = (List<Row>)Items;
			list.InsertRange(index, virtRows);
		}

		public bool IsEmpty { get { return Count == 0; } }
		public Row First { get { return this[0]; } }
		public Row Last { get { return this[Count - 1]; } }

		/// <summary>
		/// Пропускает все виртуальные строки соотвевтствующи 
		/// <paramref name="skipRealRowsCount"/> реальным строкам.
		/// </summary>
		/// <remarks>
		/// Эту функцию удобно использовать при синхронизации реальных
		/// и виртуальных строк.
		/// </remarks>
		/// <param name="startVirtRowIndex">
		/// Индекс строки с которой нужно начать пропускать строки.
		/// </param>
		/// <param name="skipRealRowsCount">
		/// Количество реальных строк которое необходимо пропустить.
		/// </param>
		/// <returns></returns>
		public int SkipRealRows(int startVirtRowIndex, int skipRealRowsCount)
		{
			List<Row> rows = (List<Row>)Items;
			int count = rows.Count;
			DocumentRow pevRow = rows[startVirtRowIndex].RealDocumentRow;
			int i = startVirtRowIndex + 1;

			for (; i < count && skipRealRowsCount > 0; i++)
			{
				DocumentRow row = rows[i].RealDocumentRow;

				if (row != pevRow)
				{
					pevRow = row;
					skipRealRowsCount--;
				}
			}

			//return i - 1;
			if (skipRealRowsCount <= 0)
				return i - 1;

			return i;
		}
	}
}
