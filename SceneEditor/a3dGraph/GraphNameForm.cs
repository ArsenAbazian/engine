using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor.graphViewer {
    public partial class GraphNameForm : Form {
        public GraphNameForm() {
            InitializeComponent();
        }

        public string GraphName {
            get { return textBox1.Text; }
            set { textBox1.Text = value; }
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
        }
    }
}