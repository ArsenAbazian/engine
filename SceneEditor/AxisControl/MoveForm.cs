using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
	public partial class MoveForm : sceneEditor.AxisesForm {
		public MoveForm() {
			InitializeComponent();
		}

		protected override void OnObjectWithBasisChanged() {
			base.OnObjectWithBasisChanged();
			
			if(ObjectWithBasis == null) return;

			AxisesControl.GlobalControl.XValue = ObjectWithBasis.Basis.A30;
			AxisesControl.GlobalControl.YValue = ObjectWithBasis.Basis.A31;
			AxisesControl.GlobalControl.ZValue = ObjectWithBasis.Basis.A32;

			AxisesControl.LocalControl.XValue = ObjectWithBasis.Basis.A30;
			AxisesControl.LocalControl.YValue = ObjectWithBasis.Basis.A31;
			AxisesControl.LocalControl.ZValue = ObjectWithBasis.Basis.A32;
		}

		public float GlobalXValue { get { return AxisesControl.GlobalControl.XValue; } set { AxisesControl.GlobalControl.XValue = value; } }
		public float GlobalYValue { get { return AxisesControl.GlobalControl.YValue; } set { AxisesControl.GlobalControl.YValue = value; } }
		public float GlobalZValue { get { return AxisesControl.GlobalControl.ZValue; } set { AxisesControl.GlobalControl.ZValue = value; } }

		public float LocalXValue { get { return AxisesControl.LocalControl.XValue; } set { AxisesControl.LocalControl.XValue = value; } }
		public float LocalYValue { get { return AxisesControl.LocalControl.YValue; } set { AxisesControl.LocalControl.YValue = value; } }
		public float LocalZValue { get { return AxisesControl.LocalControl.ZValue; } set { AxisesControl.LocalControl.ZValue = value; } }

		protected override void OnGlobalXValueChanged(object sender, EventArgs e) {
			Matrix4_Mapper basis = ObjectWithBasis.Basis;
			basis.A30 = GlobalXValue;
			ObjectWithBasis.Basis = basis;
			base.OnGlobalXValueChanged(sender, e);
		}
		protected override void OnGlobalYValueChanged(object sender, EventArgs e) {
			Matrix4_Mapper basis = ObjectWithBasis.Basis;
			basis.A31 = GlobalYValue;
			ObjectWithBasis.Basis = basis;
			base.OnGlobalYValueChanged(sender, e);
		}
		protected override void OnGlobalZValueChanged(object sender, EventArgs e) {
			Matrix4_Mapper basis = ObjectWithBasis.Basis;
			basis.A32 = GlobalZValue;
			ObjectWithBasis.Basis = basis;
			base.OnGlobalYValueChanged(sender, e);
		}
	}
}

