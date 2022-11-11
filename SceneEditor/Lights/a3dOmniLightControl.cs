using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.Lights {
    public partial class a3dOmniLightControl : a3dLightBaseControl {
        public a3dOmniLightControl() {
            InitializeComponent();
        }

        public a3dOmniLight_Mapper OmniLight {
            get { return base.Light as a3dOmniLight_Mapper; }
            set { base.Light = value; }
        }

        protected override void OnLightChanged() {
            base.OnLightChanged();
            if(OmniLight == null) return;
            FarAttenStart = OmniLight.FarAttenStart;
            FarAttenEnd = OmniLight.FarAttenEnd;
            vectorControl1.V0 = OmniLight.Position.X;
            vectorControl1.V1 = OmniLight.Position.Y;
            vectorControl1.V2 = OmniLight.Position.Z;
        }

        public float FarAttenStart {
            get { return float.Parse(farAttenStart.Value.ToString()); }
            set { farAttenStart.Value = new decimal(value); }
        }

        public float FarAttenEnd {
            get { return float.Parse(farAttenEnd.Value.ToString()); }
            set { farAttenEnd.Value = new decimal(value); }
        }

        public Vector3_Mapper Position {
            get { return vectorControl1.VectorValue3; }
            set { vectorControl1.VectorValue3 = value; }
        }
    }
}
