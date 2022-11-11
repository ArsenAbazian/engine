using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace Rsdn.Editor.ObjectModel.Edit
{

	public abstract class ModifyTextCommand : Command
	{
		#region Конструкторы

		//[DebuggerHidden]
		public ModifyTextCommand(
			Document document, 
			Position<Document> startPosition,
			Range<Document> selectionBefore,
			Range<Document> selectionAfter)
			: base(document, selectionBefore, selectionAfter, true)
		{
			_startPosition = startPosition;
		}

		//[DebuggerHidden]
		public ModifyTextCommand(
			Document document, 
			Position<Document> startPosition)
			: base(document, Range<Document>.Invalid, Range<Document>.Invalid, false)
		{
			_startPosition = startPosition;
		}

		[DebuggerHidden]
		public ModifyTextCommand(
			Document document,
			Position<Document> startPosition, 
			bool autoSelect)
			: base(document, Range<Document>.Invalid, Range<Document>.Invalid, false)
		{
			_startPosition = startPosition;
			_autoSelect = autoSelect;
		}
		
		#endregion

		#region AutoSelect property

		private bool _autoSelect;

		public bool AutoSelect
		{
			get { return _autoSelect; }
		}
		
		#endregion

		#region StartPosition property

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		Position<Document> _startPosition;

		public Position<Document> StartPosition
		{
			[DebuggerHidden]
			get { return _startPosition; }
		}

		#endregion


	}
}
