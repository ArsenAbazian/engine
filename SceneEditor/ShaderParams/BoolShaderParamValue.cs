using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class BoolShaderParam : ShaderParamInfoBase {
        public BoolShaderParam() {
            InitializeComponent();
        }

        public new a3dBoolShaderParamMapper Param { 
            get { return base.Param as a3dBoolShaderParamMapper; }
            set { base.Param = value; }
        }

        protected override void OnParamChanged() {
            base.OnParamChanged();
            value.Checked = Param.Value != 0;
        }

        private void value_CheckedChanged(object sender, EventArgs e) {
            Param.Value = value.Checked? 1: 0;
            UpdateMainForm();
        }
    }
}
