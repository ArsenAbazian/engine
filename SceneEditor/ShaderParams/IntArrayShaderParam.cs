using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class IntArrayShaderParam : IntShaderParam {
        public IntArrayShaderParam() {
            InitializeComponent();
        }

        public a3dIntArrayShaderParamMapper ArrayParam {
            get { return base.Param as a3dIntArrayShaderParamMapper; }
            set { base.Param = value; }
        }

        public ListBox ItemsList {
            get { return itemsList; }
        }

        protected override void UpdateValueEditor() {
            UpdateItemsList();
            ItemsList.SelectedIndex = 0;    
        }

        protected virtual void UpdateItemsList() {
            ItemsList.Items.Clear();

            if (ArrayParam.Row != 1 || ArrayParam.Col != 1)
                throw new Exception("IntArrayShaderParam Row or Col !=1 use BoolMatrixArrayShaderParam");

            int index = 0;
            if (ArrayParam.ArraySize3 > 1) {
                for (int i3 = 0; i3 < ArrayParam.ArraySize3; i3++) {
                    for (int i2 = 0; i2 < ArrayParam.ArraySize2; i2++) {
                        for (int i1 = 0; i1 < ArrayParam.ArraySize1; i1++, index++) {
                            ItemsList.Items.Add(new IntArrayItemInfo(this, 3, i1, i2, i3, ArrayParam.get_Value(index)));
                        }
                    }
                }
            }
            else if (ArrayParam.ArraySize2 > 1) {
                for (int i2 = 0; i2 < ArrayParam.ArraySize2; i2++) {
                    for (int i1 = 0; i1 < ArrayParam.ArraySize1; i1++, index++) {
                        ItemsList.Items.Add(new IntArrayItemInfo(this, 2, i1, i2, 0, ArrayParam.get_Value(index)));
                    }
                }
            }
            else if (ArrayParam.ArraySize1 > 1) {
                for (int i1 = 0; i1 < ArrayParam.ArraySize1; i1++, index++) {
                    ItemsList.Items.Add(new IntArrayItemInfo(this, 1, i1, 0, 0, ArrayParam.get_Value(index)));
                }
            }
        }

        private void itemsList_SelectedIndexChanged(object sender, EventArgs e) {
            if(ItemsList.SelectedIndex >= 0)
                ValueEditor.Value = new decimal((int)(ItemsList.SelectedItem as IntArrayItemInfo).EditValue);
        }

        protected override void UpdateParamValue() {
            IntArrayItemInfo info = ItemsList.SelectedItem as IntArrayItemInfo;
            if (info == null) return;
            info.EditValue = int.Parse(ValueEditor.Value.ToString());
        }

        protected internal virtual void MapEditorValuesToParam() {
            int index = 0;
            foreach(IntArrayItemInfo info in ItemsList.Items) {
                ArrayParam.set_Value(index, (int)info.EditValue);
                index++;
            }
        }
    }

    public class IntArrayItemInfo : ArrayItemInfo { 
        IntArrayShaderParam owner;
        public IntArrayItemInfo(IntArrayShaderParam owner, int arrayDimension, int itemIndex1, int itemIndex2, int itemIndex3, int editValue) : base(arrayDimension, itemIndex1, itemIndex2, itemIndex3, editValue) {
            this.owner = owner;
        }

        public IntArrayShaderParam Owner { get { return owner; } }
        protected override void OnEditValueChanged() {
            base.OnEditValueChanged();
            Owner.MapEditorValuesToParam();
            Owner.UpdateMainForm();
        }
    }
}
