using System;
using System.Collections.Generic;
using System.Text;
using Rsdn.Editor.ObjectModel;
using System.Diagnostics.CodeAnalysis;

namespace Rsdn.Editor
{
	public interface IFormatter
	{
		RowCollection DocumentToViewRows(IView view, DocumentRowCollection rows);
		void StartParse(DocumentRowCollection rows, int startState);
		void PrepareRows(DocumentRowCollection rows);
		int PrepareRows(DocumentRowCollection rows, int start, int count);
		Position<Document> GetPreviousWordPosition(
			DocumentRowCollection rows, Position<Document> start);
		Position<Document> GetNextWordPosition(
			DocumentRowCollection rows, Position<Document> start);
		Range<Document> GetWordRange(DocumentRowCollection rows, Position<Document> start);
	}
}
