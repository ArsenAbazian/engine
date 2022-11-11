using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.Dependency {
    public partial class DependencyPropertyTreeForm : FormWithSceneUpdate {
        public DependencyPropertyTreeForm() {
            InitializeComponent();
        }

        public IDependencyTreeNode RootNode { 
            get { return dependencyPropertyTreeControl1.RootNode; } 
            set { dependencyPropertyTreeControl1.RootNode = value; } 
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            dependencyPropertyTreeControl1.MainForm = MainForm;
        }

        protected override void OnClosed(EventArgs e) {
            base.OnClosed(e);
            if (MainForm != null) MainForm.dependencyTreeMenuItem.Checked = false;
        }
    }
}
