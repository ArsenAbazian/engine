using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;
using sceneEditor.Configuration;

namespace sceneEditor.ParticleSystem {
    public partial class ParticleSystemControl : sceneEditor.ControlWithSceneUpdate {
        public ParticleSystemControl() {
            InitializeComponent();
        }

        public string SelectedParticleTypeName { 
            get {
                if (particleSystemList.SelectedItem == null) return string.Empty;
                return particleSystemList.SelectedItem.ToString(); 
            } 
        }
        public string ParticleName { get { return nameTextBox.Text; } }
        public int ParticleCount { 
            get {
                try { return int.Parse(particleCountTextBox.Text); }
                catch (Exception) { return 0; }
            } 
        }
        public int MaxParticleCount { 
            get {
                try { return int.Parse(mpCountTextBox.Text); }
                catch (Exception) { return 0; }
            } 
        }

        private void particleSystemList_SelectedIndexChanged(object sender, EventArgs e) {
            if (nameTextBox.Text == "")
                nameTextBox.Text = SelectedParticleTypeName.ToLower();
        }

        public virtual void SaveConfig() {
            Configurator.Default.SetValue("ParticleSystemControl", "ParticleCount", ParticleCount);
            Configurator.Default.SetValue("ParticleSystemControl", "MaxParticleCount", MaxParticleCount);
        }

        protected override void OnVisibleChanged(EventArgs e) {
            base.OnVisibleChanged(e);
            if (Visible) {
                particleCountTextBox.Text = Configurator.Default.Options["ParticleSystemControl"].GetValue("ParticleCount", ParticleCount).ToString();
                mpCountTextBox.Text = Configurator.Default.Options["ParticleSystemControl"].GetValue("MaxParticleCount", MaxParticleCount).ToString();
            }
        }
    }
}

