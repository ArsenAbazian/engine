using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor.ShaderParams {
    public partial class FloatMatrixArrayShaderParam : FloatMatrixShaderParam {
        public FloatMatrixArrayShaderParam() {
            InitializeComponent();
        }

        public ComboBox ItemIndexList { get { return this.itemsIndexList; } }

        protected virtual void InitializeItemList() {
            int index = 0;
            if (Param.ArraySize3 > 1) {
                for (int i3 = 0; i3 < Param.ArraySize3; i3++) {
                    for (int i2 = 0; i2 < Param.ArraySize2; i2++) {
                        for (int i1 = 0; i1 < Param.ArraySize1; i1++, index++) {
                            ItemIndexList.Items.Add(new ArrayItemInfo(3, i1, i2, i3, index));
                        }
                    }
                }
            }
            else if (Param.ArraySize2 > 1) {
                for (int i2 = 0; i2 < Param.ArraySize2; i2++) {
                    for (int i1 = 0; i1 < Param.ArraySize1; i1++, index++) {
                        ItemIndexList.Items.Add(new ArrayItemInfo(2, i1, i2, 0, index));
                    }
                }
            }
            else if (Param.ArraySize1 > 1) {
                for (int i1 = 0; i1 < Param.ArraySize1; i1++, index++) {
                    ItemIndexList.Items.Add(new ArrayItemInfo(1, i1, 0, 0, index));
                }
            }
            ItemIndexList.SelectedIndex = 0;
        }

        protected override void UpdateMatrixEditors() {
            base.UpdateMatrixEditors();
            ItemIndexList.Items.Clear();
            InitializeItemList();
        }

        protected override int ItemIndex1 {
            get {
                if (ItemIndexList.SelectedItem == null) return 0;
                return (ItemIndexList.SelectedItem as ArrayItemInfo).ItemIndex1;
            }
        }

        protected override int ItemIndex2 {
            get {
                if (ItemIndexList.SelectedItem == null) return 0;
                return (ItemIndexList.SelectedItem as ArrayItemInfo).ItemIndex2;
            }
        }

        protected override int ItemIndex3 {
            get {
                if (ItemIndexList.SelectedItem == null) return 0;
                return (ItemIndexList.SelectedItem as ArrayItemInfo).ItemIndex3;
            }
        }

        private void itemIndexList_SelectedIndexChanged(object sender, EventArgs e) {
            UpdateMatrixEditorsValues();
        }
    }
}
