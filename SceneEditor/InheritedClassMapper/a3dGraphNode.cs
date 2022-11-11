using System;
using System.Collections.Generic;
using System.Text;
using classMapper;
using System.Drawing;
using System.Windows.Forms;

namespace sceneEditor.InheritedClassMapper {
    public enum GraphNodeHitTest { None, Line, Point1, Point2, Point3, Point4 }

    public class GraphNodeHitInfo {
        static PointF InvalidPoint = new PointF(-100000.0f, 100000.0f);

        PointF hitPoint;
        GraphNodeHitTest hitTest;

        public GraphNodeHitInfo() {
            this.hitPoint = InvalidPoint;
            this.hitTest = GraphNodeHitTest.None;
        }

        public PointF HitPoint {
            get { return hitPoint; }
            set { hitPoint = value; }
        }

        public GraphNodeHitTest HitTest {
            get { return hitTest; }
            set { hitTest = value; }
        }
    }

    public class GraphNode : a3dGraphNodeMapper {
        GraphNodeHitInfo hotInfo;
        GraphNodeHitInfo pressedInfo;
        GraphNodeHitInfo selectedInfo;

        public GraphNode(bool createNativeNode)
            : base(createNativeNode) {
            this.hotInfo = new GraphNodeHitInfo();
            this.pressedInfo = new GraphNodeHitInfo();
            SplitNext = true;
        }

        public GraphNode() : this(true) { }

        public PointF R1 { get { return new PointF(P3.X - P1.X, P3.Y - P1.Y); } }
        public PointF R2 { get { return new PointF(P4.X - P2.X, P4.Y - P2.Y); } }

        public new Graph OwnerGraph { get { return base.OwnerGraph as Graph; } }

        public GraphNodeHitInfo HotInfo {
            get { return hotInfo; }
            set {
                GraphNodeHitTest oldHitTest = hotInfo.HitTest;
                hotInfo = value;
                if (hotInfo.HitTest != oldHitTest)
                    OnPropertiesChanged();
            }
        }

        public GraphNodeHitInfo PressedInfo {
            get { return pressedInfo; }
            set {
                GraphNodeHitTest oldHitTest = pressedInfo.HitTest;
                pressedInfo = value;
                if (pressedInfo.HitTest != oldHitTest)
                    OnPropertiesChanged();
            }
        }

        public GraphNodeHitInfo SelectedInfo {
            get { return selectedInfo; }
            set {
                GraphNodeHitTest oldHitTest = selectedInfo == null? GraphNodeHitTest.None: selectedInfo.HitTest;
                selectedInfo = value;
                if (selectedInfo.HitTest != oldHitTest)
                    OnPropertiesChanged();
            }
        }

        protected virtual void OnPropertiesChanged() {
            if (OwnerGraph != null)
                OwnerGraph.OnPropertiesChanged();
        }

        public float ProcessDeltaX(float x, float deltaX, float minX, float maxX) {
            if (x + deltaX < minX) deltaX = minX - x;
            else if (x + deltaX > maxX) deltaX = maxX - x;
            return deltaX;
        }

        public void MovePoint1(PointF delta) { MovePoint1(delta, -100000.0f, 100000.0f); }
        public void MovePoint2(PointF delta) { MovePoint2(delta, -100000.0f, 100000.0f); }
        public void MovePoint3(PointF delta) { MovePoint3(delta, -100000.0f, 100000.0f); }
        public void MovePoint4(PointF delta) { MovePoint4(delta, -100000.0f, 100000.0f); }

        public void MovePoint1(PointF delta, float minX, float maxX) {
            delta.X = ProcessDeltaX(P1.X, delta.X, minX, maxX);
            P1 = new PointF(P1.X + delta.X, P1.Y + delta.Y);
            if (NodeType == a3dGraphNodeType_Mapper.BezieSpline)
                MovePoint3(delta);
        }

        public void MovePoint2(PointF delta, float minX, float maxX) {
            delta.X = ProcessDeltaX(P2.X, delta.X, minX, maxX);
            P2 = new PointF(P2.X + delta.X, P2.Y + delta.Y);
            if (NodeType == a3dGraphNodeType_Mapper.BezieSpline)
                MovePoint4(delta);
        }

        public void MovePoint3(PointF delta, float minX, float maxX) {
            delta.X = ProcessDeltaX(P3.X, delta.X, minX, maxX);
            P3 = new PointF(P3.X + delta.X, P3.Y + delta.Y);
        }

        public void MovePoint4(PointF delta, float minX, float maxX) {
            delta.X = ProcessDeltaX(P4.X, delta.X, minX, maxX);
            P4 = new PointF(P4.X + delta.X, P4.Y + delta.Y);
        }

        public virtual float CalcNode(PointF pt) {
            return CalcNode(pt.X);
        }

        protected override void OnNodeTypeChanged() {

            float dx = (P2.X - P1.X) / 3.0f;
            float dy = (P2.Y - P1.Y) / 3.0f;
            if (NodeType == a3dGraphNodeType_Mapper.BezieSpline && (P3 == InvalidPoint || P4 == InvalidPoint)) {
                P3 = new PointF(P1.X + dx, P1.Y + dy);
                P4 = new PointF(P2.X - dx, P2.Y - dy);
            }
            OnPropertiesChanged();
        }

        protected virtual RectangleF GetPointRectangle(PointF pt) {
            float size = OwnerGraph.Viewer.NodePointSize;
            PointF vpt = OwnerGraph.Viewer.Point2ViewPort(pt);
            return new RectangleF(vpt.X - size * 0.5f, vpt.Y - size * 0.5f, size, size);
        }

        public GraphNodeHitInfo ProcessHitTestCore(MouseEventArgs e) {

            PointF pt = OwnerGraph.Viewer.ViewPort2Point(e.Location);
            PointF vpt = new PointF(e.X, e.Y);

            GraphNodeHitInfo hitInfo = new GraphNodeHitInfo();
            hitInfo.HitPoint = pt;
            if (!OwnerGraph.Visible)
                return hitInfo;

            RectangleF rect = GetPointRectangle(P3);
            hitInfo.HitTest = GraphNodeHitTest.None;
            if (rect.Contains(vpt) && NodeType == a3dGraphNodeType_Mapper.BezieSpline)
                hitInfo.HitTest = GraphNodeHitTest.Point3;
            rect = GetPointRectangle(P4);
            if (rect.Contains(vpt) && NodeType == a3dGraphNodeType_Mapper.BezieSpline && hitInfo.HitTest == GraphNodeHitTest.None)
                hitInfo.HitTest = GraphNodeHitTest.Point4;
            rect = GetPointRectangle(P1);
            if (rect.Contains(vpt) && hitInfo.HitTest == GraphNodeHitTest.None)
                hitInfo.HitTest = GraphNodeHitTest.Point1;
            rect = GetPointRectangle(P2);
            if (rect.Contains(vpt) && hitInfo.HitTest == GraphNodeHitTest.None)
                hitInfo.HitTest = GraphNodeHitTest.Point2;
 
            if (hitInfo.HitTest != GraphNodeHitTest.None)
                return hitInfo;

            else if (P1.X <= pt.X && pt.X <= P2.X) {
                float y = CalcNode(pt);
                if (Math.Abs(y - pt.Y) < ((float)5 / OwnerGraph.Viewer.ScaleY)) hitInfo.HitTest = GraphNodeHitTest.Line;
            }

            return hitInfo;
        }

        public void ProcessHitTest(MouseEventArgs e) {
            if (e.Button == MouseButtons.Left) {
                PressedInfo = ProcessHitTestCore(e);
                SelectedInfo = PressedInfo;
                HotInfo = new GraphNodeHitInfo();
            }
            else {
                HotInfo = ProcessHitTestCore(e);
                PressedInfo = new GraphNodeHitInfo();
            }
        }
    }
}
