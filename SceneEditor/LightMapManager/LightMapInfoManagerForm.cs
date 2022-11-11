using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor.LightMapManager {
    public partial class LightMapInfoManagerForm : FormWithSceneUpdate {
        public LightMapInfoManagerForm() {
            InitializeComponent();
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            lightMapManagerControl1.MainForm = MainForm;
        }

        private void okButton_Click(object sender, EventArgs e) {
            this.Close();
        }
    }
}