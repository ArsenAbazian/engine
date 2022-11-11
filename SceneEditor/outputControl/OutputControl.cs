using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.outputControl {
    public partial class OutputControl : UserControl {
        a3dRichTextOutput textOutput;
        
        public OutputControl() {
            InitializeComponent();
            textOutput = new a3dRichTextOutput();
            textOutput.TextBox = outputBox;
        }
    }

    public class a3dRichTextOutput : a3dOutputManaged {
        RichTextBox textBox;

        public RichTextBox TextBox { 
            get { return textBox; }
            set { textBox = value; }
        }
        
        public override void outManagedInfo(string text) {
            textBox.SelectionStart = textBox.TextLength;
            textBox.SelectionLength = 0;
            textBox.SelectionColor = Color.Black;
            textBox.SelectedText = text;
        }

        public override void outManagedWarning(string text) {
            textBox.SelectionStart = textBox.TextLength;
            textBox.SelectionLength = 0;
            textBox.SelectionColor = Color.Blue;
            textBox.SelectedText = text;
        }

        public override void outManagedError(string text) {
            textBox.SelectionStart = textBox.TextLength;
            textBox.SelectionLength = 0;
            textBox.SelectionColor = Color.Red;
            textBox.SelectedText = text;
        }
    }
}
