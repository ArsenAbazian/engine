using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.Dependency {
    public partial class SimpleDependencyPropertyControl : UserControl {
        public SimpleDependencyPropertyControl() {
            InitializeComponent();
        }

        public a3dDependencyPropertyMapper SelectedProperty { get { return dependencyPropertyTreeControl1.SelectedProperty; } }
        public IDependencyTreeNode RootNode {
            get { return dependencyPropertyTreeControl1.RootNode; }
            set { dependencyPropertyTreeControl1.RootNode = value; }
        }
    }
}
