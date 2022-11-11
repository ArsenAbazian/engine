using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class FloatShaderParam : NumericShaderParam {
        public FloatShaderParam() {
            InitializeComponent();
        }

        public new a3dFloatShaderParamMapper Param {
            get { return base.Param as a3dFloatShaderParamMapper; }
            set { base.Param = value; }
        }

        protected virtual void UpdateValueEditor() {
            UpdateValueText();
            UpdateValueSlider();
        }

        protected override string GetMinValueText() { return MinValue.ToString(); }
        protected override string GetMaxValueText() { return MaxValue.ToString(); }

        protected override void OnParamChanged() {
            base.OnParamChanged();
            if (Param.MaxValue - Param.MinValue <= 0.0f)
                Param.MaxValue = Math.Max(Param.MinValue + 1, Param.Value);
            if (Param.Step == 0.0f)
                Param.Step = 0.001f;
            MinValue = new decimal(Param.MinValue);
            MaxValue = new decimal(Param.MaxValue);
            valueText.Increment = new decimal(Param.Step);
            UpdateValueEditor();
        }

        protected virtual void UpdateValueSlider() {
            if (AllowSlider) {
                valueSlider.Value = (int)((Param.Value - Param.MinValue) / (Param.MaxValue - Param.MinValue) * 10000);
            }
        }

        protected virtual void UpdateValueText() {
            valueText.Value = new decimal(Param.Value);
        }

        bool suppressValueText = false;
        bool suppressValueSlider = false;
        protected override void OnValueTextValueChanged(object sender, EventArgs e) {
            if (suppressValueText) return;
            suppressValueText = true;
            Param.Value = float.Parse(valueText.Value.ToString());
            UpdateValueSlider();
            suppressValueText = false;
            MainForm.ViewControl.Invalidate();
        }

        protected override void OnValueSliderValueChanged(object sender, EventArgs e) {
            if (suppressValueSlider) return;
            suppressValueSlider = true;
            Param.Value = Param.MinValue + (Param.MaxValue - Param.MinValue) * valueSlider.Value / 10000.0f;
            UpdateValueText();
            suppressValueSlider = false;
            MainForm.ViewControl.Invalidate();
        }
    }
}
