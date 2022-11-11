using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor.a3dGraph {
    public partial class a3dGraphPropForm : Form {
        public a3dGraphPropForm() {
            InitializeComponent();
        }

        private void propertyGrid1_PropertyValueChanged(object s, PropertyValueChangedEventArgs e) {
            Control ctrl = propertyGrid1.SelectedObject as Control;
            if (ctrl == null)
                ctrl.Invalidate();
        }
    }
}
