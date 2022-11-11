using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor.Lights {
    public partial class a3dLightForm : FormWithSceneUpdate {
        public a3dLightForm() {
            InitializeComponent();
        }

        public a3dLightControl LightControl { get { return a3dLightControl1; } }

        private void a3dLightControl1_SizeChanged(object sender, EventArgs e) {
            Height = Height - ClientRectangle.Height + a3dLightControl1.Height + 3 + ClientRectangle.Height - okButton.Top;
        }
    }
}