using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class EffectListControl : ControlWithSceneUpdate {
        public EffectListControl() {
            InitializeComponent();
        }

        protected override void OnMainFormChanged() {
            effectList.Items.Clear();
            if (Scene == null) return;
            foreach (a3dEffectDesc_Mapper effect in Scene.Effects) {
                effectList.Items.Add(effect);
            }
        }

        private void addNew_Click(object sender, EventArgs e) {
            MainForm.AddEffect();
            MainForm.SolutionControl.SceneView.RefreshSceneContent();
        }

        protected virtual void ChangeEffect() {
            a3dShaderMaterialMapper material = MainForm.SolutionControl.SceneView.SelectedMaterial;
            if (material == null) return;
            MainForm.Scene.ChangeEffect(material, effectList.SelectedItem as a3dEffectDesc_Mapper);
            MainForm.SolutionControl.SceneView.RefreshSceneContent();
            MessageBox.Show(this, "Effect changed", "Effects list", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void effectList_DoubleClick(object sender, EventArgs e) {
            ChangeEffect();
        }

        private void selButton_Click(object sender, EventArgs e) {
            ChangeEffect();
        }
    }
}
