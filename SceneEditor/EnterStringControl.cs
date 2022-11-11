using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
    public partial class EnterStringControl : UserControl {
        public EnterStringControl() {
            InitializeComponent();
        }

        public string EnterText { get { return label1.Text; } set { label1.Text = value; } }
        public string Value { get { return textBox1.Text.Trim(); } set { textBox1.Text = value; } }
    
        public event EventHandler OkClick {
            add { button1.Click += value; }
            remove { button1.Click -= value; } 
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter)
                button1.AccessibilityObject.DoDefaultAction();
        }
    }
}
