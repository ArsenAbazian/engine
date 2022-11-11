using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class ModelRefCollectionForm : FormWithSceneUpdate {
        public ModelRefCollectionForm() {
            InitializeComponent();
        }

        private void modelRefCollectionViewControl1_OkClick(object sender, EventArgs e) {
            this.Close();
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            modelRefCollectionViewControl1.MainForm = MainForm;
        }
    }
}