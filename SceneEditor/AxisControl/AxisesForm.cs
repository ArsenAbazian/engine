using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
	public partial class AxisesForm : FormWithSceneUpdate {
		public AxisesForm() {
			InitializeComponent();
			InitializeEvents();
		}

        IObjectWithBasis objectWithBasis;
        public IObjectWithBasis ObjectWithBasis {
            get { return objectWithBasis; }
            set { 
                objectWithBasis = value;
                OnObjectWithBasisChanged();
            }
        }

        public AxisesControl AxisesControl { get { return axisesControl1; } }

		public event EventHandler GlobalXValueChanged {
			add { AxisesControl.GlobalControl.XControl.ValueChanged += value; }
			remove { AxisesControl.GlobalControl.XControl.ValueChanged -= value; }
		}

		public event EventHandler GlobalYValueChanged {
			add { AxisesControl.GlobalControl.YControl.ValueChanged += value; }
			remove { AxisesControl.GlobalControl.YControl.ValueChanged -= value; }
		}

		public event EventHandler GlobalZValueChanged {
			add { AxisesControl.GlobalControl.ZControl.ValueChanged += value; }
			remove { AxisesControl.GlobalControl.ZControl.ValueChanged -= value; }
		}

		public event EventHandler LocalXValueChanged {
			add { AxisesControl.LocalControl.XControl.ValueChanged += value; }
			remove { AxisesControl.LocalControl.XControl.ValueChanged -= value; }
		}

		public event EventHandler LocalYValueChanged {
			add { AxisesControl.LocalControl.YControl.ValueChanged += value; }
			remove { AxisesControl.LocalControl.YControl.ValueChanged -= value; }
		}

		public event EventHandler LocalZValueChanged {
			add { AxisesControl.LocalControl.ZControl.ValueChanged += value; }
			remove { AxisesControl.LocalControl.ZControl.ValueChanged -= value; }
		}

		public event CoerceEventHandler CoerceGlobalXValue {
			add { AxisesControl.GlobalControl.XControl.CoerceValue += value; }
			remove { AxisesControl.GlobalControl.XControl.CoerceValue -= value; }
		}

		public event CoerceEventHandler CoerceGlobalYValue {
			add { AxisesControl.GlobalControl.YControl.CoerceValue += value; }
			remove { AxisesControl.GlobalControl.YControl.CoerceValue -= value; }
		}

		public event CoerceEventHandler CoerceGlobalZValue {
			add { AxisesControl.GlobalControl.ZControl.CoerceValue += value; }
			remove { AxisesControl.GlobalControl.ZControl.CoerceValue -= value; }
		}

		public event CoerceEventHandler CoerceLocalXValue {
			add { AxisesControl.LocalControl.XControl.CoerceValue += value; }
			remove { AxisesControl.LocalControl.XControl.CoerceValue -= value; }
		}

		public event CoerceEventHandler CoerceLocalYValue {
			add { AxisesControl.LocalControl.YControl.CoerceValue += value; }
			remove { AxisesControl.LocalControl.YControl.CoerceValue -= value; }
		}

		public event CoerceEventHandler CoerceLocalZValue {
			add { AxisesControl.LocalControl.ZControl.CoerceValue += value; }
			remove { AxisesControl.LocalControl.ZControl.CoerceValue -= value; }
		}

		protected virtual void InitializeEvents() {
			GlobalXValueChanged += new EventHandler(OnGlobalXValueChanged);
			GlobalYValueChanged += new EventHandler(OnGlobalYValueChanged);
			GlobalZValueChanged += new EventHandler(OnGlobalZValueChanged);

			LocalXValueChanged += new EventHandler(OnLocalXValueChanged);
			LocalYValueChanged += new EventHandler(OnLocalYValueChanged);
			LocalZValueChanged += new EventHandler(OnLocalZValueChanged);

			CoerceGlobalXValue += new CoerceEventHandler(OnCoerceGlobalXValue);
			CoerceGlobalYValue += new CoerceEventHandler(OnCoerceGlobalYValue);
			CoerceGlobalZValue += new CoerceEventHandler(OnCoerceGlobalZValue);
            
			CoerceLocalXValue += new CoerceEventHandler(OnCoerceLocalXValue);
			CoerceLocalYValue += new CoerceEventHandler(OnCoerceLocalYValue);
			CoerceLocalZValue += new CoerceEventHandler(OnCoerceLocalZValue);
		}

		protected virtual void OnGlobalXValueChanged(object sender, EventArgs e) {
			UpdateMainForm();
		}

		protected virtual void OnGlobalYValueChanged(object sender, EventArgs e) {
			UpdateMainForm();
		}

		protected virtual void OnGlobalZValueChanged(object sender, EventArgs e) {
			UpdateMainForm();
		}

		protected virtual void OnLocalXValueChanged(object sender, EventArgs e) {
			UpdateMainForm();
		}

		protected virtual void OnLocalYValueChanged(object sender, EventArgs e) {
			UpdateMainForm();
		}

		protected virtual void OnLocalZValueChanged(object sender, EventArgs e) {
			UpdateMainForm();
		}

		protected virtual void OnCoerceGlobalXValue(object sender, CoerceEventArgs e) {
			UpdateMainForm();
		}

		protected virtual void OnCoerceGlobalYValue(object sender, CoerceEventArgs e) {
			UpdateMainForm();
		}

		protected virtual void OnCoerceGlobalZValue(object sender, CoerceEventArgs e) {
			UpdateMainForm();
		}

		protected virtual void OnCoerceLocalXValue(object sender, CoerceEventArgs e) {
			UpdateMainForm();
		}

		protected virtual void OnCoerceLocalYValue(object sender, CoerceEventArgs e) {
			UpdateMainForm();
		}

		protected virtual void OnCoerceLocalZValue(object sender, CoerceEventArgs e) {
			UpdateMainForm();
		}
		
		protected virtual void OnObjectWithBasisChanged() {
			UpdateMainForm();
        }
    }
}