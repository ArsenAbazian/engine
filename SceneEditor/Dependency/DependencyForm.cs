using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using sceneEditor.Configuration;

namespace sceneEditor.Dependency {
    public partial class DependencyForm : FormWithSceneUpdate {
        public DependencyForm() {
            InitializeComponent();
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            PropertyControl.MainForm = MainForm;
        }

        public DependencyPropertyControl PropertyControl { get { return this.dependencyPropertyControl1; } }

        protected override void SaveSettings() {
            base.SaveSettings();
            Configurator.Default.SerializeAllProperties("GraphViewer", dependencyPropertyControl1.graphControl1.Viewer);
        }
        protected override void ApplySettings() {
            base.ApplySettings();
        }
    }
}
