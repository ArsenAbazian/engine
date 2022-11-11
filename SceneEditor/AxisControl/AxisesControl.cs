using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using sceneEditor;

namespace sceneEditor {
	public partial class AxisesControl : UserControl {
		public AxisesControl() {
			InitializeComponent();
		}

		public AxisControl GlobalControl { get { return globalAxisControl; } }
		public AxisControl LocalControl { get { return localAxisControl; } }

		[Category("AxisEvents")]
		public event EventHandler GlobalXValueChanged {
			add { GlobalControl.XControl.ValueChanged += value; }
			remove { GlobalControl.XControl.ValueChanged -= value; }
		}

		[Category("AxisEvents")]
		public event EventHandler GlobalYValueChanged {
			add { GlobalControl.YControl.ValueChanged += value; }
			remove { GlobalControl.YControl.ValueChanged -= value; }
		}

		[Category("AxisEvents")]
		public event EventHandler GlobalZValueChanged {
			add { GlobalControl.ZControl.ValueChanged += value; }
			remove { GlobalControl.ZControl.ValueChanged -= value; }
		}

		[Category("AxisEvents")]
		public event EventHandler LocalXValueChanged {
			add { LocalControl.XControl.ValueChanged += value; }
			remove { LocalControl.XControl.ValueChanged -= value; }
		}

		[Category("AxisEvents")]
		public event EventHandler LocalYValueChanged {
			add { LocalControl.YControl.ValueChanged += value; }
			remove { LocalControl.YControl.ValueChanged -= value; }
		}

		[Category("AxisEvents")]
		public event EventHandler LocalZValueChanged {
			add { LocalControl.ZControl.ValueChanged += value; }
			remove { LocalControl.ZControl.ValueChanged -= value; }
		}

		[Category("AxisEvents")]
		public event CoerceEventHandler CoerceGlobalXValue {
			add { GlobalControl.XControl.CoerceValue += value; }
			remove { GlobalControl.XControl.CoerceValue -= value; }
		}

		[Category("AxisEvents")]
		public event CoerceEventHandler CoerceGlobalYValue {
			add { GlobalControl.YControl.CoerceValue += value; }
			remove { GlobalControl.YControl.CoerceValue -= value; }
		}

		[Category("AxisEvents")]
		public event CoerceEventHandler CoerceGlobalZValue {
			add { GlobalControl.ZControl.CoerceValue += value; }
			remove { GlobalControl.ZControl.CoerceValue -= value; }
		}

		[Category("AxisEvents")]
		public event CoerceEventHandler CoerceLocalXValue {
			add { LocalControl.XControl.CoerceValue += value; }
			remove { LocalControl.XControl.CoerceValue -= value; }
		}

		[Category("AxisEvents")]
		public event CoerceEventHandler CoerceLocalYValue {
			add { LocalControl.YControl.CoerceValue += value; }
			remove { LocalControl.YControl.CoerceValue -= value; }
		}

		[Category("AxisEvents")]
		public event CoerceEventHandler CoerceLocalZValue {
			add { LocalControl.ZControl.CoerceValue += value; }
			remove { LocalControl.ZControl.CoerceValue -= value; }
		}
    }
}
