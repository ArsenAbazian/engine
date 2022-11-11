using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;
using sceneEditor.InheritedClassMapper;
using sceneEditor.a3dGraph;

namespace sceneEditor.graphViewer {
    public partial class GraphControl : UserControl {
        public GraphControl() {
            InitializeComponent();
            this.nodeTypeCombo.SelectedIndex = 1;
        }

        GraphNameForm graphNameForm;
        public GraphNameForm GraphForm { 
            get {
                if (graphNameForm == null || graphNameForm.IsDisposed)
                    graphNameForm = new GraphNameForm();
                return graphNameForm;
            }
        }

        public Graph AddGraph(string name) {
            Graph graph = new Graph(true);
            graph.GraphName = name;
            Viewer.Graphs.Add(graph);
            UpdateGraphsComboBox(graph);
            return graph;
        }

        public void RemoveGraph(string name) {
            foreach(Graph graph in Graphs) {
                if(graph.GraphName == name) {
                    Graphs.Remove(graph);
                    UpdateGraphsComboBox(null);
                    return;
                }
            }
        }

        private void addNewGraphMenuItem_Click(object sender, EventArgs e) {
            if (GraphForm.Visible) return;
            if(GraphForm.ShowDialog() == DialogResult.OK) {
                AddGraph(GraphForm.GraphName);
            }
        }

        public GraphViewer Viewer { get { return graphViewer; } }
        public GraphCollection Graphs { get { return Viewer.Graphs; } }

        protected internal virtual void UpdateGraphsComboBox(Graph selGraph) {
            int selIndex = 0;
            this.graphComboBox.Items.Clear();
            foreach(Graph graph in Viewer.Graphs) {
                this.graphComboBox.Items.Add(new GraphInfo(graph));
                if (selGraph == graph)
                    selIndex = this.graphComboBox.Items.Count - 1;
            }
            if (selIndex >= 0 && selIndex < this.graphComboBox.Items.Count)
                this.graphComboBox.SelectedIndex = selIndex;
        }

        private void graphButton_MouseDown(object sender, MouseEventArgs e) {
            graphMenu.Show(graphButton.PointToScreen(new Point(0, graphButton.Bottom)));
        }

        private void nodeButton_Click(object sender, EventArgs e) {
            nodeMenu.Show(nodeButton.PointToScreen(new Point(0, nodeButton.Bottom)));
        }

        private void graphComboBox_SelectedIndexChanged(object sender, EventArgs e) {
            GraphInfo info = this.graphComboBox.SelectedItem as GraphInfo;
            if (info == null) {
                this.graphName.Text = "";
                this.visibleCheck.Checked = false;
                this.widthEdit.Text = "";
            }
            else {
                this.graphName.Text = info.Graph.GraphName;
                this.visibleCheck.Checked = info.Graph.Visible;
                this.widthEdit.Text = info.Graph.Width.ToString();
            }
            UpdateNodeTypeCombo();
            UpdateNodeMenu();
            Viewer.Refresh();
        }

        protected virtual void OnToggleNode(object sender, EventArgs e) {
            if (SelectedGraphInfo.Graph.SelectedNode != null) {
                SelectedGraphInfo.Graph.SelectedNode.BResult = !SelectedGraphInfo.Graph.SelectedNode.BResult;
            }
            Invalidate();
        }

        ToolStripMenuItem toggleMenuItem;
        protected virtual void UpdateNodeMenu() {
            if (SelectedGraphInfo.Graph.Type != GraphType.Bool) {
                if (nodeMenu.Items.Contains(toggleMenuItem))
                    nodeMenu.Items.Remove(toggleMenuItem);
                return;
            }
            if(toggleMenuItem == null)
                toggleMenuItem = new ToolStripMenuItem("Toggle", null, new EventHandler(OnToggleNode));
            nodeMenu.Items.Add(toggleMenuItem);
        }
        
        protected virtual void UpdateNodeTypeCombo() {
            if (SelectedGraphInfo != null && SelectedGraphInfo.Graph.Type == GraphType.Bool)
                nodeTypeCombo.Enabled = false;
            else
                nodeTypeCombo.Enabled = true;
        }

        private void graphComboBox_DrawItem(object sender, DrawItemEventArgs e) {
            if (e.State == DrawItemState.HotLight)
                e.Graphics.FillRectangle(new SolidBrush(SystemColors.Control), e.Bounds);
            else
                e.Graphics.FillRectangle(new SolidBrush(Color.White), e.Bounds);

            if (e.Index < 0) return;
            GraphInfo info = graphComboBox.Items[e.Index] as GraphInfo;
            if(info == null) return;
            e.Graphics.DrawString(info.Graph.GraphName, graphComboBox.Font, new SolidBrush(graphComboBox.ForeColor), new PointF(e.Bounds.X, e.Bounds.Y));
            Rectangle rect = new Rectangle(e.Bounds.Right - 17, e.Bounds.Top + (e.Bounds.Height - 16)/ 2, 16, 16);
            e.Graphics.FillRectangle(new SolidBrush(info.Graph.Color), rect);
            e.Graphics.DrawRectangle(Pens.Black, rect);
        }

        private void graphComboBox_MouseDown(object sender, MouseEventArgs e) {
            Rectangle rect = new Rectangle(this.graphComboBox.Width - 36, (this.graphComboBox.Height - 16) / 2, 16, 16);
            if (!rect.Contains(e.Location)) return;
            if (graphColorDialog.ShowDialog() != DialogResult.OK) return;
            GraphInfo info = this.graphComboBox.SelectedItem as GraphInfo;
            if (info == null) return;
            info.Graph.Color = graphColorDialog.Color;
            Viewer.Refresh();
        }

        public GraphInfo SelectedGraphInfo { get { return this.graphComboBox.SelectedItem as GraphInfo; } }

        private void widthEdit_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode != Keys.Enter) return;
            if (SelectedGraphInfo == null) return;
            SelectedGraphInfo.Graph.Width = float.Parse(widthEdit.Text);
            Viewer.Refresh();
        }

        private void graphName_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode != Keys.Enter) return;
            if (SelectedGraphInfo == null) return;
            SelectedGraphInfo.Graph.GraphName = graphName.Text;
            Viewer.Refresh();
        }

        private void widthEdit_Leave(object sender, EventArgs e) {
            if (SelectedGraphInfo == null) return;
            SelectedGraphInfo.Graph.Width = float.Parse(widthEdit.Text);
        }

        private void graphName_Leave(object sender, EventArgs e) {
            if (SelectedGraphInfo == null) return;
            SelectedGraphInfo.Graph.GraphName = graphName.Text;
        }

        protected virtual a3dGraphNodeType_Mapper NodeType { 
            get {
                if (SelectedGraphInfo.Graph.Type == GraphType.Bool) return a3dGraphNodeType_Mapper.Bool;
                a3dGraphNodeType_Mapper type = a3dGraphNodeType_Mapper.Const;
                if (nodeTypeCombo.SelectedItem.ToString() == "Line") type = a3dGraphNodeType_Mapper.Line;
                else if (nodeTypeCombo.SelectedItem.ToString() == "BezieSpline") type = a3dGraphNodeType_Mapper.BezieSpline;
                return type;
            }
        }

        protected virtual int NodeType2Index(a3dGraphNodeType_Mapper type) { 
            switch(type) {
                case a3dGraphNodeType_Mapper.Const: return 0;
                case a3dGraphNodeType_Mapper.Line: return 1;
                case a3dGraphNodeType_Mapper.BezieSpline: return 2;
            }
            return 0;
        }
        
        protected virtual GraphNode SelectedNode {
            get { return SelectedGraphInfo == null ? null : SelectedGraphInfo.Graph.SelectedNode; }
        }
        protected virtual void SetCoords(PointF p) {
            xCoor.Value = p.X;
            yCoor.Value = p.Y;
        }
        protected virtual void ClearCoords() {
            xCoor.Text = "";
            yCoor.Text = "";
        }
        private void graphViewer_MouseDown(object sender, MouseEventArgs e) {
            if (SelectedGraphInfo == null) return;
            if(e.Button == MouseButtons.Left) {
                if (Control.ModifierKeys == Keys.Control && SelectedGraphInfo != null) {
                    SelectedGraphInfo.Graph.InsertNode(Viewer.ViewPort2Point(e.Location), NodeType);
                }
                
                graphViewer.ProcessHitTest(e);
                SelectedGraphInfo.Graph.SelectedNode = graphViewer.FindActiveNode(SelectedGraphInfo.Graph);
                if (SelectedNode != null) {
                    nodeTypeCombo.SelectedIndex = NodeType2Index(SelectedGraphInfo.Graph.SelectedNode.NodeType);
                    SelectedNode.ProcessHitTest(e);
                    if (SelectedNode.PressedInfo.HitTest == GraphNodeHitTest.Point1) {
                        graphViewer.Operation = GraphOpType.MovePoint1;
                        SetCoords(SelectedNode.P1);
                    }
                    else if (SelectedNode.PressedInfo.HitTest == GraphNodeHitTest.Point2) {
                        graphViewer.Operation = GraphOpType.MovePoint2;
                        SetCoords(SelectedNode.P2);
                    }
                    else if (SelectedNode.PressedInfo.HitTest == GraphNodeHitTest.Point3) {
                        graphViewer.Operation = GraphOpType.MovePoint3;
                        SetCoords(SelectedNode.P3);
                    }
                    else if (SelectedNode.PressedInfo.HitTest == GraphNodeHitTest.Point4) {
                        graphViewer.Operation = GraphOpType.MovePoint4;
                        SetCoords(SelectedNode.P4);
                    }
                    else {
                        ClearCoords();
                    }
                }
            
                Viewer.Refresh();
            }
        }

        private void nodeTypeCombo_SelectedIndexChanged(object sender, EventArgs e) {
            if (SelectedGraphInfo == null || SelectedGraphInfo.Graph.SelectedNode == null) return;
            SelectedGraphInfo.Graph.SelectedNode.NodeType = NodeType;
            Viewer.Refresh();
        }

        private void graphViewer_MouseMove(object sender, MouseEventArgs e) {
            if (SelectedGraphInfo == null) return;
            PointF delta = Viewer.GetMoveDelta(e.Location);
            if (Viewer.Operation == GraphOpType.MovePoint1) {
                if (SelectedGraphInfo.Graph.SelectedNode != null) {
                    SelectedGraphInfo.Graph.MoveSelPoint1(delta);
                    SetCoords(SelectedGraphInfo.Graph.SelectedNode.P1);
                }
            }
            else if (Viewer.Operation == GraphOpType.MovePoint2) {
                if (SelectedGraphInfo.Graph.SelectedNode != null) {
                    SelectedGraphInfo.Graph.MoveSelPoint2(delta);
                    SetCoords(SelectedGraphInfo.Graph.SelectedNode.P2);
                }
            }
            else if (Viewer.Operation == GraphOpType.MovePoint3) {
                if (SelectedGraphInfo.Graph.SelectedNode != null) {
                    SelectedGraphInfo.Graph.MoveSelPoint3(delta);
                    SetCoords(SelectedGraphInfo.Graph.SelectedNode.P3);
                }
            }
            else if (Viewer.Operation == GraphOpType.MovePoint4) {
                if (SelectedGraphInfo.Graph.SelectedNode != null) {
                    SelectedGraphInfo.Graph.MoveSelPoint4(delta);
                    SetCoords(SelectedGraphInfo.Graph.SelectedNode.P4);
                }
            }
            if(Viewer.Operation != GraphOpType.MoveOriginPoint && Viewer.Operation != GraphOpType.None)
                graphViewer.DownPoint = e.Location;
            Viewer.Invalidate();
        }

        private void removeNodeMenuItem_Click(object sender, EventArgs e) {
            if(SelectedGraphInfo == null) return;
            SelectedGraphInfo.Graph.RemoveSelNode();
        }

        private void graphViewer_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Delete && SelectedGraphInfo != null)
                SelectedGraphInfo.Graph.RemoveSelNode();
        }

        private void breakOffNodeMenuItem_Click(object sender, EventArgs e) {
            if (SelectedNode == null) return;
            SelectedGraphInfo.Graph.SelectedNodeSplitNext(false);
        }

        private void joinNodeToNextMenuItem_Click(object sender, EventArgs e) {
            if (SelectedNode == null) return;
            SelectedGraphInfo.Graph.SelectedNodeSplitNext(true);
        }

        private void visibleCheck_CheckedChanged(object sender, EventArgs e) {
            if (SelectedGraphInfo == null) return;
            SelectedGraphInfo.Graph.Visible = visibleCheck.Checked;
        }

        protected virtual void UpdateNodeCoors() {
            if (SelectedNode.SelectedInfo.HitTest == GraphNodeHitTest.Point1) {
                SelectedNode.P1 = new PointF(xCoor.Value, yCoor.Value);
            }
            else if (SelectedNode.SelectedInfo.HitTest == GraphNodeHitTest.Point2) {
                SelectedNode.P2 = new PointF(xCoor.Value, yCoor.Value);
            }
            else if (SelectedNode.SelectedInfo.HitTest == GraphNodeHitTest.Point3) {
                SelectedNode.P3 = new PointF(xCoor.Value, yCoor.Value);
            }
            else if (SelectedNode.SelectedInfo.HitTest == GraphNodeHitTest.Point4) {
                SelectedNode.P4 = new PointF(xCoor.Value, yCoor.Value);
            }
        }

        private void xCoor_ValueChanged(object sender, EventArgs e) {
            if (SelectedNode == null) return;
            UpdateNodeCoors();
        }

        private void yCoor_ValueChanged(object sender, EventArgs e) {
            if (SelectedNode == null) return;
            UpdateNodeCoors();
        }

        private void controlPropertiesToolStripMenuItem_Click(object sender, EventArgs e) {
            using (a3dGraphPropForm form = new a3dGraphPropForm()) {
                form.propertyGrid1.SelectedObject = graphViewer;
                form.ShowDialog();
            }
        }
    }

    public class GraphInfo { 
        Graph graph;
        public GraphInfo(Graph graph) {
            this.graph = graph;
        }

        public Graph Graph { get { return graph; } }
        public override string ToString() {
            return Graph.GraphName;
        }    
    }
}
