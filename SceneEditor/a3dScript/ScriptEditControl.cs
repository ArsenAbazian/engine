using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using Rsdn.Editor.a3dScript;
using Rsdn.Editor.ObjectModel;
using Rsdn.Editor;
using System.Globalization;
using classMapper;
using Intillisense;

namespace sceneEditor.a3dScript {
    public partial class ScriptEditControl : ControlWithSceneUpdate {
        public ScriptEditControl() {
            InitializeComponent();
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            scriptEdit1.Root = MainForm.Scene;
        }

        private void scriptEdit1_SelectionChanged(object sender, SelectionChangedEventArgs e) {
            selLabel.Text = e.StartPosition + " - " + e.EndPosition;
        }
    }
}
