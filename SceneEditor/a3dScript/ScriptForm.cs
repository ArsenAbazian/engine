using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor.a3dScript {
    public partial class ScriptForm : FormWithSceneUpdate {
        public ScriptForm() {
            InitializeComponent();
        }
        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            scriptEditControl1.MainForm = MainForm;
        }
    }
}
