using System;
using System.Collections.Generic;
using System.Text;
using System.Collections.ObjectModel;

namespace Rsdn.Collections.Generic
{
	public class CollectionEx<T> : Collection<T>
	{
		protected CollectionEx(int capacity)
		{
			List<T> list = (List<T>)Items;
			list.Capacity = capacity;
		}

		protected override void ClearItems()
		{
			base.ClearItems();
		}

		internal void AddRange(IEnumerable<T> seq)
		{
			List<T> list = (List<T>)Items;
			list.AddRange(seq);
		}

		internal void RemoveRange(int index, int coint)
		{
			List<T> list = (List<T>)Items;
			list.RemoveRange(index, coint);
		}

		internal void InsertRange(int index, IEnumerable<T> seq)
		{
			List<T> list = (List<T>)Items;
			list.InsertRange(index, seq);
		}

		public bool IsEmpty { get { return Count == 0; } }
		public T First { get { return this[0]; } }
		public T Last { get { return this[Count - 1]; } }
	}
}
