using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class BoolArrayShaderParam : ShaderParamInfoBase {
        public BoolArrayShaderParam() {
            InitializeComponent();
        }
    
        public CheckedListBox ValueList {
            get { return valueList; }
        }

        private void valueList_SelectedValueChanged(object sender, EventArgs e) {
            BoolArrayItemInfo info = ValueList.Items[ValueList.SelectedIndex] as BoolArrayItemInfo;
            info.EditValue = ValueList.CheckedIndices.Contains(ValueList.SelectedIndex);
        }

        protected override void OnParamChanged() {
            base.OnParamChanged();
            UpdateList();
        }

        public new a3dBoolArrayShaderParamMapper Param {
            get { return base.Param as a3dBoolArrayShaderParamMapper; }
            set { base.Param = value; }
        }

        protected virtual void UpdateList() {
            ValueList.Items.Clear();

            if (Param.Row != 1 || Param.Col != 1)
                throw new Exception("BoolArrayShaderParam Row or Col !=1 use BoolMatrixArrayShaderParam");

            int index = 0;
            if(Param.ArraySize3 > 1) { 
                for(int i3 = 0; i3 < Param.ArraySize3; i3++) { 
                    for( int i2 = 0; i2 < Param.ArraySize2; i2++ ) { 
                        for( int i1 = 0; i1 < Param.ArraySize1; i1++, index++ ) {
                            ValueList.Items.Add(new BoolArrayItemInfo(this, 3, i1, i2, i3, Param.get_Value(index)));
                            ValueList.SetItemChecked(index, Param.get_Value(index));
                        }
                    }
                }
            }
            else if(Param.ArraySize2 > 1) {
                for (int i2 = 0; i2 < Param.ArraySize2; i2++) {
                    for (int i1 = 0; i1 < Param.ArraySize1; i1++, index++) {
                        ValueList.Items.Add(new BoolArrayItemInfo(this, 2, i1, i2, 0, Param.get_Value(index)));
                        ValueList.SetItemChecked(index, Param.get_Value(index));
                    }
                }
            }
            else if (Param.ArraySize1 > 1) {
                for (int i1 = 0; i1 < Param.ArraySize1; i1++, index++) {
                    ValueList.Items.Add(new BoolArrayItemInfo(this, 1, i1, 0, 0, Param.get_Value(index)));
                    ValueList.SetItemChecked(index, Param.get_Value(index));
                }
            }
        }

        protected internal virtual void MapEditorValuesToParam() {
            int index = 0;
            foreach (BoolArrayItemInfo info in ValueList.Items) {
                Param.set_Value(index, (bool)info.EditValue);
                index++;
            }
        }
    }

    public class BoolArrayItemInfo : ArrayItemInfo { 
        BoolArrayShaderParam owner;
        public BoolArrayItemInfo(BoolArrayShaderParam owner, int arrayDimension, int itemIndex1, int itemIndex2, int itemIndex3, bool editValue) : base(arrayDimension, itemIndex1, itemIndex2, itemIndex3, editValue) {
            this.owner = owner;
        }

        public BoolArrayShaderParam Owner { get { return owner; } }
        protected override void OnEditValueChanged() {
            base.OnEditValueChanged();
            Owner.MapEditorValuesToParam();
            Owner.UpdateMainForm();
        }
    }
}
