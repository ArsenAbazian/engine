using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class ShaderParamsForm : FormWithSceneUpdate {
        public ShaderParamsForm() {
            InitializeComponent();
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            shaderParamsControl1.MainForm = MainForm;
        }

        public ShaderParamsControl ParamsControl { get { return shaderParamsControl1; } }

        private void okButton_Click(object sender, EventArgs e) {
            Close();
        }
        
        a3dModelDesc_Mapper modelDesc = null;
        public a3dModelDesc_Mapper ModelDesc {
            get { return modelDesc; }
            set { modelDesc = value; }
        }

        private void ShaderParamsForm_FormClosed(object sender, FormClosedEventArgs e) {
            if (ModelDesc != null) {
                if (MessageBox.Show(this, "It seems that you tried to modify model material. Resave model?", "Resave Model", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No)
                    return;
                if (!ModelDesc.SaveModel()) {
                    MessageBox.Show(this, "Error: model not saved", "Resave Model", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else {
                    MessageBox.Show(this, "Model successfully saved", "Resave Model", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }
    }
}