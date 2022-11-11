using System;
using System.Collections.Generic;
using System.Text;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	public interface ISelection
	{
		Position<Document> Start { get; set; }
		Position<Document> End { get; set; }
		Range<Document> Range { get; set; }
		string Text { get; set; }

		void SetSelection(Position<Document> start, Position<Document> end);
		void SetSelection(Position<IView> start, Position<IView> end);
	}
}
