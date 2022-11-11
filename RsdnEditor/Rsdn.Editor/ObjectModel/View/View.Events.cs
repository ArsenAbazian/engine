// События View

using Rsdn.Editor;
using System;

namespace Rsdn.Editor
{
	partial class View
	{
		#region SelectionChanged

		public event EventHandler<SelectionChangedEventArgs> SelectionChanged;

		protected void OnSelectionChanged(SelectionChangedEventArgs e)
		{
			if (SelectionChanged != null)
				SelectionChanged(this, e);
		}
 
		#endregion

		#region SelectionChanging

		public event EventHandler<SelectionChangingEventArgs> SelectionChanging;

		protected void OnSelectionChanging(SelectionChangingEventArgs e)
		{
			if (SelectionChanging != null)
				SelectionChanging(this, e);
		}
 
		#endregion
	}
}
