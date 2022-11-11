using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor.ShaderParams {
    public partial class BaseMatrixShaderParam : ShaderParamInfoBase {
        public BaseMatrixShaderParam() {
            InitializeComponent();
        }

        int row = 1, col = 1;
        [DefaultValue(1)]
        public int Row {
            get { return row; }
            set {
                if (Row == value) return;
                row = value;
                UpdateMatrixEditors();
            }
        }

        [DefaultValue(1)]
        public int Col {
            get { return col; }
            set {
                if (Col == value) return;
                col = value;
                UpdateMatrixEditors();
            }
        }

        protected override void OnParamChanged() {
            base.OnParamChanged();
            Row = Param.Row;
            Col = Param.Col;
        }

        protected virtual int Map3ArrayTo1Array(int itemIndex3, int itemIndex2, int itemIndex1) {
            int itemArrayCount = Param.Row * Param.Col;
            int index = 0;
            if (itemIndex3 > 0) index += itemArrayCount * itemIndex3 * Param.ArraySize2 * Param.ArraySize1;
            if (itemIndex2 > 0) index += itemArrayCount * itemIndex2 * Param.ArraySize1;
            if (itemIndex1 > 0) index += itemArrayCount * itemIndex1;
            return index;
        }

        protected virtual int ItemIndex1 { get { return 0; } }
        protected virtual int ItemIndex2 { get { return 0; } }
        protected virtual int ItemIndex3 { get { return 0; } }

        protected virtual void UpdateMatrixEditorValue(int r, int c, int itemIndex) { 
            
        }
        
        protected virtual int GetParamIndex(int r, int c) {
            return Map3ArrayTo1Array(ItemIndex3, ItemIndex2, ItemIndex1) + r * Col + c;
        }

        bool suppressMapEditorsToParam = false;
        protected virtual void UpdateMatrixEditorsValues() {
            suppressMapEditorsToParam = true;
            for(int r = 0; r < Row; r++) { 
                for(int c = 0; c < Col; c++) {
                    UpdateMatrixEditorValue(r, c, GetParamIndex(r, c));
                }
            }
            suppressMapEditorsToParam = false;
        }

        protected virtual void MapMatrixEditorToParam(int row, int col, int itemIndex) { 
            
        }

        protected virtual void MapMatrixEditorsToParam() {
            if (suppressMapEditorsToParam) return;
            for (int r = 0; r < Row; r++) {
                for (int c = 0; c < Col; c++) {
                    MapMatrixEditorToParam(r, c, GetParamIndex(r, c));
                }
            }
            UpdateMainForm();
        }

        protected virtual void UpdateMatrixEditors() {
            
        }
    }
}
