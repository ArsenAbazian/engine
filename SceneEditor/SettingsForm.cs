using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
    public partial class SettingsForm : FormWithSceneUpdate {
		public SettingsForm() {
			InitializeComponent();
		}

		public SettingsForm(MainForm form) : this() {
            MainForm = form;
            
		}

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            settingsControl1.MainForm = MainForm;
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            MainForm.OnSettingsFormClose();
        }
    }
}