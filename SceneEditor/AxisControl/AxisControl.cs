using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
	public partial class AxisControl : UserControl {
		float d;
		
		public AxisControl() {
			InitializeComponent();
			deltaCombo.SelectedIndex = 3;
		}

		public string GroupText { get { return groupBox1.Text; } set { groupBox1.Text = value; } }

		public TrackValueControl XControl { get { return xControl; } }
		public TrackValueControl YControl { get { return yControl; } }
		public TrackValueControl ZControl { get { return zControl; } }

		public float XValue {
			get { return XControl.Value; }
			set { XControl.Value = value; }
	}

		public float YValue {
			get { return YControl.Value; }
			set { YControl.Value = value; }
		}
        
		public float ZValue {
			get { return ZControl.Value; }
			set { ZControl.Value = value; }
		}

		public float Delta { 
			get { return d; } 
			set { 
				d = value;
				OnDeltaChanged();
		}
    }

		public event EventHandler XValueChanged {
			add { XControl.ValueChanged += value; }
			remove { XControl.ValueChanged -= value; }
		}
		
		public event EventHandler YValueChanged {
			add { YControl.ValueChanged += value; }
			remove { YControl.ValueChanged -= value; }
		}

		public event EventHandler ZValueChanged {
			add { ZControl.ValueChanged += value; }
			remove { ZControl.ValueChanged -= value; }
			}

		public event CoerceEventHandler CoerceXValue {
			add { XControl.CoerceValue += value; }
			remove { XControl.CoerceValue -= value; }
		}

		public event CoerceEventHandler CoerceYValue {
			add { YControl.CoerceValue += value; }
			remove { YControl.CoerceValue -= value; }
		}

		public event CoerceEventHandler CoerceZValue {
			add { ZControl.CoerceValue += value; }
			remove { ZControl.CoerceValue -= value; }
		}

		protected virtual void OnDeltaChanged() {
			XControl.Delta = Delta;
			YControl.Delta = Delta;
			ZControl.Delta = Delta;
		}

		private void deltaCombo_SelectedIndexChanged(object sender, EventArgs e) {
			Delta = float.Parse(deltaCombo.SelectedItem.ToString());
		}
	}
}
