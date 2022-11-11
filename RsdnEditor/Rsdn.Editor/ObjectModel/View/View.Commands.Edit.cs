// Команды - Редактирование документа.

using System;
using System.Drawing;
using System.Diagnostics;
using System.Windows.Forms;
using System.Collections.Generic;
using Rsdn.Editor.ObjectModel.Edit;
using Rsdn.Editor.Keyboard;

namespace Rsdn.Editor
{
	partial class View
	{

		#region Работа с клипбордом.

		public void Paste()
		{
			Document.Replace(Clipboard.GetText(), 
				_selectionStartDocument, _selectionEndDocument);
		}

		public void Copy()
		{
			string text = Document.GetText(_selectionStartDocument, 
				_selectionEndDocument);
			Clipboard.SetDataObject(new DataObject(text));
		}

		public void Cut()
		{
			Copy();
			Delete();
		}

		#endregion

		#region Delete

		[DebuggerHidden]
		[ViewCommand("Удаление выделенного фрагмента или текущего символа")]
		public void Delete()
		{
			Document.Delete(_selectionStartDocument, _selectionEndDocument);
		}
		
		#endregion

		#region DeleteBack

		[DebuggerHidden]
		[ViewCommand("Удаление выделенного фрагмента или предыдущего символа")]
		public void DeleteBack()
		{
			if (_selectionStartDocument == _selectionEndDocument)
				Document.DeleteBack(_selectionStartDocument);
			else
				Delete();
		}
		
		#endregion

		[DebuggerHidden, ViewCommand("Отмена предыдущего действия")]
		public void Undo() { Document.Undo(); }
		[DebuggerHidden, ViewCommand("Повтор только что отменненого действия")]
		public void Redo() { Document.Redo(); }
	}
}
