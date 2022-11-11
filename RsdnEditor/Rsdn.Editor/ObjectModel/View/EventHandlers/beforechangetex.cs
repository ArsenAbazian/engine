using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace Rsdn.Editor
{
	public class BeforeChangeTexEventArgs : CancelEventArgs
	{
		public BeforeChangeTexEventArgs(string text, 
			Position<Document> selectionStart, Position<Document> selectionEnd)
		{
			_text = text;
			_selectionStart = selectionStart;
			_selectionEnd = selectionEnd;
		}

		private string _text;

		public string Text
		{
			get { return _text; }
		}

		private Position<Document> _selectionStart;

		public Position<Document> SelectionStart
		{
			get { return _selectionStart; }
		}

		private Position<Document> _selectionEnd;

		public Position<Document> SelectionEnd
		{
			get { return _selectionEnd; }
		}
	}
}
