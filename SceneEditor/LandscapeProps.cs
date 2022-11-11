using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class LandscapeProps : ControlWithSceneUpdate {
        public LandscapeProps() {
            InitializeComponent();
        }

        private void hmButton_Click(object sender, EventArgs e) {
            if( pngDialog.ShowDialog() == DialogResult.OK ) {
                this.hmText.Text = pngDialog.FileName;
            }
        }

        private void lmButton_Click(object sender, EventArgs e) {
            if( pngDialog.ShowDialog() == DialogResult.OK ) {
                this.lmText.Text = pngDialog.FileName;
            }
        }

        a3dLandscapeDesc_Mapper land;
        public a3dLandscapeDesc_Mapper Landscape {
            get { return land; }
            set {
                OnLandscapeChanging();
                land = value;
                OnLandscapeChanged();
            }
        }

        int[] ppsVal;
        public int[] PatchPerSideValues { 
            get {
                if (ppsVal == null) ppsVal = new int[] { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 };
                return ppsVal;
            } 
        }

        public int getPPSIndex(int val) {
            for (int i = 0; i < pps.Items.Count && i < PatchPerSideValues.Length; i++)
                if (PatchPerSideValues[i] == val) return i;
            return 0;
        }

        protected virtual void OnLandscapeChanging() {
            if (Landscape != null)
                Landscape.ObjectChanged -= new EventHandler(OnLandPropertiesChanged);
        }

        protected virtual void OnLandscapeChanged() {
            controlUpdated = true;
            UpdateControl();
            controlUpdated = false;
        }

        protected virtual void OnLandPropertiesChanged(object sender, EventArgs e) {
            if (MainForm != null) {
                MainForm.UpdateForm();
            }
        }

        bool controlUpdated = false;
        public bool IsControlUpdated { get { return controlUpdated; } }

        protected virtual void UpdateControl() {
            if (Landscape == null) return;
            this.name.Text = Landscape.Name;
            this.hmText.Text = Landscape.HeightMapFileName;
            this.lmText.Text = Landscape.LightMapFileName;
            this.effect.Text = Landscape.EffectFileName;
            this.pixelError.Text = Landscape.PixelError.ToString();
            this.pps.SelectedIndex = getPPSIndex(Landscape.PatchPerSide);
            this.heightScale.Text = Landscape.Height.ToString();
            this.sideScale.Text = Landscape.Width.ToString();
            this.lightMapWidth.Text = Landscape.LightMapWidth.ToString();
            this.lightMapHeight.Text = Landscape.LightMapHeight.ToString();
        }

        public string LandName { get { return name.Text.Trim(); } }
        public string HeightMapFileName { get { return hmText.Text; } }
        public string LightMapFileName { get { return lmText.Text; } }
        public string EffectFileName { get { return effect.Text; } }
        public float PixelError { get { return float.Parse(this.pixelError.Text); } }
        public int PatchPerSide { get { return Int32.Parse(pps.Text); } }
        public float HeightScale { get { return float.Parse( this.heightScale.Text ); } }
        public float SideScale { get { return float.Parse( this.sideScale.Text ); } }
        public int LightMapWidth { get { return Int32.Parse( this.lightMapWidth.Text ); } }
        public int LightMapHeight { get { return Int32.Parse( this.lightMapHeight.Text ); } }

        [Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public bool IsCorrectData { 
            get {
                if (DesignMode) return true;
                if(LandName == string.Empty) {
                    MessageBox.Show("Error: LandName must not be empty!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                
                if( !System.IO.File.Exists( HeightMapFileName ) ) {
                    MessageBox.Show("Error: HeightMapFileName = '" + HeightMapFileName + "' is not correct!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                if (!System.IO.File.Exists(LightMapFileName)) {
                    MessageBox.Show("Error: LightMapFileName = '" + LightMapFileName + "' is not correct!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                if (!System.IO.File.Exists(EffectFileName)) {
                    MessageBox.Show("Error: EffectFileName = '" + EffectFileName + "' is not correct!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                try {
                    float pe = PixelError;
                }
                catch (Exception) {
                    MessageBox.Show("Error: PixelError value not correct!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                
                try {
                    int pps = PatchPerSide;
                }
                catch(Exception) {
                    MessageBox.Show("Error: PatchPerSide value not correct!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                try {
                    float hs = HeightScale;
                }
                catch (Exception) {
                    MessageBox.Show("Error: HeightScale value not correct!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                try {
                    float ss = SideScale;
                }
                catch (Exception) {
                    MessageBox.Show("Error: SideScale value not correct!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                try {
                    int lmw = LightMapWidth;
                }
                catch (Exception) {
                    MessageBox.Show("Error: LightMapWidth value not correct!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                try {
                    int lmh = LightMapHeight;
                }
                catch (Exception) {
                    MessageBox.Show("Error: LightMapHeight value not correct!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                return true;
            }
        }
        
        private void button1_Click(object sender, EventArgs e) {
            if( effectDialog.ShowDialog() == DialogResult.OK ) {
                this.effect.Text = effectDialog.FileName;
            }
        }

        public event EventHandler NameChanged {
            add { name.TextChanged += value; }
            remove { name.TextChanged -= value; }
        }

        public event EventHandler HeightMapFileChanged {
            add { hmText.TextChanged += value; }
            remove { hmText.TextChanged -= value; }
        }

        public event EventHandler LightMapFileChanged {
            add { lmText.TextChanged += value; }
            remove { lmText.TextChanged -= value; }
        }

        public event EventHandler EffectChanged {
            add { effect.TextChanged += value; }
            remove { effect.TextChanged -= value; }
        }

        public event EventHandler PixelErrorChanged {
            add { pixelError.ValueChanged += value; }
            remove { pixelError.ValueChanged -= value; }
        }

        public event EventHandler PatchPerSideChanged {
            add { pps.SelectedIndexChanged += value; }
            remove { pps.SelectedIndexChanged -= value; }
        }

        public event EventHandler SideScaleChanged {
            add { sideScale.ValueChanged += value; }
            remove { sideScale.ValueChanged -= value; }
        }

        public event EventHandler HeightScaleChanged {
            add { heightScale.ValueChanged += value; }
            remove { heightScale.ValueChanged -= value; }
        }

        public event EventHandler LightMapWidthChanged {
            add { lightMapWidth.ValueChanged += value; }
            remove { lightMapWidth.ValueChanged -= value; }
        }

        public event EventHandler LightMapHeightChanged {
            add { lightMapHeight.ValueChanged += value; }
            remove { lightMapHeight.ValueChanged -= value; }
        }
    }
}
