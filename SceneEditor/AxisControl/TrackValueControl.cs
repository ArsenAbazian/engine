using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
	public partial class TrackValueControl : UserControl {
		private static readonly object valueChanged = new object();
		private static readonly object coerceValue = new object();

		float val, delta;
		float initialValue;

		public TrackValueControl() {
			InitializeComponent();
		}

		public string ValueName { get { return label1.Text; } set { label1.Text = value; } }
		public float Value { 
			get { return val; } 
			set {
				if(val == value) return;
				val = value;
				OnValueChanged();
			} 
		}
		public float Delta {
			get { return delta; }
			set { delta = value; }
		}
		public event EventHandler ValueChanged {
			add { Events.AddHandler(valueChanged, value); }
			remove { Events.RemoveHandler(valueChanged, value); }
		}
		protected virtual void RaiseValueChanged() {
			EventHandler handler = Events[valueChanged] as EventHandler;
			if(handler != null) handler(this, EventArgs.Empty);
		}
		public event CoerceEventHandler CoerceValue {
			add { Events.AddHandler(coerceValue, value); }
			remove { Events.RemoveHandler(coerceValue, value); }
		}
		protected virtual void RaiseCoerceValue(CoerceEventArgs e) {
			CoerceEventHandler handler = Events[coerceValue] as CoerceEventHandler;
			if(handler != null) handler(this, e);
		}
		protected virtual void OnValueChanged() {
			this.valueText.Text = Value.ToString();
		}
		protected float InitialValue { get { return initialValue; } set { initialValue = value; } }
		protected int MiddlePos { get { return valueTrack.Minimum + (valueTrack.Maximum - valueTrack.Minimum) >> 1; } }
		private void valueTrack_MouseDown(object sender, MouseEventArgs e) {
			InitialValue = Value;
		}
		protected virtual void UpdateValue() {
			CoerceEventArgs e = new CoerceEventArgs(InitialValue + (valueTrack.Value - MiddlePos) * Delta);
			RaiseCoerceValue(e);
			this.val = e.Value;
			OnValueChanged();
			RaiseValueChanged();
		}
		private void valueTrack_ValueChanged(object sender, EventArgs e) {
			UpdateValue();
		}

		private void valueTrack_MouseUp(object sender, MouseEventArgs e) {
			InitialValue = Value;
			valueTrack.Value = MiddlePos;
		}

        private void valueText_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Escape)
                valueText.Text = Value.ToString();
            else if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Tab) {
                this.val = float.Parse(valueText.Text);
                RaiseValueChanged();
            }
        }
	}

	public class CoerceEventArgs : EventArgs {
		float val;
		public CoerceEventArgs(float val) { this.val = val; }

		public float Value { get { return val; } set { val = value; } }
	}
	public delegate void CoerceEventHandler(object sender, CoerceEventArgs e);
}
