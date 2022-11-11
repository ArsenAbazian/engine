using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class LandscapePropsForm : FormWithSceneUpdate {
        public LandscapePropsForm() {
            InitializeComponent();
        }

        a3dLandscapeDesc_Mapper land;
        public a3dLandscapeDesc_Mapper Landscape {
            get { return land; }
            set {
                land = value;
                OnLandscapeChanged();
            }
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            landscapePropsEx1.MainForm = MainForm;
        }

        protected virtual void OnLandscapeChanged() {
            landscapePropsEx1.Landscape = Landscape;
        }

        private void landscapePropsEx1_OkClick(object sender, EventArgs e) {
            Close();
        }

        public bool IsFormUpdated { get { return landscapePropsEx1.IsControlUpdated; } }

        private void landscapePropsEx1_NameChanged(object sender, EventArgs e) {
            if (IsFormUpdated) return;
            Landscape.Name = landscapePropsEx1.LandscapeProps.LandName;
        }

        private void landscapePropsEx1_HeightMapFileChanged(object sender, EventArgs e) {
            if (IsFormUpdated) return;
            Landscape.HeightMapFileName = landscapePropsEx1.LandscapeProps.HeightMapFileName;
        }

        private void landscapePropsEx1_LightMapFileChanged(object sender, EventArgs e) {
            if (IsFormUpdated) return;
            Landscape.LightMapFileName = landscapePropsEx1.LandscapeProps.LightMapFileName;
        }

        private void landscapePropsEx1_EffectChanged(object sender, EventArgs e) {
            if (IsFormUpdated) return;
            Landscape.EffectFileName = landscapePropsEx1.LandscapeProps.EffectFileName;
        }

        private void landscapePropsEx1_PixelErrorChanged(object sender, EventArgs e) {
            if (IsFormUpdated) return;
            Landscape.PixelError = landscapePropsEx1.LandscapeProps.PixelError;
        }

        private void landscapePropsEx1_PatchPerSideChanged(object sender, EventArgs e) {
            if (IsFormUpdated) return;
            Landscape.PatchPerSide = landscapePropsEx1.LandscapeProps.PatchPerSide;
        }

        private void landscapePropsEx1_HeightScaleChanged(object sender, EventArgs e) {
            if (IsFormUpdated) return;
            Landscape.Height = landscapePropsEx1.LandscapeProps.HeightScale;
        }

        private void landscapePropsEx1_SideScaleChanged(object sender, EventArgs e) {
            if (IsFormUpdated) return;
            Landscape.Width = landscapePropsEx1.LandscapeProps.SideScale;
        }

        private void landscapePropsEx1_LightMapHeightChanged(object sender, EventArgs e) {
            if (IsFormUpdated) return;
            Landscape.LightMapWidth = landscapePropsEx1.LandscapeProps.LightMapWidth;
        }

        private void landscapePropsEx1_LightMapWidthChanged(object sender, EventArgs e) {
            if (IsFormUpdated) return;
            Landscape.LightMapHeight = landscapePropsEx1.LandscapeProps.LightMapHeight;
        }
    }
}