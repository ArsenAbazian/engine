using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class LandscapePropsEx : ControlWithSceneUpdate {
        public LandscapePropsEx() {
            InitializeComponent();
        }

        LandPassInfoForm passInfoForm;
        public LandPassInfoForm PassInfoForm {  
            get {  
                if(passInfoForm == null || passInfoForm.IsDisposed) {
                    passInfoForm = new LandPassInfoForm();
                    passInfoForm.MainForm = MainForm;
                    passInfoForm.Landscape = Landscape;
                }
                return passInfoForm;
            } 
        }

        a3dLandscapeDesc_Mapper land;
        public a3dLandscapeDesc_Mapper Landscape {
            get { return land; }
            set {
                land = value;
                OnLandscapeChanged();
            }
        }

        public bool IsControlUpdated { get { return landscapeProps1.IsControlUpdated; } }

        protected virtual void OnLandscapeChanged() {
            landscapeProps1.Landscape = Landscape;
            PassInfoForm.Landscape = Landscape;
            return;
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            PassInfoForm.MainForm = MainForm;
        }
        
        private void passProps_Click(object sender, EventArgs e) {
            PassInfoForm.Landscape = Landscape;
            PassInfoForm.Show();
        }

        public event EventHandler OkClick {
            add { okButton.Click += value; }
            remove { okButton.Click -= value; }
        }

        public LandscapeProps LandscapeProps { get { return landscapeProps1; } }

        public event EventHandler NameChanged {
            add { LandscapeProps.NameChanged += value; }
            remove { LandscapeProps.NameChanged -= value; }
        }

        public event EventHandler HeightMapFileChanged {
            add { LandscapeProps.HeightMapFileChanged += value; }
            remove { LandscapeProps.HeightMapFileChanged -= value; }
        }

        public event EventHandler LightMapFileChanged {
            add { LandscapeProps.LightMapFileChanged += value; }
            remove { LandscapeProps.LightMapFileChanged -= value; }
        }

        public event EventHandler EffectChanged {
            add { LandscapeProps.EffectChanged += value; }
            remove { LandscapeProps.EffectChanged -= value; }
        }

        public event EventHandler PixelErrorChanged {
            add { LandscapeProps.PixelErrorChanged += value; }
            remove { LandscapeProps.PixelErrorChanged -= value; }
        }

        public event EventHandler PatchPerSideChanged {
            add { LandscapeProps.PatchPerSideChanged += value; }
            remove { LandscapeProps.PatchPerSideChanged -= value; }
        }

        public event EventHandler SideScaleChanged {
            add { LandscapeProps.SideScaleChanged += value; }
            remove { LandscapeProps.SideScaleChanged -= value; }
        }

        public event EventHandler HeightScaleChanged {
            add { LandscapeProps.HeightScaleChanged += value; }
            remove { LandscapeProps.HeightScaleChanged -= value; }
        }

        public event EventHandler LightMapWidthChanged {
            add { LandscapeProps.LightMapWidthChanged += value; }
            remove { LandscapeProps.LightMapWidthChanged -= value; }
        }

        public event EventHandler LightMapHeightChanged {
            add { LandscapeProps.LightMapHeightChanged += value; }
            remove { LandscapeProps.LightMapHeightChanged -= value; }
        }
    }
}
