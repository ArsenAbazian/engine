using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class IntShaderParam : ShaderParamInfoBase {
        public IntShaderParam() {
            InitializeComponent();
        }

        public a3dIntShaderParamMapper IntParam {
            get { return base.Param as a3dIntShaderParamMapper; }
            set { base.Param = value; }
        }

        protected virtual void UpdateValueEditor() {
            this.valueEditor.Value = new decimal(IntParam.Value);
        }

        protected override void OnParamChanged() {
            base.OnParamChanged();
            UpdateValueEditor();
        }
        
        protected virtual void UpdateParamValue() {
            IntParam.Value = int.Parse(this.valueEditor.Value.ToString());
        }

        private void valueEditor_ValueChanged(object sender, EventArgs e) {
            UpdateParamValue();
        }

        public NumericUpDown ValueEditor { get { return valueEditor; } }
    }
}
