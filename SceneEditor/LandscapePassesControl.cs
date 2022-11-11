using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class LandscapePassesControl : ControlWithSceneUpdate {
		public LandscapePassesControl() {
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
        protected virtual void OnLandscapeChanged() {
            passCollectionControl1.Landscape = Landscape;
            return;
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            passCollectionControl1.MainForm = MainForm;
            landscapePassInfo1.MainForm = MainForm;
        }
	}
}
