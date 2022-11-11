using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using classMapper;
using sceneEditor.graphViewer;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.ComponentModel;

namespace sceneEditor.InheritedClassMapper {

    public class GraphDrawInfo {
        Pen normalPen;
        Pen selectedPen;
        Pen hotTrackPen;
        Pen pressedPen;
        Pen splineWeightPen;
        Pen selectedLinePen;
        Pen selectedHotTrackPen;
        Pen selectedPressedPen;

        Graph graph;
        public GraphDrawInfo(Graph graph) {
            this.graph = graph;
        }

        public Graph Graph { get { return graph; } }

        public Pen NormalLinePen {
            get {
                if (normalPen == null) {
                    normalPen = new Pen(Graph.Color, Graph.Width);
                }
                normalPen.DashStyle = Graph.Style;
                return normalPen;
            }
        }

        public Pen HotTrackLinePen {
            get {
                if (hotTrackPen == null) {
                    hotTrackPen = new Pen(Graph.OwnerColl.Viewer.HotTrackColor, Graph.Width);
                }
                hotTrackPen.DashStyle = Graph.Style;

                return hotTrackPen;
            }
        }

        public Pen PressedLinePen {
            get {
                if (pressedPen == null) {
                    pressedPen = new Pen(Graph.OwnerColl.Viewer.PressedColor, Graph.Width);
                }
                pressedPen.DashStyle = Graph.Style;

                return pressedPen;
            }
        }

        public Pen SelectedPointPen {
            get {
                if (selectedPen == null) {
                    selectedPen = new Pen(Graph.OwnerColl.Viewer.HotTrackColor, Graph.Width * 2);
                }
                selectedPen.DashStyle = Graph.Style;
                return selectedPen;
            }
        }

        public Pen SelectedLinePen {
            get {
                if (selectedPen == null) {
                    selectedPen = new Pen(Graph.Color, Graph.Width * 2);
                }
                selectedPen.DashStyle = Graph.Style;
                return selectedPen;
            }
        }

        public Pen SelectedHotTrackPen {
            get {
                if (selectedHotTrackPen == null) {
                    selectedHotTrackPen = new Pen(Graph.OwnerColl.Viewer.HotTrackColor, Graph.Width * 2);
                }
                selectedHotTrackPen.DashStyle = Graph.Style;
                return selectedHotTrackPen;
            }
        }

        public Pen SelectedPressedPen {
            get {
                if (selectedPressedPen == null) {
                    selectedPressedPen = new Pen(Graph.OwnerColl.Viewer.PressedColor, Graph.Width * 2);
                }
                selectedPressedPen.DashStyle = Graph.Style;
                return selectedPressedPen;
            }
        }

        public Pen SplineWeightPen {
            get {
                if (splineWeightPen == null) {
                    splineWeightPen = new Pen(Graph.OwnerColl.Viewer.SplineWeightColor, 1);
                }
                return splineWeightPen;
            }
        }

        public Pen SelectedSplineWeightPen {
            get {
                if (splineWeightPen == null) {
                    splineWeightPen = new Pen(Graph.OwnerColl.Viewer.SplineWeightColor, Graph.Width * 2);
                }
                return splineWeightPen;
            }
        }

        public Pen NormalPen {
            get {
                NormalLinePen.DashStyle = DashStyle.Solid;
                return NormalLinePen;
            }
        }

        public Pen HotTrackPen {
            get {
                HotTrackLinePen.DashStyle = DashStyle.Solid;
                return HotTrackLinePen;
            }
        }

        public Pen PressedPen {
            get {
                PressedLinePen.DashStyle = DashStyle.Solid;
                return PressedLinePen;
            }
        }
    }

    public class Graph : a3dGraphMapper {
        GraphCollection ownerColl = null;
        Color color;
        DashStyle style;
        float width;
        bool visible;
        GraphDrawInfo drawInfo;
        bool showAuxillaryElements;
        public Graph(bool createNativeGraph)
            : base(createNativeGraph) {
            this.color = Color.Black;
            this.style = DashStyle.Solid;
            this.width = 1.0f;
            this.visible = true;
            this.drawInfo = null;
            this.showAuxillaryElements = true;
        }
        public Graph() : this(true) { }

        protected override a3dGraphNodeMapper CreateNode() {
            return new GraphNode(false);
        }

        protected bool ShouldSerializeColor() { return Color != Color.Black; }
        public Color Color {
            get { return color; }
            set {
                if (Color == value) return;
                color = value;
                OnColorPropertiesChanged();
                OnPropertiesChanged();
            }
        }

        [DefaultValue(true)]
        public bool Visible {
            get { return visible; }
            set {
                if (Visible == value) return;
                visible = value;
                OnPropertiesChanged();
            }
        }

        [DefaultValue(true)]
        public bool ShowAuxillaryElements {
            get { return showAuxillaryElements; }
            set {
                if (ShowAuxillaryElements == value) return;
                showAuxillaryElements = value;
                OnPropertiesChanged();
            }
        }

        [DefaultValue(1.0f)]
        public float Width {
            get { return width; }
            set {
                if (Width == value) return;
                width = value;
                OnColorPropertiesChanged();
                OnPropertiesChanged();
            }
        }

        [DefaultValue(DashStyle.Solid)]
        public DashStyle Style {
            get { return style; }
            set {
                if (Style == value) return;
                style = value;
                OnColorPropertiesChanged();
                OnPropertiesChanged();
            }
        }

        public GraphCollection OwnerColl {
            get { return ownerColl; }
            set {
                if (OwnerColl == value) return;
                ownerColl = value;
                OnPropertiesChanged();
            }
        }

        GraphNode selectedNode = null;
        public GraphNode SelectedNode {
            get { return selectedNode; }
            set {
                if (SelectedNode == value) return;
                selectedNode = value;
                OnPropertiesChanged();
            }
        }

        protected GraphDrawInfo DrawInfo {
            get {
                if (drawInfo == null)
                    drawInfo = new GraphDrawInfo(this);
                return drawInfo;
            }
        }

        protected internal virtual void OnColorPropertiesChanged() {
            this.drawInfo = null;
        }

        public override void OnPropertiesChanged() {
            if (OwnerColl != null)
                OwnerColl.OnPropertiesChanged();
        }

        protected virtual void UpdateConstNode(int index, GraphNode node) {
            // do nothing
        }

        protected virtual void UpdateLineNode(int index, GraphNode node) {
            float k = (node.P2.X - node.P1.X);
            if (k == 0) {
                node.K = 0.0f;
                return;
            }
            node.K = (node.P2.Y - node.P1.Y) / k;
        }

        protected virtual void UpdateSinNode(int index, GraphNode node) {
            // do nothing
        }

        protected virtual void UpdateSplineNode(int index, GraphNode node) {
            // do nothing

        }

        protected virtual void UpdateGraphNodes() {
            for (int i = 0; i < Nodes.Count; i++) {
                switch (Nodes[i].NodeType) {
                    case a3dGraphNodeType_Mapper.Const:
                        UpdateConstNode(i, Nodes[i] as GraphNode);
                        break;
                    case a3dGraphNodeType_Mapper.Line:
                        UpdateLineNode(i, Nodes[i] as GraphNode);
                        break;
                    case a3dGraphNodeType_Mapper.BezieSpline:
                        UpdateSplineNode(i, Nodes[i] as GraphNode);
                        break;
                }
            }
        }

        protected float GetP1MinX(int nodeIndex) {
            return nodeIndex > 0 ? Nodes[nodeIndex - 1].P1.X + 0.00001f : -100000.0f;
        }

        protected float GetP1MaxX(int nodeIndex) {
            return Nodes[nodeIndex].P2.X - 0.000001f;
        }

        protected float GetP2MinX(int nodeIndex) {
            return Nodes[nodeIndex].P1.X + 0.000001f;
        }

        protected float GetP2MaxX(int nodeIndex) {
            return nodeIndex < Nodes.Count - 1 ? Nodes[nodeIndex + 1].P2.X - 0.000001f : 100000.0f;
        }

        public void RemoveSelNode() {
            if (SelectedNode == null) return;
            int index = Nodes.IndexOf(SelectedNode);
            Nodes.Remove(SelectedNode);
            selectedNode = null;
            if (index == Nodes.Count) {
                SelectedNode = Nodes[index - 1] as GraphNode;
                OnPropertiesChanged();
                return;
            }
            if (index < Nodes.Count)
                SelectedNode = Nodes[index] as GraphNode;
            if (Nodes.Count < 2 || index == 0) {
                OnPropertiesChanged();
                return;
            }
            if (Nodes[index - 1].SplitNext)
                Nodes[index].P1 = Nodes[index - 1].P2;
            else
                Nodes[index].P1 = new PointF(Nodes[index - 1].P2.X, Nodes[index].P1.Y);
            OnPropertiesChanged();
        }

        public void SelectedNodeSplitNext(bool value) {
            if (SelectedNode == null) return;
            SelectedNode.SplitNext = value;
            if (value == false) return;
            int index = Nodes.IndexOf(SelectedNode);
            if (index == Nodes.Count - 1) return;
            GraphNode nn = Nodes[index + 1] as GraphNode;
            MoveSelPoint2(new PointF(0, nn.P1.Y - SelectedNode.P2.Y));
        }

        public void MoveSelPoint1(PointF delta) {
            if (SelectedNode == null) return;
            int index = Nodes.IndexOf(SelectedNode);
            SelectedNode.MovePoint1(delta, GetP1MinX(index), GetP1MaxX(index));
            if (index == 0) return;
            GraphNode pn = Nodes[index - 1] as GraphNode;
            if (pn.SplitNext)
                pn.MovePoint2(new PointF(SelectedNode.P1.X - pn.P2.X, SelectedNode.P1.Y - pn.P2.Y));
            else
                pn.MovePoint2(new PointF(SelectedNode.P1.X - pn.P2.X, 0));
        }

        public void MoveSelPoint2(PointF delta) {
            if (SelectedNode == null) return;
            int index = Nodes.IndexOf(SelectedNode);
            SelectedNode.MovePoint2(delta, GetP2MinX(index), GetP2MaxX(index));
            if (index == Nodes.Count - 1) return;
            GraphNode nn = Nodes[index + 1] as GraphNode;
            if (SelectedNode.SplitNext)
                nn.MovePoint1(new PointF(SelectedNode.P2.X - nn.P1.X, SelectedNode.P2.Y - nn.P1.Y));
            else
                nn.MovePoint1(new PointF(SelectedNode.P2.X - nn.P1.X, 0));
        }

        public void MoveSelPoint3(PointF delta) {
            if (SelectedNode == null) return;
            int index = Nodes.IndexOf(SelectedNode);
            SelectedNode.MovePoint3(delta, -100000.0f, 100000.0f);
            //if (index == 0 || !Nodes[index-1].SplitNext) return;
            //GraphNode prevNode = Nodes[index - 1];
            //PointF dt = SelectedNode.R1;
            //PointF dt2 = prevNode.R2;
            //float len = (float)Math.Sqrt(dt.X * dt.X + dt.Y * dt.Y);
            //float len2 = (float)Math.Sqrt(dt2.X * dt2.X + dt2.Y * dt2.Y);
            //prevNode.P4 = new PointF(prevNode.P2.X - dt.X / len * len2, prevNode.P2.Y - dt.Y / len * len2);
        }

        public void MoveSelPoint4(PointF delta) {
            if (SelectedNode == null) return;
            int index = Nodes.IndexOf(SelectedNode);
            SelectedNode.MovePoint4(delta, -100000.0f, 100000.0f);
            //if (index >= Nodes.Count - 1 || !Nodes[index].SplitNext) return;
            //GraphNode nextNode = Nodes[index + 1];
            //PointF dt = SelectedNode.R2;
            //PointF dt2 = nextNode.R1;
            //float len = (float)Math.Sqrt(dt.X * dt.X + dt.Y * dt.Y);
            //float len2 = (float)Math.Sqrt(dt2.X * dt2.X + dt2.Y * dt2.Y);
            //nextNode.P3 = new PointF(nextNode.P1.X - dt.X / len * len2, nextNode.P1.Y - dt.Y / len * len2);
        }

        protected virtual bool Contains(PointF scrPoint1, PointF scrPoint2, GraphViewer view) {
            return !((scrPoint1.Y < 0.0f && scrPoint2.Y < 0.0f) || (scrPoint1.Y > view.ClientRectangle.Height && scrPoint2.Y > view.ClientRectangle.Height));
        }
        bool IsNodeLineSelected(GraphNode node) {
            return node.SelectedInfo != null && node.SelectedInfo.HitTest == GraphNodeHitTest.Line;
        }
        protected virtual Pen GetLinePen(GraphNode node) {
            if (node.PressedInfo.HitTest == GraphNodeHitTest.Line) {
                if (IsNodeLineSelected(node)) return DrawInfo.SelectedHotTrackPen;
                return DrawInfo.PressedLinePen;
            }
            if (node.HotInfo.HitTest == GraphNodeHitTest.Line) {
                if (IsNodeLineSelected(node)) return DrawInfo.SelectedPressedPen;
                return DrawInfo.HotTrackLinePen;
            }
            if (IsNodeLineSelected(node)) return DrawInfo.SelectedLinePen;
            return DrawInfo.NormalLinePen;
        }
        bool IsNodeP1Selected(GraphNode node) {
            return node.SelectedInfo != null && node.SelectedInfo.HitTest == GraphNodeHitTest.Point1;
        }
        bool IsNodeP2Selected(GraphNode node) {
            return node.SelectedInfo != null && node.SelectedInfo.HitTest == GraphNodeHitTest.Point2;
        }
        bool IsNodeP3Selected(GraphNode node) {
            return node.SelectedInfo != null && node.SelectedInfo.HitTest == GraphNodeHitTest.Point3;
        }
        bool IsNodeP4Selected(GraphNode node) {
            return node.SelectedInfo != null && node.SelectedInfo.HitTest == GraphNodeHitTest.Point4;
        }
        protected virtual Pen GetPoint1Pen(GraphNode node) {
            if (node.PressedInfo.HitTest == GraphNodeHitTest.Point1) return DrawInfo.PressedPen;
            if (node.HotInfo.HitTest == GraphNodeHitTest.Point1) return DrawInfo.HotTrackPen;
            if (IsNodeP1Selected(node)) return DrawInfo.SelectedPointPen;
            return DrawInfo.NormalPen;
        }
        protected virtual Pen GetPoint2Pen(GraphNode node) {
            if (node.PressedInfo.HitTest == GraphNodeHitTest.Point2) return DrawInfo.PressedPen;
            if (node.HotInfo.HitTest == GraphNodeHitTest.Point2) return DrawInfo.HotTrackPen;
            if (IsNodeP2Selected(node)) return DrawInfo.SelectedPointPen;
            return DrawInfo.NormalPen;
        }

        protected virtual Pen GetPoint3Pen(GraphNode node) {
            if (node.PressedInfo.HitTest == GraphNodeHitTest.Point3) return DrawInfo.PressedPen;
            if (node.HotInfo.HitTest == GraphNodeHitTest.Point3) return DrawInfo.HotTrackPen;
            if (IsNodeP3Selected(node)) return DrawInfo.SelectedPointPen;
            return DrawInfo.NormalPen;
        }

        protected virtual Pen GetPoint4Pen(GraphNode node) {
            if (node.PressedInfo.HitTest == GraphNodeHitTest.Point4) return DrawInfo.PressedPen;
            if (node.HotInfo.HitTest == GraphNodeHitTest.Point4) return DrawInfo.HotTrackPen;
            if (IsNodeP4Selected(node)) return DrawInfo.SelectedPointPen;
            return DrawInfo.NormalPen;
        }

        protected virtual Pen GetSplineWeightPen3(GraphNode node) {
            if (node.PressedInfo.HitTest == GraphNodeHitTest.Point3) return DrawInfo.PressedPen;
            if (node.HotInfo.HitTest == GraphNodeHitTest.Point3) return DrawInfo.HotTrackPen;
            if (IsNodeP3Selected(node)) return DrawInfo.SelectedSplineWeightPen;
            return DrawInfo.SplineWeightPen;
        }

        protected virtual Pen GetSplineWeightPen4(GraphNode node) {
            if (node.PressedInfo.HitTest == GraphNodeHitTest.Point4) return DrawInfo.PressedPen;
            if (node.HotInfo.HitTest == GraphNodeHitTest.Point4) return DrawInfo.HotTrackPen;
            if (IsNodeP4Selected(node)) return DrawInfo.SelectedSplineWeightPen;
            return DrawInfo.SplineWeightPen;
        }

        protected virtual void DrawPoint(PaintEventArgs e, PointF pt, Pen pen, GraphViewer view) {
            e.Graphics.DrawRectangle(pen, new Rectangle((int)(pt.X - view.NodePointSize / 2), (int)(pt.Y - view.NodePointSize / 2), view.NodePointSize, view.NodePointSize));
        }

        protected virtual void DrawNodePoints(PaintEventArgs e, GraphNode node, GraphViewer view) {
            PointF pt1 = view.Point2ViewPort(node.P1), pt2 = view.Point2ViewPort(node.P2);
            UpdateScrPoints(node, ref pt1, ref pt2, view);
            DrawPoint(e, pt1, GetPoint1Pen(node), view);
            DrawPoint(e, pt2, GetPoint2Pen(node), view);
        }

        protected virtual void DrawConstNode(PaintEventArgs e, GraphNode node, GraphViewer view) {
            PointF scrPoint1 = view.Point2ViewPort(node.P1);
            PointF scrPoint2 = view.Point2ViewPort(new PointF(node.P2.X, node.P1.Y));

            if (!Contains(scrPoint1, scrPoint2, view)) return;
            e.Graphics.DrawLine(GetLinePen(node), scrPoint1, scrPoint2);
        }

        protected virtual void UpdateScrPoints(GraphNode node, ref PointF p1, ref PointF p2, GraphViewer view) {
            if (node.NodeType != a3dGraphNodeType_Mapper.Bool) {
                if (node.NodeType == a3dGraphNodeType_Mapper.Const) p2 = view.Point2ViewPort(new PointF(node.P2.X, node.P1.Y));
                return;
            }
            if(node.BResult)
                p1.Y = view.ViewPortBounds.Y + view.ViewPortBounds.Height / 2;
            else
                p1.Y = view.ViewPortBounds.Bottom;
            p2.Y = p1.Y;
        }

        protected virtual void DrawBoolNode(PaintEventArgs e, GraphNode node, GraphViewer view) {
            PointF scrPoint1 = view.Point2ViewPort(node.P1);
            PointF scrPoint2 = view.Point2ViewPort(node.P2);

            UpdateScrPoints(node, ref scrPoint1, ref scrPoint2, view);
            e.Graphics.DrawLine(GetLinePen(node), scrPoint1, scrPoint2);
        }

        protected virtual void DrawLineNode(PaintEventArgs e, GraphNode node, GraphViewer view) {
            PointF scrPoint1 = view.Point2ViewPort(node.P1);
            PointF scrPoint2 = view.Point2ViewPort(node.P2);

            if (!Contains(scrPoint1, scrPoint2, view)) return;
            e.Graphics.DrawLine(GetLinePen(node), scrPoint1, scrPoint2);
        }

        protected virtual void DrawBezieSplineNode(PaintEventArgs e, GraphNode node, GraphViewer view) {

            PointF scrPoint1 = view.Point2ViewPort(node.P1);
            PointF scrPoint2 = view.Point2ViewPort(node.P2);
            if (!Contains(scrPoint1, scrPoint2, view)) return;

            PointF pt3 = view.Point2ViewPort(node.P3), pt4 = view.Point2ViewPort(node.P4);

            if (ShowAuxillaryElements) {
                DrawPoint(e, pt3, GetPoint3Pen(node), view);
                DrawPoint(e, pt4, GetPoint4Pen(node), view);
            }

            Pen pen = GetLinePen(node);

            PointF delta = view.ViewPort2Point(new Point(1, 1));
            PointF d2 = view.ViewPort2Point(new Point(0, 0));
            delta.X -= d2.X;
            delta.Y -= d2.Y;

            PointF prevPoint = scrPoint1, newPoint = Point.Empty, scrPoint = Point.Empty;
            for (newPoint.X = node.P1.X; newPoint.X <= node.P2.X; newPoint.X += delta.X) {
                newPoint.Y = node.CalcNode(newPoint.X);
                scrPoint = view.Point2ViewPort(newPoint);
                e.Graphics.DrawLine(pen, prevPoint, scrPoint);
                prevPoint = scrPoint;
            }

            if (ShowAuxillaryElements) {
                e.Graphics.DrawLine(GetSplineWeightPen3(node), scrPoint1, pt3);
                e.Graphics.DrawLine(GetSplineWeightPen4(node), scrPoint2, pt4);
            }
        }

        protected virtual void DrawNode(PaintEventArgs e, GraphNode node, GraphViewer view) {

            float scrPoint1 = view.X2ViewPort(node.P1.X);
            float scrPoint2 = view.X2ViewPort(node.P2.X);

            if (scrPoint2 < 0.0f || scrPoint1 > view.ClientRectangle.Width) return;

            if (ShowAuxillaryElements)
                DrawNodePoints(e, node, view);

            if (Type == GraphType.Bool) {
                DrawBoolNode(e, node, view);
                return;
            }

            switch (node.NodeType) {
                case a3dGraphNodeType_Mapper.Const:
                    DrawConstNode(e, node, view);
                    break;
                case a3dGraphNodeType_Mapper.Line:
                    DrawLineNode(e, node, view);
                    break;
                case a3dGraphNodeType_Mapper.BezieSpline:
                    DrawBezieSplineNode(e, node, view);
                    break;
            }
        }

        public virtual void Draw(PaintEventArgs e, GraphViewer view) {
            if (!Visible) return;
            foreach (GraphNode node in Nodes) {
                DrawNode(e, node, view);
            }
        }

        public void SelectNode(PointF pt) {
            int index = FindNodeByPoint(pt);
            if (index == -1)
                SelectedNode = null;
            else SelectedNode = Nodes[index] as GraphNode;
        }

        protected virtual int FindNodeByPoint(PointF pt) {
            for (int i = 0; i < Nodes.Count; i++) {
                if (Nodes[i].P1.X <= pt.X && pt.X <= Nodes[i].P2.X) {
                    float y = Nodes[i].CalcNode(pt.X);
                    if (Math.Abs(y - pt.Y) < 5) return i;
                    return -1;
                }
            }
            return -1;
        }

        protected virtual int FindNodeIndexByX(float x) {
            if (Nodes.Count == 0) return 0;
            if (Nodes[0].P1.X > x) return -1;
            for (int i = 0; i < Nodes.Count; i++) {
                if (Nodes[i].P1.X <= x && x <= Nodes[i].P2.X) return i;
            }
            return Nodes.Count;
        }

        public void InsertNode(PointF pt, a3dGraphNodeType_Mapper nodeType) {
            int index = FindNodeIndexByX(pt.X);

            GraphNode node = new GraphNode();
            if (Type == GraphType.Bool)
                node.NodeType = a3dGraphNodeType_Mapper.Bool;
            else
                node.NodeType = nodeType;

            if (index >= 0 && index < Nodes.Count) {
                node.P1 = Nodes[index].P1;
                node.P2 = pt;
                Nodes[index].P1 = pt;
            }
            else if (index == Nodes.Count) {
                node.P2 = pt;
                if (Nodes.Count > 0)
                    node.P1 = Nodes[Nodes.Count - 1].P2;
                else
                    node.P1 = new PointF(0.0f, 0.0f);
            }
            else if (index < 0) {
                node.P1 = pt;
                node.P2 = Nodes[0].P1;
            }

            Nodes.Insert(index, node);
            // todo Hodler not initialized OnInsertComplete not called
            InitializeBezieSplineWeight(index);
        }

        protected virtual void InitializeBezieSplineWeight(int index) {
            if (Nodes[index].NodeType == a3dGraphNodeType_Mapper.BezieSpline) {
                PointF delta = PointF.Empty;
                if (index == 0 || !Nodes[index - 1].SplitNext) {
                    delta = new PointF((Nodes[index].P2.X - Nodes[index].P1.X) / 3, (Nodes[index].P2.Y - Nodes[index].P1.Y) / 3);
                    Nodes[index].P3 = new PointF(Nodes[index].P1.X + delta.X, Nodes[index].P1.X + delta.Y);
                }
                else {
                    delta = new PointF(Nodes[index - 1].P2.X - Nodes[index - 1].P4.X, Nodes[index - 1].P2.Y - Nodes[index - 1].P4.Y);
                    Nodes[index].P3 = new PointF(Nodes[index].P1.X + delta.X, Nodes[index].P1.Y + delta.Y);
                }
                if (index == Nodes.Count - 1) {
                    delta = new PointF((Nodes[index].P2.X - Nodes[index].P1.X) / 3, (Nodes[index].P2.Y - Nodes[index].P1.Y) / 3);
                    Nodes[index].P4 = new PointF(Nodes[index].P2.X - delta.X, Nodes[index].P2.Y - delta.Y);
                }
                else {
                    delta = new PointF(Nodes[index + 1].P2.X - Nodes[index + 1].P4.X, Nodes[index + 1].P2.Y - Nodes[index + 1].P4.Y);
                    Nodes[index].P4 = new PointF(Nodes[index].P2.X + delta.X, Nodes[index].P2.Y + delta.Y); ;
                }
            }
        }

        public GraphViewer Viewer {
            get { return OwnerColl == null ? null : OwnerColl.Viewer; }
        }

        public void ProcessHitTest(MouseEventArgs e) {
            foreach (GraphNode node in Nodes) {
                node.ProcessHitTest(e);
            }
        }
    }

}
