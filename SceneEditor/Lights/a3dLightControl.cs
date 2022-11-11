using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.Lights {
    public partial class a3dLightControl : ControlWithSceneUpdate {
        public a3dLightControl() {
            InitializeComponent();
            typeList.SelectedIndex = 0;
        }
        
        protected virtual void HideControls() {
            a3dDirectionalLightControl1.Visible = false;
            a3dOmniLightControl1.Visible = false;
        }

        public a3dOmniLightControl OmniLightControl { get { return a3dOmniLightControl1; } }
        public a3dDirectionalLightControl DirLightControl { get { return a3dDirectionalLightControl1; } }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            OmniLightControl.MainForm = MainForm;
            DirLightControl.MainForm = MainForm;
        }

        protected virtual void CalcBestSize() {
            if (SelectedLightType == LightType.LightNone) return;
            if(SelectedLightType == LightType.OmniLight)
                groupBox2.Height = a3dOmniLightControl1.Bottom + 4;
            else if(SelectedLightType == LightType.DirectionLight)
                groupBox2.Height = a3dDirectionalLightControl1.Bottom + 4;
            Height = groupBox2.Bottom + 3;            
        }

        public LightType SelectedLightType {
            get { 
                if (typeList.SelectedIndex == 0) return LightType.OmniLight;
                if (typeList.SelectedIndex == 1) return LightType.DirectionLight;
                return LightType.LightNone;
            }
        }

        private void typeList_SelectedIndexChanged(object sender, EventArgs e) {
            if(SelectedLightType == LightType.OmniLight && !a3dOmniLightControl1.Visible) {
                HideControls();
                OmniLightControl.LightName = DirLightControl.LightName;
                OmniLightControl.LightColor = DirLightControl.LightColor;
                OmniLightControl.Visible = true;
            }
            else if (SelectedLightType == LightType.DirectionLight && !a3dDirectionalLightControl1.Visible) {
                HideControls();
                DirLightControl.LightName = OmniLightControl.LightName;
                DirLightControl.LightColor = OmniLightControl.LightColor;
                DirLightControl.Visible = true;
            }
            CalcBestSize();
        }
    }
}
