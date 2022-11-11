using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.Dependency {
    public partial class DependencyPropertyTreeControl : ControlWithSceneUpdate {
        public DependencyPropertyTreeControl() {
            InitializeComponent();
        }

        public IDependencyTreeNode RootNode {
            get { return dependencyTree1.RootNode; }
            set { dependencyTree1.RootNode = value; }
        }

        private void dependencyTree1_AfterSelect(object sender, TreeViewEventArgs e) {
            IDependencyTreeNode node = e.Node.Tag as IDependencyTreeNode;
            if (node == null || node.DependencyOwner == null) {
                dependencyPropertyListControl1.Visible = false;
                label1.Visible = true;
            }
            else {
                dependencyPropertyListControl1.Visible = true;
                dependencyPropertyListControl1.PropertyOwner = (e.Node.Tag as IDependencyTreeNode).DependencyOwner;                
                label1.Visible = false;
            }
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            dependencyPropertyListControl1.MainForm = MainForm;
        }

        public a3dDependencyPropertyMapper SelectedProperty {
            get { 
                return dependencyPropertyListControl1.SelectedPropertyInfo == null? null:
                            dependencyPropertyListControl1.SelectedPropertyInfo.Property; 
            }
        }
    }

    public class DependencyPropertyHelper {
        static bool CheckForRecursionCore(a3dDependencyPropertyMapper from, a3dDependencyPropertyMapper to) {
            if(from.IsNativeEquals(to)) return true;
            if(from.NotifyProperties == null) return false;
            foreach(a3dDependencyPropertyMapper prop in from.NotifyProperties) {
                if(CheckForRecursion(prop, to)) return true;
            }
            return false;
        }
        public static bool CheckForRecursion(a3dDependencyPropertyMapper p1, a3dDependencyPropertyMapper p2) {
            if(CheckForRecursionCore(p1, p2)) return true;
            if(CheckForRecursionCore(p2, p1)) return true;
            return false;
        }
    }
}
