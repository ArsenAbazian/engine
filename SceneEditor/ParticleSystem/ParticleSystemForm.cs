using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using sceneEditor.Configuration;

namespace sceneEditor.ParticleSystem {
    public partial class ParticleSystemForm : sceneEditor.FormWithSceneUpdate {
        public ParticleSystemForm() {
            InitializeComponent();
        }

        public ParticleSystemControl ParticleInfo { get { return particleSystemControl1; } }

        private void cancelButton_Click(object sender, EventArgs e) {

        }

        private void okButton_Click(object sender, EventArgs e) {

        }
        protected override void OnClosed(EventArgs e) {
            base.OnClosed(e);
            ParticleInfo.SaveConfig();
        }
    }
}

