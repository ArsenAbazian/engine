// Отладочный код.
#define CHECK_FOR_NECESSARY_REPAINT

using System;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using System.Text;
using System.Reflection;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	partial class View
	{
		private void Test1()
		{
			//TextUpdated();
			//EnsureVisible(ToView(CaretPosition).Line);
		}
		protected void PaintTest()
		{
			OnPaint(new PaintEventArgs(
				Graphics.FromHwnd(Handle),
				ClientRectangle));
		}

		#region Измерение производительности

		PerfCounter _timerAll = new PerfCounter();
		PerfCounter _timerPaintRow = new PerfCounter();
		PerfCounter _timerMakeStyledRanges = new PerfCounter();
		PerfCounter _timerExpandTabs = new PerfCounter();
		PerfCounter _timerPaintStyledRange = new PerfCounter();
		PerfCounter _timerMeasureWidth = new PerfCounter();
		PerfCounter _timerDrawText = new PerfCounter();
		float _paintRowTime;
		float _makeStyledRangesTime;
		float _expandTabsTime;
		float _paintStyledRangeTime;
		float _measureWidthTime;
		float _drawTextTime;

		[Conditional("PERF_TEST")]
		void AllTimeStart() { _timerAll.Start(); }
		[Conditional("PERF_TEST")]
		void DrawRowTimeStart() { _timerPaintRow.Start(); }
		[Conditional("PERF_TEST")]
		void DrawRowTimeFinish() { _paintRowTime += _timerPaintRow.Finish(); }
		[Conditional("PERF_TEST")]
		void MakeStyledRangesTimeStart() { _timerMakeStyledRanges.Start(); }
		[Conditional("PERF_TEST")]
		void MakeStyledRangesTimeFinish() { _makeStyledRangesTime += _timerMakeStyledRanges.Finish(); }
		[Conditional("PERF_TEST")]
		void ExpandTabsTimeStart() { _timerExpandTabs.Start(); }
		[Conditional("PERF_TEST")]
		void ExpandTabsTimeFinish() { _expandTabsTime += _timerExpandTabs.Finish(); }
		[Conditional("PERF_TEST")]
		void PaintStyledRangeTimeStart() { _timerPaintStyledRange.Start(); }
		[Conditional("PERF_TEST")]
		void PaintStyledRangeTimeFinish() { _paintStyledRangeTime += _timerPaintStyledRange.Finish(); }
		[Conditional("PERF_TEST")]
		void MeasureWidthTimeStart() { _timerMeasureWidth.Start(); }
		[Conditional("PERF_TEST")]
		void MeasureWidthTimeFinish() { _measureWidthTime += _timerMeasureWidth.Finish(); }
		[Conditional("PERF_TEST")]
		void DrawTextTimeStart() { _timerDrawText.Start(); }
		[Conditional("PERF_TEST")]
		void DrawTextTimeFinish() { _drawTextTime += _timerDrawText.Finish(); }

		[Conditional("PERF_TEST")]
		private void DisplayPerfInfo(int count)
		{
			Form form = this.FindForm();

			if (form == null)
				return;

			int intPtrSize =
				System.Runtime.InteropServices.Marshal.SizeOf(typeof(IntPtr));

			if (count > 0)
				form.Text = string.Format(
					"Lines {0}; ALL {1:0.00000000}; "
					+ "DrawRow {2:0.00000000}; "
					+ "MakeRanges {3:0.00000000}; "
					+ "ExpandTabs {4:0.00000000}; "
					+ "PaintRange {5:0.00000000}; "
					+ "MeasureWidth {6:0.00000000}; "
					+ "DrawText {7:0.00000000}; ",
					count, _timerAll.Finish(), _paintRowTime, _makeStyledRangesTime,
					_expandTabsTime, _paintStyledRangeTime, _measureWidthTime,
					_drawTextTime);
			//IntPtr = {5}; 
			else
				form.Text = "Draw line 0.";
		}

		#endregion

		static string ToStr(object obj)
		{
			Type type = obj.GetType();
			StringBuilder sb = new StringBuilder();

			//sb.Append("\r\n");

			foreach (PropertyInfo prop in type.GetProperties())
			{
				sb.Append("	" + prop.Name + " = "
					+ prop.GetValue(obj, new object[0]) + ";");
			}

			return sb.ToString();
		}

#if CHECK_FOR_NECESSARY_REPAINT
		private int _oldFirstVisibleRow = -1;
		private int _oldFirstVisibleCol = -1;
		private int _oldLastVisibleRow = -1;
		Range<Document> _oldSelection;
		static int _unnecessaryRepaintCount = 0;
		int _oldViewHeight;

		private bool IsNecessaryRepaint()
		{
			//if (DoublleBuffering)
			//	return true;

			if (_oldFirstVisibleRow != FirstVisibleRow)
				return true;

			if (_oldFirstVisibleCol != FirstVisibleCol)
				return true;

			if (_oldSelection != Selection.Range)
				return true;

			if (_oldLastVisibleRow != FirstVisibleRow + CalcTopPageRowCount())
				return true;

			if (_oldViewHeight != Height)
				return true;

			return false;
		}

		private void CheckForNecessaryRepaint()
		{
			try
			{
				if (_oldFirstVisibleRow != FirstVisibleRow)
					return;

				if (_oldFirstVisibleCol != FirstVisibleCol)
					return;

				if (_oldSelection != Selection.Range)
					return;

				if (_oldLastVisibleRow != FirstVisibleRow + CalcTopPageRowCount())
					return;

				if (_oldViewHeight != Height)
					return;

				Debug.WriteLine("Unnecessary repaint " + ++_unnecessaryRepaintCount);

				return;
			}
			finally
			{
				_oldFirstVisibleRow = FirstVisibleRow;
				_oldFirstVisibleCol = FirstVisibleCol;
				_oldSelection = Selection.Range;
				_oldLastVisibleRow = FirstVisibleRow + CalcTopPageRowCount();
				_oldViewHeight = Height;
			}
		}
#else
		[Conditional("DE186812644A4846AC70DD6C2F6E28D4")]
		private void CheckForNecessaryRepaint() {  }
#endif
		#region Trace switches

		private static TraceSwitch TraceDragDrop
		{
			get
			{
				return _traceDragDrop ?? (_traceDragDrop = new TraceSwitch("View.DragDrop", "View drag'n'drop trace"));
			}
		}

		private static TraceSwitch TraceKeyboard
		{
			get
			{
				return _traceKeyboard ?? (_traceKeyboard = new TraceSwitch("View.Keyboard", "View keyboard trace"));
			}
		}

		private static TraceSwitch TraceMouse
		{
			get
			{
				return _traceMouse ?? (_traceMouse = new TraceSwitch("View.Mouse", "View mouse trace"));
			}
		}

		private static TraceSwitch _traceDragDrop;
		private static TraceSwitch _traceKeyboard;
		private static TraceSwitch _traceMouse;

		#endregion
	}
}
