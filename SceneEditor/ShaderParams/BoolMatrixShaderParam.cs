using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class BoolMatrixShaderParam : BaseMatrixShaderParam {
        public BoolMatrixShaderParam() {
            InitializeComponent();
        }

        CheckBox[,] matrixEditors;
        public CheckBox[,] MatrixEditors { 
            get { 
                if(matrixEditors == null) {
                    matrixEditors = new CheckBox[4, 4];
                    matrixEditors[0, 0] = a00;
                    matrixEditors[0, 1] = a01;
                    matrixEditors[0, 2] = a02;
                    matrixEditors[0, 3] = a03;
                    matrixEditors[1, 0] = a10;
                    matrixEditors[1, 1] = a11;
                    matrixEditors[1, 2] = a12;
                    matrixEditors[1, 3] = a13;
                    matrixEditors[2, 0] = a20;
                    matrixEditors[2, 1] = a21;
                    matrixEditors[2, 2] = a22;
                    matrixEditors[2, 3] = a23;
                    matrixEditors[3, 0] = a30;
                    matrixEditors[3, 1] = a31;
                    matrixEditors[3, 2] = a32;
                    matrixEditors[3, 3] = a33;
                }

                return matrixEditors;
            }
        }

        public new a3dBoolArrayShaderParamMapper Param {
            get { return base.Param as a3dBoolArrayShaderParamMapper; }
            set { base.Param = value; }
        }

        protected override void UpdateMatrixEditorValue(int r, int c, int itemIndex) {
            MatrixEditors[r, c].Checked = Param.get_Value(itemIndex);
        }

        protected override void UpdateMatrixEditors() {
            for(int r = 0; r < 4; r++) { 
                for(int c = 0; c < 4; c++) {
                    MatrixEditors[r, c].Visible = r < Row && c < Col;    
                }
            }
            base.UpdateMatrixEditors();
        }

        protected override void MapMatrixEditorToParam(int row, int col, int itemIndex) {
            base.MapMatrixEditorToParam(row, col, itemIndex);
            Param.set_Value(itemIndex, MatrixEditors[row, col].Checked);
        }

        protected virtual void OnMatrixEditorCheckedChanged(object sender, EventArgs e) {
            MapMatrixEditorsToParam();
        }
    }
}
