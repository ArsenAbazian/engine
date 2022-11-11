using System;
using System.Collections.Generic;
using System.Text;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	public class SelectionChangedEventArgs : EventArgs
	{
		#region Констрктор
		public SelectionChangedEventArgs(
			Position<Document> startPosition,
			Position<Document> endPosition,
			IView view)
		{
			_startPosition = startPosition;
			_endPosition = endPosition;
			_view = view;
		}
		
		#endregion

		#region View property

		private IView _view;

		public IView View
		{
			get { return _view; }
		}
		
		#endregion

		#region StartPosition property

		private Position<Document> _startPosition;

		public Position<Document> StartPosition
		{
			get { return _startPosition; }
		}
		
		#endregion

		#region EndPosition property
		private Position<Document> _endPosition;

		public Position<Document> EndPosition
		{
			get { return _endPosition; }
		}
		
		#endregion
		
		#region Text property

		public string Text
		{
			get
			{
				throw new NotImplementedException();
			}
		}
 
		#endregion
	}
}
