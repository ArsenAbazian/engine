using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class NumericShaderParam : ShaderParamInfoBase {
        public NumericShaderParam() {
            InitializeComponent();
        }

        bool allowSlider = true;
        public bool AllowSlider {
            get { return allowSlider; }
            set {
                if (AllowSlider == value) return;
                allowSlider = value;
                OnAllowSliderChanged();
            }
        }

        decimal minValue = new decimal(), 
                maxValue = new decimal();
        public decimal MinValue {
            get { return minValue; }
            set {
                if (MinValue == value) return;
                minValue = value;
                OnMinValueChanged();
            }
        }

        public decimal MaxValue {
            get { return maxValue; }
            set {
                if (MaxValue == value) return;
                maxValue = value;
                OnMaxValueChanged();
            }
        }

        protected override void OnParamChanged() {
            base.OnParamChanged();
            AllowSlider = Param.WidgetType == a3dShaderParamWidgetTypeMapper.SpwtSlider;
        }

        protected virtual void OnAllowSliderChanged() {
            valueSlider.Visible = AllowSlider;
            minLabel.Visible = AllowSlider;
            maxLabel.Visible = AllowSlider;

            if (AllowSlider) {
                Size = new Size(203, 84);
            }
            else {
                Size = new Size(203, 42);
            }
        }

        protected virtual string GetMinValueText() { return "0"; }
        protected virtual string GetMaxValueText() { return "1"; }
        protected virtual void OnMinValueChanged() {
            minLabel.Text = GetMinValueText();
            valueText.Minimum = MinValue;
        }
        protected virtual void OnMaxValueChanged() {
            maxLabel.Text = GetMaxValueText();
            valueText.Maximum = MaxValue;
        }

        protected virtual void OnValueTextValueChanged(object sender, EventArgs e) {
            
        }

        protected virtual void OnValueSliderValueChanged(object sender, EventArgs e) {

        }
    }
}
