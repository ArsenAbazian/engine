using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
    public partial class EffectListForm : FormWithSceneUpdate {
        public EffectListForm() {
            InitializeComponent();
        }

        public EffectListControl EffectListControl { get { return effectListControl1; } }
        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            EffectListControl.MainForm = MainForm;
        }
    }
}