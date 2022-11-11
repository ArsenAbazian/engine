using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;
using sceneEditor.InheritedClassMapper;
using sceneEditor.Configuration;

namespace sceneEditor.Dependency {
    public partial class DependencyPropertyControl : ControlWithSceneUpdate {
        public DependencyPropertyControl() {
            InitializeComponent();
            this.dependencyPropertyListControl1.PropertyControl = this;
        }

        int collapsedWidth = 256;
        int expandedWidth = 900;

        [DefaultValue(256)]
        public int CollapsedWidth {
            get { return collapsedWidth; }
            set {
                if (CollapsedWidth == value)
                    return;
                collapsedWidth = value;
                if (IsControlCollapsed) Width = CollapsedWidth;
            }
        }

        [DefaultValue(720)]
        public int ExpandedWidth {
            get { return expandedWidth; }
            set {
                if (ExpandedWidth == value)
                    return;
                expandedWidth = value;
                if (!IsControlCollapsed) Width = ExpandedWidth;
            }
        }

        public bool IsControlCollapsed { get { return splitContainer1.Panel2Collapsed; } }
        
        void ExpandControl() {
            CollapsedWidth = Width;
            Form frm = Parent as Form;
            if (frm == null) return;
            int width = frm.Width - frm.ClientRectangle.Width;
            width += ExpandedWidth;
            frm.Width = width;
        }

        void CollapseControl() {
            ExpandedWidth = Width;
            Form frm = Parent as Form;
            if (frm == null) return;
            frm.Width = frm.Width - frm.ClientRectangle.Width + CollapsedWidth;
        }

        void InitializeGraphs(Graph selGraph) { 
            //if(PropertyOwner == null) return;
            //graphControl1.Graphs.Clear();
            //foreach (a3dDependencyPropertyMapper prop in PropertyOwner.DependencyProperties) {
            //    if ( (prop.Graph as Graph) == null) continue;
            //    graphControl1.Graphs.Add(prop.Graph as Graph);
            //}
            //graphControl1.UpdateGraphsComboBox(selGraph);
        }
        public void ShowGraphs() {
            ShowGraphs(null);
        }
        public void ShowGraphs(Graph selGraph) {
            int oldDist = splitContainer1.SplitterDistance;
            ExpandControl();
            splitContainer1.Panel2Collapsed = false;
            splitContainer1.SplitterDistance = oldDist;
            Configurator.Default.DeserializeAllProperties("GraphViewer", graphControl1.Viewer);
            InitializeGraphs(selGraph);
        }

        public void HideGraphs() {
            splitContainer1.Panel2Collapsed = true;
            CollapseControl();
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            dependencyPropertyListControl1.MainForm = MainForm;
        }

        public IDependencyPropertyOwner PropertyOwner {
            get { return dependencyPropertyListControl1.PropertyOwner; }
            set { dependencyPropertyListControl1.PropertyOwner = value; }
        }
    }
}
