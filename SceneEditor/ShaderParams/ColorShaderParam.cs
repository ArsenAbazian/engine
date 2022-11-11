using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class ColorShaderParam : ShaderParamInfoBase {
        public ColorShaderParam() {
            InitializeComponent();
        }

        public a3dFloatArrayShaderParamMapper FloatArrayParam {
            get { return base.Param as a3dFloatArrayShaderParamMapper; }
            set { base.Param = value; }
        }

        public a3dMatrixShaderParam_Mapper MatrixParam {
            get { return base.Param as a3dMatrixShaderParam_Mapper; }
            set { base.Param = value; }
        }

        protected virtual void SetFloatArrayParamValues() {
            
            int a = 255, r, g, b, index = 0;
            if (FloatArrayParam.Count == 4) {
                a = (int)(FloatArrayParam.get_Value(0) * 255);
                index = 1;
            }
            r = (int)(FloatArrayParam.get_Value(index) * 255);
            g = (int)(FloatArrayParam.get_Value(index + 1) * 255);
            b = (int)(FloatArrayParam.get_Value(index + 2) * 255);

            colorPicker1.Color = Color.FromArgb(a, r, g, b);
        }

        protected virtual void SetMatrixParamValues() {

            int a = 255, r, g, b, index = 0;
            if (MatrixParam.Col == 4) {
                a = (int)(MatrixParam.Value.getValue(0,0) * 255);
                index = 1;
            }
            r = (int)(MatrixParam.Value.getValue(0, index) * 255);
            g = (int)(MatrixParam.Value.getValue(0, index + 1) * 255);
            b = (int)(MatrixParam.Value.getValue(0, index + 2) * 255);

            colorPicker1.Color = Color.FromArgb(a, r, g, b);
        }

        protected override void OnParamChanged() {
            base.OnParamChanged();

            if (FloatArrayParam != null)
                SetFloatArrayParamValues();
            else if (MatrixParam != null)
                SetMatrixParamValues();
        }

        protected virtual void UpdateFloatArrayParamValues() {
            
            int index = 0;
            if (FloatArrayParam.Count == 4) {
                FloatArrayParam.set_Value(0, colorPicker1.Color.A / 255.0f);
                index = 1;
            }
            FloatArrayParam.set_Value(index, colorPicker1.Color.R / 255.0f);
            FloatArrayParam.set_Value(index + 1, colorPicker1.Color.G / 255.0f);
            FloatArrayParam.set_Value(index + 2, colorPicker1.Color.B / 255.0f);
        }

        protected virtual void UpdateMatrixParamValues() {
            int index = 0;
            if (MatrixParam.Col == 4) {
                MatrixParam.Value.setValue(0, 0, colorPicker1.Color.A / 255.0f);
                index = 1;
            }
            MatrixParam.SetValue(0, index, colorPicker1.Color.R / 255.0f);
            MatrixParam.SetValue(0, index + 1, colorPicker1.Color.G / 255.0f);
            MatrixParam.SetValue(0, index + 2, colorPicker1.Color.B / 255.0f);
            UpdateMainForm();
        }

        private void colorPicker1_ColorChanged(object sender, EventArgs e) {

            if (FloatArrayParam != null)
                UpdateFloatArrayParamValues();
            else if (MatrixParam != null)
                UpdateMatrixParamValues();
        }
    }
}
