using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.Lights {
    public partial class a3dLightBaseControl : ControlWithSceneUpdate {
        public a3dLightBaseControl() {
            InitializeComponent();
        }

        a3dLightBase_Mapper light;
        public virtual a3dLightBase_Mapper Light {
            get { return light; }
            set {
                if (Light == value) return;
                light = value;
                OnLightChanged();
            }
        }

        public string LightName {
            get { return lightName.Text; }
            set {
                if (LightName == value) return;
                lightName.Text = value;
                OnLightNameChanged();
            }
        }

        public Color LightColor {
            get { return lightColor.Color; }
            set {
                if (LightColor == value) return;
                lightColor.Color = value;
                OnLightColorChanged();
            }
        }
        
        protected virtual void OnLightNameChanged() { 
            
        }

        protected virtual void OnLightColorChanged() { 
            
        }

        protected virtual void OnLightChanged() {
            if (Light == null) return;
            LightName = Light.Name;
            LightColor = Light.Color;
        }

        private void lightColor_ColorChanged(object sender, EventArgs e) {
            if (Light == null) return;
            Light.Color = LightColor;
        }

        private void lightName_ValueChanged(object sender, EventArgs e) {
            if (Light == null) return;
            Light.Name = LightName;
        }
    }
}
