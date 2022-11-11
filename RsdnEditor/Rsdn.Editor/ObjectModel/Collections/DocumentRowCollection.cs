using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;
using System.ComponentModel;
using System.Diagnostics;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	public class DocumentRowCollection : Collection<DocumentRow>
	{
		[DebuggerHidden]
		internal DocumentRowCollection(Document document)
		{
			if (document == null)
				throw new ArgumentNullException("document");

			Document = document;
		}

		private Document _document;

		[Browsable(false)]
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public Document Document
		{
			[DebuggerHidden]
			get { return _document; }
			[DebuggerHidden]
			internal set { _document = value; }
		}

		[DebuggerHidden]
		protected override void ClearItems()
		{
			base.ClearItems();
			Document.TextUpdated();
		}

		[DebuggerHidden]
		public override string ToString()
		{
			StringBuilder cb = new StringBuilder(1000);

			bool isFirst = true;

			foreach (DocumentRow row in Items)
			{
				if (isFirst)
					isFirst = false;
				else
					cb.Append(Environment.NewLine);

				cb.Append(row.Text);
			}

			return cb.ToString();
		}

		[DebuggerHidden]
		internal void Init(string text)
		{
			using (FreezeEventHelper herper = new FreezeEventHelper(this))
			{
				Document.ResetViewsInfo();

				Items.Clear();

				foreach (string line in Parse(text))
					Items.Add(new DocumentRow(line));

				Document.Formatter.PrepareRows(this);
			}
		}

		[DebuggerHidden]
		internal void AddRange(DocumentRowCollection rows)
		{
			List<DocumentRow> list = (List<DocumentRow>)Items;
			list.AddRange(rows);
		}

		[DebuggerHidden]
		internal void AddRange(IEnumerable<DocumentRow> rows)
		{
			List<DocumentRow> list = (List<DocumentRow>)Items;
			list.AddRange(rows);
		}

		[DebuggerHidden]
		public new int IndexOf(DocumentRow row)
		{
			List<DocumentRow> list = (List<DocumentRow>)Items;
			for (int i = 0, length = list.Count; i < length; i++)
				if (list[i] == row)
					return i;

			return -1;
		}

		[DebuggerHidden]
		internal void InsertRange(int index, IEnumerable<DocumentRow> rows)
		{
			List<DocumentRow> list = (List<DocumentRow>)Items;
			list.InsertRange(index, rows);
		}

		internal void RemoveRange(int start, int count)
		{
			List<DocumentRow> list = (List<DocumentRow>)Items;
			list.RemoveRange(start, count);
		}

		[DebuggerHidden]
		public DocumentRowCollection GetRangeCollection(int start, int count)
		{
			DocumentRow[] range = GetRange(start, count);

			DocumentRowCollection copy = new DocumentRowCollection(Document);
			List<DocumentRow> copyList = (List<DocumentRow>)copy.Items;

			copyList.AddRange(range);

			return copy;
		}

		public DocumentRow[] GetRange(int start, int count)
		{
			Debug.Assert(start + count <= Count);

			DocumentRow[] range = new DocumentRow[count];
			List<DocumentRow> sourceList = (List<DocumentRow>)Items;
			sourceList.CopyTo(start, range, 0, count);

			return range;
		}

		[DebuggerHidden]
		public DocumentRow[] ToArray()
		{
			List<DocumentRow> list = (List<DocumentRow>)Items;
			return list.ToArray();
		}

		#region Parse

		public static string[] Parse(string text)
		{
			if (text == null)
				text = "";

			int len = text.Length;
			char[] chars = new char[len + 2];
			text.CopyTo(0, chars, 0, len);
			int prev = 0;
			List<string> list = new List<string>();

			for (int i = 0; i < len; i++)
			{
				if (chars[i] == '\r')
				{
					list.Add(new string(chars, prev, i - prev));

					if (chars[i + 1] == '\n')
						i++;

					prev = i + 1;
				}
				else if (chars[i] == '\n')
				{
					list.Add(new string(chars, prev, i - prev));
					prev = i + 1;
				}
			}

			if (prev <= len)
				list.Add(new string(chars, prev, len - prev));
			
			return list.ToArray();
		}

		public static string[] Parse(TextReader reader)
		{
			List<string> list = new List<string>();
			string line;

			while ((line = reader.ReadLine()) != null)
				list.Add(line);

			return list.ToArray();
		}
		
		#endregion

		#region FreezeEventHelper
		
		private bool _isRiseEvents = true;

		private class FreezeEventHelper : IDisposable
		{
			public FreezeEventHelper(DocumentRowCollection rows)
			{
				_rows = rows;
				_isRiseEvents = rows._isRiseEvents;
				rows._isRiseEvents = false;
			}

			private bool _isRiseEvents;
			private DocumentRowCollection _rows;

			#region IDisposable Members

			public void Dispose()
			{
				_rows._isRiseEvents = _isRiseEvents;
				if (_isRiseEvents)
					_rows.Document.TextUpdated();
			}

			#endregion
		}

		#endregion
	}
}
