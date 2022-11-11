using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class LandscapePassInfo : ControlWithSceneUpdate {
        private static readonly object passChanged = new object();
        
        public LandscapePassInfo() {
			InitializeComponent();
		}

		private void mfnButton_Click(object sender, EventArgs e) {
			if(imageFileDialog.ShowDialog() == DialogResult.OK) {
				MapFileName = imageFileDialog.FileName;
			}
		}

		private void msfButton_Click(object sender, EventArgs e) {
			if(imageFileDialog.ShowDialog() == DialogResult.OK) {
				MaskFileName = imageFileDialog.FileName;
			}
		}

		private void dfnButton_Click(object sender, EventArgs e) {
			if(imageFileDialog.ShowDialog() == DialogResult.OK) {
				DetailFileName = imageFileDialog.FileName;
			}
        }

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public string PassName { 
            get { return passName.Text; }
            set { passName.Text = value; }
        }

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public string MapFileName { 
            get { return mapFileName.Text; } 
            set {
                if (LandMapPassInfo == null) return;
                mapFileName.Text = value;
                LandMapPassInfo.MapFileName = MapFileName;
                RaisePassChanged();
            } 
        }
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public string MaskFileName { 
            get { return maskFileName.Text; } 
            set {
                if (LandMapPassInfo == null) return;
                maskFileName.Text = value;
                LandMapPassInfo.MaskFileName = MaskFileName;
                RaisePassChanged();
            } 
        }
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public string DetailFileName { 
            get { return detFileName.Text; } 
            set {
                if (LandMapPassInfo == null) return;
                detFileName.Text = value;
                LandMapPassInfo.DetailFileName = DetailFileName;
                RaisePassChanged();
            } 
        }
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public bool PassEnabled { 
            get { return passEnabled.Checked; } 
            set {
                if (LandMapPassInfo == null) return;
                if (passEnabled.Checked == value) return;
                passEnabled.Checked = value;
                LandMapPassInfo.Enabled = PassEnabled;
                RaisePassChanged();
            } 
        }
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public bool PassAnimated { 
            get { return animated.Checked; } 
            set {
                if (LandMapPassInfo == null) return;
                animated.Checked = value;
                LandMapPassInfo.Animated = PassAnimated;
                RaisePassChanged();
            } 
        }
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public float MapUSize { 
            get { return float.Parse(mapUSize.Text); } 
            set {
                if (LandMapPassInfo == null) return;
                mapUSize.Text = value.ToString();
                LandMapPassInfo.MapUSize = MapUSize;
                RaisePassChanged();
            } 
        }
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public float MapVSize { 
            get { return float.Parse(mapVSize.Text); } 
            set {
                if (LandMapPassInfo == null) return;
                mapVSize.Text = value.ToString();
                LandMapPassInfo.MapVSize = MapVSize;
                RaisePassChanged();
            } 
        }
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public float DetailUSize { 
            get { return float.Parse(detUSize.Text); } 
            set {
                if (LandMapPassInfo == null) return;
                detUSize.Text = value.ToString();
                LandMapPassInfo.DetailUSize = DetailUSize;
                RaisePassChanged();
            } 
        }
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public float DetailVSize { 
            get { return float.Parse(detVSize.Text); } 
            set {
                if (LandMapPassInfo == null) return;
                detVSize.Text = value.ToString();
                LandMapPassInfo.DetailVSize = DetailVSize;
                RaisePassChanged();
            } 
        }
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public float MinDetailDistance { 
            get { return float.Parse(minDetAlphaDist.Text); } 
            set {
                if (LandMapPassInfo == null) return;
                minDetAlphaDist.Text = value.ToString();
                LandMapPassInfo.MinDetailDistance = MinDetailDistance;
                RaisePassChanged();
            } 
        }
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public float MaxDetailDistance { 
            get { return float.Parse(maxDetAlphaDist.Text); } 
            set {
                if (LandMapPassInfo == null) return;
                maxDetAlphaDist.Text = value.ToString();
                LandMapPassInfo.MaxDetailDistance = MaxDetailDistance;
                RaisePassChanged();
            } 
        }
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public float MaxDetailAlphaValue { 
            get { return float.Parse(maxDetAlpha.Text); } 
            set {
                if (LandMapPassInfo == null) return;
                maxDetAlpha.Text = value.ToString();
                LandMapPassInfo.MaxDetailAlphaValue = MaxDetailAlphaValue;
                RaisePassChanged();
            } 
        }

        public event EventHandler PassChanged {
            add { Events.AddHandler(passChanged, value); }
            remove { Events.RemoveHandler(passChanged, value); }
        }

        protected virtual void RaisePassChanged() {
            EventHandler handler = Events[passChanged] as EventHandler;
            if (handler != null) handler(this, EventArgs.Empty);
        }

        public event EventHandler PassEnabledChanged {
            add { passEnabled.CheckedChanged += value; }
            remove { passEnabled.CheckedChanged -= value; }
        }
        
		ImageViewForm imageView;
		public ImageViewForm ImageView {
			get {
				if(imageView == null || imageView.IsDisposed)
					imageView = new ImageViewForm();
				return imageView;
			}
		}

		private void vmpfViewButton_Click(object sender, EventArgs e) {
			ImageView.ImageFileName = MapFileName;
            ImageView.MainForm = MainForm;
            ImageView.Show();
		}

		private void msfViewButton_Click(object sender, EventArgs e) {
			ImageView.ImageFileName = MaskFileName;
            ImageView.MainForm = MainForm;
			ImageView.Show();
		}

		private void detButton_Click(object sender, EventArgs e) {
			ImageView.ImageFileName = MaskFileName;
            ImageView.MainForm = MainForm;
			ImageView.Show();
		}

        public a3dLandPassInfo_Mapper LandMapPassInfo { 
            get {
                if (PassListControl == null || PassListControl.PassList.SelectedIndex < 0) return null;
                return PassListControl.PassColl[PassListControl.PassList.SelectedIndex]; 
            }
        }

		PassCollectionControl passListControl;
		public PassCollectionControl PassListControl {
			get { return passListControl; }
			set {
				OnPassListControlChanging();
				passListControl = value;
				OnPassListControlChanged();
			}
		}

		protected virtual void OnPassListControlChanging() {
			if(PassListControl == null) return;
			PassListControl.PassList.ItemCheck -= new ItemCheckEventHandler(OnPassListItemCheck);
			PassListControl.PassList.SelectedIndexChanged -= new EventHandler(OnPassListSelectedIndexChanged);
			PassListControl.PassListInitialized -= new EventHandler(OnPassListInitialized);
		}
		protected virtual void OnPassListControlChanged() {
			PassListControl.PassList.ItemCheck += new ItemCheckEventHandler(OnPassListItemCheck);
			PassListControl.PassList.SelectedIndexChanged += new EventHandler(OnPassListSelectedIndexChanged);
			PassListControl.PassListInitialized += new EventHandler(OnPassListInitialized);
		}
		protected virtual void OnPassListItemCheck(object sender, ItemCheckEventArgs e) {
            PassEnabled = e.NewValue == CheckState.Checked;
        }
		protected virtual void OnPassListInitialized(object sender, EventArgs e) { 
		}
		protected virtual void OnPassListSelectedIndexChanged(object sender, EventArgs e) {
            if (PassListControl.PassList.SelectedIndex == -1)
                this.Enabled = false;
            else
                this.Enabled = true;

            UpdateControls();
        }

        protected virtual void UpdateControls() {
            if (LandMapPassInfo == null) return;
            this.passEnabled.Checked = LandMapPassInfo.Enabled;
            this.animated.Checked = LandMapPassInfo.Animated;
            this.passName.Text = LandMapPassInfo.Name;
            this.mapFileName.Text = LandMapPassInfo.MapFileName;
            this.maskFileName.Text = LandMapPassInfo.MaskFileName;
            this.detFileName.Text = LandMapPassInfo.MaskFileName;
            this.mapUSize.Text = LandMapPassInfo.MapUSize.ToString();
            this.mapVSize.Text = LandMapPassInfo.MapVSize.ToString();
            this.detUSize.Text = LandMapPassInfo.DetailUSize.ToString();
            this.detVSize.Text = LandMapPassInfo.DetailVSize.ToString();
            this.minDetAlphaDist.Text = LandMapPassInfo.MinDetailDistance.ToString();
            this.maxDetAlphaDist.Text = LandMapPassInfo.MaxDetailDistance.ToString();
            this.maxDetAlpha.Text = LandMapPassInfo.MaxDetailDistance.ToString();
        }

        private void passEnabled_CheckedChanged(object sender, EventArgs e) {
            if (LandMapPassInfo == null) return;
            LandMapPassInfo.Enabled = PassEnabled;
            if (PassListControl.PassList.GetItemChecked(PassListControl.PassList.SelectedIndex) == PassEnabled) return;
            PassListControl.PassList.SetItemChecked(PassListControl.PassList.SelectedIndex, PassEnabled);
        }

        private void animated_CheckedChanged(object sender, EventArgs e) {
            if (LandMapPassInfo == null) return;
            LandMapPassInfo.Animated = PassAnimated;
        }

        private void mapUSize_ValueChanged(object sender, EventArgs e) {
            LandMapPassInfo.MapUSize = MapUSize;
        }

        private void mapVSize_ValueChanged(object sender, EventArgs e) {
            LandMapPassInfo.MapVSize = MapVSize;
        }

        private void detUSize_ValueChanged(object sender, EventArgs e) {
            LandMapPassInfo.DetailUSize = DetailUSize;
        }

        private void detVSize_ValueChanged(object sender, EventArgs e) {
            LandMapPassInfo.DetailVSize = DetailVSize;
        }

        private void minDetAlphaDist_ValueChanged(object sender, EventArgs e) {
            LandMapPassInfo.MinDetailDistance = MinDetailDistance;
        }

        private void maxDetAlphaDist_ValueChanged(object sender, EventArgs e) {
            LandMapPassInfo.MaxDetailDistance = MaxDetailDistance;
        }

        private void maxDetAlpha_ValueChanged(object sender, EventArgs e) {
            LandMapPassInfo.MaxDetailAlphaValue = MaxDetailAlphaValue;
        }

        private void passName_TextChanged(object sender, EventArgs e) {
            LandMapPassInfo.Name = PassName;
        }
	}
}
