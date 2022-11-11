using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class LandPassInfoForm : FormWithSceneUpdate {
		public LandPassInfoForm() {
			InitializeComponent();
		}

        public a3dLandscapeDesc_Mapper Landscape { 
            get { return landscapePassesControl1.Landscape; } 
            set { landscapePassesControl1.Landscape = value; } 
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            landscapePassesControl1.MainForm = MainForm;
        }

        private void okButton_Click(object sender, EventArgs e) {
            this.Close();
        }
    }
}