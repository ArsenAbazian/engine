using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class ShaderParamInfoBase : ControlWithSceneUpdate {
        public ShaderParamInfoBase() {
            InitializeComponent();
        }
    
        public string ParamName {
            get { return paramBox.Text; }
            set { paramBox.Text = value; }
        } 

        public bool ParamEnabled {
            get { return paramEnabled.Checked; }
            set { paramEnabled.Checked = value; }
        }

        a3dShaderParamBaseMapper param;
        public virtual a3dShaderParamBaseMapper Param {
            get { return param; }
            set {
                if (Param == value) return;
                param = value;
                OnParamChanged();
                Size = CalcBestSize();
            }
        }

        protected virtual void OnParamChanged() { 
            if(Param == null) return;
            ParamName = Param.Name;
            ParamEnabled = Param.Used;
        }

        private void paramEnabled_CheckedChanged(object sender, EventArgs e) {
            if (Param.Used == ParamEnabled) return;
            Param.Used = ParamEnabled;
        }
        
        protected virtual Size CalcBestSize() {
            Size size = Size.Empty;
            foreach(Control ctrl in Controls) {
                if (!ctrl.Visible) continue;
                if (ctrl == this.paramBox) { 
                    foreach(Control ctrl2 in this.paramBox.Controls) {
                        if (!ctrl2.Visible) continue;
                        size.Height = Math.Max(ctrl2.Bottom, size.Height);
                        size.Width = Math.Max(ctrl2.Right, size.Width);
                    }
                    continue;
                }
                size.Height = Math.Max(ctrl.Bottom, size.Height);
                size.Width = Math.Max(ctrl.Right, size.Width);
            }
            size.Height += this.paramBox.Height - this.paramBox.DisplayRectangle.Bottom;
            size.Width += this.paramBox.Width - this.paramBox.DisplayRectangle.Right;
            return size;
        }
    }

    public class ArrayItemInfo {
        int arrayDimension;
        int itemIndex1;
        int itemIndex2;
        int itemIndex3;
        object editValue;

        public ArrayItemInfo(int arrayDimension, int itemIndex1, int itemIndex2, int itemIndex3, object editValue) {
            this.arrayDimension = arrayDimension;
            this.itemIndex1 = itemIndex1;
            this.itemIndex2 = itemIndex2;
            this.itemIndex3 = itemIndex3;
            this.editValue = editValue;
        }

        public int ArrayDimension { get { return arrayDimension; } }
        public int ItemIndex1 { get { return itemIndex1; } }
        public int ItemIndex2 { get { return itemIndex2; } }
        public int ItemIndex3 { get { return itemIndex3; } }

        public object EditValue {
            get { return editValue; }
            set {
                if (EditValue == value) return;
                editValue = value;
                OnEditValueChanged();
            }
        }

        protected virtual void OnEditValueChanged() { }

        public override string ToString() {
            if (ArrayDimension == 3) return "[" + ItemIndex3.ToString() + "][" + ItemIndex2.ToString() + "][" + ItemIndex1.ToString() + "]";
            if (ArrayDimension == 2) return "[" + ItemIndex2.ToString() + "][" + ItemIndex1.ToString() + "]";
            return "[" + ItemIndex1.ToString() + "]";
        }
    }
}
