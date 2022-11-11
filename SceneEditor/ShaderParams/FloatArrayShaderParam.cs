using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class FloatArrayShaderParam : ShaderParamInfoBase {
        public FloatArrayShaderParam() {
            InitializeComponent();
        }

        public NumericUpDown ValueEditor { get { return numericUpDown1; } }
        
        public a3dFloatArrayShaderParamMapper ArrayParam {
            get { return base.Param as a3dFloatArrayShaderParamMapper; }
            set { base.Param = value; }
        }

        public ListBox ItemsList {
            get { return itemsList; }
        }

        protected virtual void UpdateItemsList() {
            ItemsList.Items.Clear();

            if (ArrayParam.Row != 1 || ArrayParam.Col != 1)
                throw new Exception("FloatArrayShaderParam Row or Col !=1 use BoolMatrixArrayShaderParam");

            int index = 0;
            if (ArrayParam.ArraySize3 > 1) {
                for (int i3 = 0; i3 < ArrayParam.ArraySize3; i3++) {
                    for (int i2 = 0; i2 < ArrayParam.ArraySize2; i2++) {
                        for (int i1 = 0; i1 < ArrayParam.ArraySize1; i1++, index++) {
                            ItemsList.Items.Add(new FloatArrayItemInfo(this, 3, i1, i2, i3, ArrayParam.get_Value(index)));
                        }
                    }
                }
            }
            else if (ArrayParam.ArraySize2 > 1) {
                for (int i2 = 0; i2 < ArrayParam.ArraySize2; i2++) {
                    for (int i1 = 0; i1 < ArrayParam.ArraySize1; i1++, index++) {
                        ItemsList.Items.Add(new FloatArrayItemInfo(this, 2, i1, i2, 0, ArrayParam.get_Value(index)));
                    }
                }
            }
            else if (ArrayParam.ArraySize1 > 1) {
                for (int i1 = 0; i1 < ArrayParam.ArraySize1; i1++, index++) {
                    ItemsList.Items.Add(new FloatArrayItemInfo(this, 1, i1, 0, 0, ArrayParam.get_Value(index)));
                }
            }
        }

        private void itemsList_SelectedIndexChanged(object sender, EventArgs e) {
            if (ItemsList.SelectedIndex >= 0)
                ValueEditor.Value = new decimal((float)(ItemsList.SelectedItem as FloatArrayItemInfo).EditValue);
        }

        protected virtual void UpdateParamValue() {
            FloatArrayItemInfo info = ItemsList.SelectedItem as FloatArrayItemInfo;
            if (info == null) return;
                info.EditValue = float.Parse(ValueEditor.Value.ToString());
        }

        protected internal virtual void MapEditorValuesToParam() {
            int index = 0;
            foreach (FloatArrayItemInfo info in ItemsList.Items) {
                ArrayParam.set_Value(index, (float)info.EditValue);
                index++;
            }
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e) {
            UpdateParamValue();
        }

        protected override void OnParamChanged() {
            base.OnParamChanged();
            UpdateItemsList();
            if (ItemsList.Items.Count > 0)
                ItemsList.SelectedIndex = 0;
        }
    }

    public class FloatArrayItemInfo : ArrayItemInfo {
        FloatArrayShaderParam owner;
        public FloatArrayItemInfo(FloatArrayShaderParam owner, int arrayDimension, int itemIndex1, int itemIndex2, int itemIndex3, float editValue)
            : base(arrayDimension, itemIndex1, itemIndex2, itemIndex3, editValue) {
            this.owner = owner;
        }

        public FloatArrayShaderParam Owner { get { return owner; } }
        protected override void OnEditValueChanged() {
            base.OnEditValueChanged();
            Owner.MapEditorValuesToParam();
            Owner.UpdateMainForm();
        }
    }
}
