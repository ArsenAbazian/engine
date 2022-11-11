using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;
using sceneEditor.InheritedClassMapper;

namespace sceneEditor.Dependency {
    public partial class GraphDependencyPropertyForm : FormWithSceneUpdate {
        public GraphDependencyPropertyForm() {
            InitializeComponent();
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            graphDependencyPropertyControl1.MainForm = MainForm;
        }

        public a3dDependencyPropertyMapper Source1 {
            get { return graphDependencyPropertyControl1.Source1; }
            set { graphDependencyPropertyControl1.Source1 = value; }
        }

        public a3dDependencyPropertyMapper Source2 {
            get { return graphDependencyPropertyControl1.Source2; }
            set { graphDependencyPropertyControl1.Source2 = value; }
        }

        public a3dDependencyPropertyMapper Destination {
            get { return graphDependencyPropertyControl1.Destination; }
            set { graphDependencyPropertyControl1.Destination = value; }
        }

        public Graph Graph { get { return graphDependencyPropertyControl1.Graph; } }
        public Graph Graph2 { get { return graphDependencyPropertyControl1.Graph2; } }

        public bool LoopGraph { 
            get { return graphDependencyPropertyControl1.LoopGraph; }
            set { graphDependencyPropertyControl1.LoopGraph = value; }
        }

        public bool LoopGraph2 {
            get { return graphDependencyPropertyControl1.LoopGraph2; }
            set { graphDependencyPropertyControl1.LoopGraph2 = value; }
        }

        public GraphOperationType Operation {
            get { return graphDependencyPropertyControl1.Operation; }
            set { graphDependencyPropertyControl1.Operation = value; }
        }
    }
}
