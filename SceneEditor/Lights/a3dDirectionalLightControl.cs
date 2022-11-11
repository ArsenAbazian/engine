using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.Lights {
    public partial class a3dDirectionalLightControl : a3dLightBaseControl {
        public a3dDirectionalLightControl() {
            InitializeComponent();
        }

        public a3dDirectionalLight_Mapper DirLight {
            get { return base.Light as a3dDirectionalLight_Mapper; }
            set { base.Light = value; }
        }

        public Vector3_Mapper Direction {
            get { return vectorControl1.VectorValue3; }
            set { vectorControl1.VectorValue3 = value; }
        }
    }
}
