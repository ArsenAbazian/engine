using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
    public partial class EnterNameForm : Form {
        public EnterNameForm() {
            InitializeComponent();
        }

        public EnterStringControl Control { get { return enterStringControl1; } }

        private void enterStringControl1_OkClick(object sender, EventArgs e) {
            this.Close();
        }
    }
}