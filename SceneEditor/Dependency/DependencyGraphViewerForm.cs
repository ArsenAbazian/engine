using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.Dependency {
    public partial class DependencyGraphViewerForm : FormWithSceneUpdate {
        public DependencyGraphViewerForm() {
            InitializeComponent();
        }

        public IDependencyTreeNode RootNode { 
            get { return dependencyGraphViewer1.RootNode; }
            set { dependencyGraphViewer1.RootNode = value; }
        }

        public a3dDependencyPropertyMapper Property { 
            get { return dependencyGraphViewer1.Property; } 
            set { dependencyGraphViewer1.Property = value; } 
        }

        private void updateViewMenuItem_Click(object sender, EventArgs e) {
            dependencyGraphViewer1.UpdateView();
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            dependencyGraphViewer1.MainForm = MainForm;
        }

        protected override void OnShown(EventArgs e) {
            base.OnShown(e);
            dependencyGraphViewer1.UpdateView();
        }
    }
}
