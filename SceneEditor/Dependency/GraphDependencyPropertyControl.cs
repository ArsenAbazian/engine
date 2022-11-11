using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;
using sceneEditor.InheritedClassMapper;

namespace sceneEditor.Dependency {
    public partial class GraphDependencyPropertyControl : ControlWithSceneUpdate {
        public GraphDependencyPropertyControl() {
            InitializeComponent();
        }

        private void sourceListBox_SelectedIndexChanged(object sender, EventArgs e) {
            newSourceButton.Enabled = sourceListBox.SelectedItem != null;
            clear.Enabled = sourceListBox.SelectedItem != null;
        }

        public a3dDependencyPropertyMapper Destination { get; set; }

        a3dDependencyPropertyMapper source1, source2;
        Graph graph1, graph2;
        public a3dDependencyPropertyMapper Source1 {
            get { return source1; }
            set {
                if(Source1 != null) {
                    RemoveGraph(Source1.ToString(), "Source 1 - ");
                    graph1 = null;
                }
                source1 = value;
                if(Source1 != null)
                    graph1 = AddGraph(Source1.ToString(), "Source 1 - ");
            }
        }
        public a3dDependencyPropertyMapper Source2 {
            get { return source2; }
            set {
                if(Source2 != null) {
                    RemoveGraph(Source2.ToString(), "Source 2 - ");
                    graph2 = null;
                }
                source2 = value;
                if(Source2 != null)
                    graph2 = AddGraph(Source2.ToString(), "Source 2 - ");
            }
        }

        public Graph Graph { get { return graph1; } }
        public Graph Graph2 { get { return graph2; } }

        protected void RemoveGraph(string name, string sourceName) {
            graphControl1.RemoveGraph(sourceName + name);
        }

        protected Graph AddGraph(string name, string sourceName) {
            return graphControl1.AddGraph(sourceName + name);
        }

        private void newSourceButton_Click(object sender, EventArgs e) {
            FindForm().Visible = false;
            try {
                using(SimpleDependencyPropertyForm form = new SimpleDependencyPropertyForm()) {
                    form.Destination = Destination;
                    form.RootNode = MainForm.Scene;
                    if(form.ShowDialog() == DialogResult.Cancel || sourceListBox.SelectedItem == null) return;
                    if(sourceListBox.SelectedIndex == 0) Source1 = form.SelectedProperty;
                    else Source2 = form.SelectedProperty;
                    UpdateListBoxCaptions();
                }
            }
            finally {
                FindForm().Visible = true;
            }
        }

        protected virtual void UpdateListBoxCaptions() {
            sourceListBox.Items.Clear();
            sourceListBox.Items.Add(Source1 == null ? "Select Source Property 1" : Source1.ToString());
            sourceListBox.Items.Add(Source2 == null ? "Select Source Property 2" : Source2.ToString());
        }

        private void clear_Click(object sender, EventArgs e) {
            if(sourceListBox.SelectedIndex == 0) Source1 = null;
            else Source2 = null;
            UpdateListBoxCaptions();
        }

        public bool LoopGraph { 
            get { return loopGraph1.Checked; }
            set { loopGraph1.Checked = value; }
        }
        public bool LoopGraph2 { 
            get { return loopGraph2.Checked; }
            set { loopGraph2.Checked = value; }
        }
        public GraphOperationType Operation { 
            get {
                switch(operationCombo.SelectedIndex) {
                    case 0: return GraphOperationType.None;
                    case 1: return GraphOperationType.SelectOp1;
                    case 2: return GraphOperationType.SelectOp2;
                    case 3: return GraphOperationType.Add;
                    case 4: return GraphOperationType.Sub;
                    case 5: return GraphOperationType.Mul;
                    case 6: return GraphOperationType.Div;
                }
                return GraphOperationType.None;
            }
            set {
                switch(value) {
                    case GraphOperationType.None:
                        operationCombo.SelectedIndex = 0;
                        break;
                    case GraphOperationType.SelectOp1:
                        operationCombo.SelectedIndex = 1;
                        break;
                    case GraphOperationType.SelectOp2:
                        operationCombo.SelectedIndex = 2;
                        break;
                    case GraphOperationType.Add:
                        operationCombo.SelectedIndex = 3;
                        break;
                    case GraphOperationType.Sub:
                        operationCombo.SelectedIndex = 4;
                        break;
                    case GraphOperationType.Mul:
                        operationCombo.SelectedIndex = 5;
                        break;
                    case GraphOperationType.Div:
                        operationCombo.SelectedIndex = 6;
                        break;
                }
            }
        }
    }
}
