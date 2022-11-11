using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class AddModelRefForm : FormWithSceneUpdate {
        public AddModelRefForm() {
            InitializeComponent();
        }
    
        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            addModelRefControl1.MainForm = MainForm;
        }

        private void addModelRefControl1_OkClick(object sender, EventArgs e) {
            this.Close();
        }
    }
}