using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	public struct HitTestInfo
	{
		[DebuggerHidden]
		public HitTestInfo(ViewArea viewArea)
		{
			_viewArea = viewArea;
			_position = new Position<IView>(-1, -1);
			_virtuelRow = null;
		}

		[DebuggerHidden]
		public HitTestInfo(ViewArea viewArea, Position<IView> position,
			Row virtuelRow)
		{
			_viewArea = viewArea;
			_position = position;
			_virtuelRow = virtuelRow;
		}


		private Row _virtuelRow;

		public Row VirtuelRow
		{
			[DebuggerHidden]
			get { return _virtuelRow; }
		}

		private ViewArea _viewArea;

		internal ViewArea ViewArea
		{
			[DebuggerHidden]
			get { return _viewArea; }
		}

		private Position<IView> _position;

		public Position<IView> Position
		{
			[DebuggerHidden]
			get { return _position; }
		}
	}
}
