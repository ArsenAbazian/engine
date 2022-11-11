using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.ShaderParams {
    public partial class TechniqueShaderParam : ShaderParamInfoBase {
        public TechniqueShaderParam() {
            InitializeComponent();
        }

        public new a3dTechniqueShaderParam_Mapper Param {
            get { return base.Param as a3dTechniqueShaderParam_Mapper; }
            set { base.Param = value; }
        }

        protected override void OnParamChanged() {
            base.OnParamChanged();
            InitializeTechList();
        }

        protected virtual void InitializeTechList() {
            techList.Items.Clear();
            int selIndex = -1, i = 0;
            foreach(string str in Param.Material.Techniques) {
                if (str == Param.Technique) selIndex = i;
                techList.Items.Add(str);
                i++;
            }
            techList.SelectedIndex = selIndex;
        }

        private void techList_SelectedIndexChanged(object sender, EventArgs e) {
            Param.Technique = techList.SelectedItem.ToString();
        }
    }
}
