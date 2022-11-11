using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.Dependency {
    public partial class SimpleDependencyPropertyForm : Form {
        public SimpleDependencyPropertyForm() {
            InitializeComponent();
        }

        public a3dDependencyPropertyMapper SelectedProperty {
            get { return simpleDependencyPropertyControl1.SelectedProperty; }
        }
        a3dDependencyPropertyMapper dest;
        public a3dDependencyPropertyMapper Destination {
            get { return dest; }
            set { dest = value; }
        }

        private void button2_Click(object sender, EventArgs e) {
            if(Destination != null && SelectedProperty == null) { 
                MessageBox.Show("Please select source property or press 'Cancel' button", "SimpleDependencyProperty", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return ;
            }
            if(Destination != null && DependencyPropertyHelper.CheckForRecursion(Destination, SelectedProperty)) {
                MessageBox.Show("Error: Recursion detected each of these properties depends from one another", "SimpleDependencyProperty", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            DialogResult = DialogResult.OK;
        }

        public IDependencyTreeNode RootNode { 
            get { return simpleDependencyPropertyControl1.RootNode; }
            set { simpleDependencyPropertyControl1.RootNode = value; }
        }
    }
}
