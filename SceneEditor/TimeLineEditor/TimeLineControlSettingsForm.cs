using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor.TimeLineEditor {
    public partial class TimeLineSettingsForm : Form {
        public TimeLineSettingsForm() {
            InitializeComponent();
        }

        public TimeLineEditor TimeLineEditor {
            get { return propertyGrid1.SelectedObject as TimeLineEditor; }
            set { propertyGrid1.SelectedObject = value; }
        }

        private void okButton_Click(object sender, EventArgs e) {
            Close();
        }
    }
}
