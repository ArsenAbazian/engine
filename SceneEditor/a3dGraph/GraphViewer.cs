using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Collections;
using classMapper;
using sceneEditor.InheritedClassMapper;
using sceneEditor.Configuration;

namespace sceneEditor.graphViewer {
    
    public enum GraphOpType { None, MoveOriginPoint, MovePoint1, MovePoint2, MovePoint3, MovePoint4 }
        
    [CustomXmlSerializable(true)]
    public class GraphViewer : Control {
        private static readonly object propertiesChanged = new object();
        public static Point InvalidDownPoint = new Point(-10000, -10000);

        static Color DefaultZoneColor1 = Color.FromArgb(225, 237, 251);
        static Color DefaultZoneColor2 = Color.FromArgb(251, 233, 225);
        static Color DefaultHotTrackColor = Color.Orange;
        static Color DefaultPressedColor = Color.Red;
        static Color DefaultSplineWeightColor = Color.Green;

        Padding viewportPadding;
        PointF originPoint, defaultOriginPoint;
        Color axisColor;
        Color gridColor;
        Color labelColor;
        DashStyle gridLineStyle; 
        float scaleX = 1.0f;
        float scaleY = 1.0f;
        float tick = 10.0f;
        int minGridVisibilityIndent = 2;
        int tickLength = 5;
        bool drawLabels = true;
        bool drawGrid = true;
        Font labelFont;
        float scalesIncrement = 0.1f;
        float scalesIncrementDecimal = 0.01f;
        GraphCollection graphs;
        Color zoneColor1, zoneColor2;
        int nodePointSize = 8;
        Color splineWeightColor = DefaultSplineWeightColor;
        Color hotTrackColor = DefaultHotTrackColor;
        Color pressedColor = DefaultPressedColor;
        GraphOpType opType = GraphOpType.None;

        public GraphViewer () {
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.UserMouse, true);
            BackColor = Color.White;
            DoubleBuffered = true;
            this.viewportPadding = new Padding(32, 8, 8, 24);
            this.axisColor = Color.DarkGray;
            this.gridColor = Color.LightGray;
            this.labelColor = Color.DarkGray;
            this.gridLineStyle = DashStyle.Dash;
            this.labelFont = new Font("Arial", 7.0f);
            this.defaultOriginPoint = new PointF(0.0f, 0.0f);
            this.graphs = new GraphCollection(this);
            this.zoneColor1 = DefaultZoneColor1;
            this.zoneColor2 = DefaultZoneColor2;
        }
        
        protected override System.Drawing.Size  DefaultSize {
	        get { return new Size(320, 96); }
        }
        [CustomXmlSerializableProperty(true)]
        public Padding ViewPortPadding {
            get { return viewportPadding; }
            set {
                if (ViewPortPadding == value) return;
                viewportPadding = value;
                OnPropertiesChanged();
            }
        }

        protected virtual bool ShouldSerializeViewPortPadding() { return ViewPortPadding != new Padding(32, 8, 8, 24); }
        protected virtual bool ShouldSerializeZoneColor1() { return ZoneColor1 !=DefaultZoneColor1; }
        protected virtual bool ShouldSerializeZoneColor2() { return ZoneColor1 != DefaultZoneColor2; }
        protected virtual bool ShouldSerializeSplineWeightColor() { return SplineWeightColor != DefaultSplineWeightColor; }
        protected virtual bool ShouldSerializeHotTrackColor() { return SplineWeightColor != DefaultHotTrackColor; }
        protected virtual bool ShouldSerializePressedColor() { return SplineWeightColor != DefaultPressedColor; }

        protected override void OnSizeChanged(EventArgs e) {
            base.OnSizeChanged(e);
        }

        [CustomXmlSerializableProperty(true)]
        public Color HotTrackColor {
            get { return hotTrackColor; }
            set {
                if (HotTrackColor == value) return;
                hotTrackColor = value;
                OnColorPropertiesChanged();
                OnPropertiesChanged();
            }
        }

        [CustomXmlSerializableProperty(true)]
        public Color PressedColor {
            get { return pressedColor; }
            set {
                if (PressedColor == value) return;
                pressedColor = value;
                OnColorPropertiesChanged();
                OnPropertiesChanged();
            }
        }

        [CustomXmlSerializableProperty(true)]
        public Color SplineWeightColor {
            get { return splineWeightColor; }
            set {
                if (SplineWeightColor == value) return;
                splineWeightColor = value;
                OnColorPropertiesChanged();
                OnPropertiesChanged();
            }
        }

        [CustomXmlSerializableProperty(true)]
        public Color ZoneColor1 {
            get { return zoneColor1; }
            set {
                if (ZoneColor1 == value) return;
                zoneColor1 = value;
                OnPropertiesChanged();
            }
        }

        [CustomXmlSerializableProperty(true)]
        public Color ZoneColor2 {
            get { return zoneColor2; }
            set {
                if (ZoneColor2 == value) return;
                zoneColor2 = value;
                OnPropertiesChanged();
            }
        }

        [DefaultValue(8), CustomXmlSerializableProperty(true)]
        public int NodePointSize {
            get { return nodePointSize; }
            set {
                if (NodePointSize == value) return;
                nodePointSize = value;
                OnPropertiesChanged();
            }
        }

        [Browsable(false)]
        public Rectangle ViewPortBounds { 
            get {
                Rectangle r = ClientRectangle;
                r.X += ViewPortPadding.Left;
                r.Y += ViewPortPadding.Top;
                r.Width -= ViewPortPadding.Horizontal;
                r.Height -= ViewPortPadding.Vertical;
                return r;
            }
        }

        [Browsable(false)]
        public PointF OriginPoint {
            get { return originPoint; }
            set {
                if (OriginPoint == value) return;
                originPoint = value;
                OnPropertiesChanged();
            }
        }

        protected internal GraphOpType Operation {
            get { return opType; }
            set {
                if (Operation == value) return;
                opType = value;
                OnPropertiesChanged();
            }
        }

        [CustomXmlSerializableProperty(true)]
        public PointF DefaultOriginPoint {
            get { return defaultOriginPoint; }
            set {
                if (DefaultOriginPoint == value) return;
                defaultOriginPoint = value;
                OnPropertiesChanged();
            }
        }

        [DefaultValue(1.0f), CustomXmlSerializableProperty(true)]
        public float ScaleX {
            get { return scaleX; }
            set {
                if (ScaleX == value) return;
                scaleX = value;
                OnPropertiesChanged();
            }
        }

        [DefaultValue(1.0f), CustomXmlSerializableProperty(true)]
        public float ScaleY {
            get { return scaleY; }
            set {
                if (ScaleY == value) return;
                scaleY = value;
                OnPropertiesChanged();
            }
        }

        [DefaultValue(10.0f), CustomXmlSerializableProperty(true)]
        public float Tick {
            get { return tick; }
            set {
                if (Tick == value) return;
                tick = value;
                OnPropertiesChanged();
            }
        }

        [DefaultValue(true), CustomXmlSerializableProperty(true)]
        public bool DrawGrid { 
            get { return drawGrid; }
            set {
                if (DrawGrid == value) return;
                drawGrid = value;
                OnPropertiesChanged();
            }
        }

        [DefaultValue(true), CustomXmlSerializableProperty(true)]
        public bool DrawLabels {
            get { return drawLabels; }
            set {
                if (DrawLabels == value) return;
                drawLabels = value;
                OnPropertiesChanged();
            }
        }

        [DefaultValue(0.1f), CustomXmlSerializableProperty(true)]
        public float ScalesIncrement {
            get { return scalesIncrement; }
            set { scalesIncrement = value; }
        }
        [DefaultValue(0.01f), CustomXmlSerializableProperty(true)]
        public float ScalesIncrementDecimal {
            get { return scalesIncrementDecimal; }
            set { scalesIncrementDecimal = value; }
        }

        public Font LabelFont {
            get { return labelFont; }
            set {
                if (LabelFont == value) return;
                labelFont = value;
                OnPropertiesChanged();
            }
        }

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        public GraphCollection Graphs { get { return graphs; } }

        [Browsable(false)]
        public SizeF GraphWindowSize { 
            get { return new SizeF(ViewPortBounds.Width / ScaleX, ViewPortBounds.Height / ScaleY); }
        }

        [CustomXmlSerializableProperty(true)]
        public Color AxisColor {
            get { return axisColor; }
            set {
                if (AxisColor == value) return;
                axisColor = value;
                OnPropertiesChanged();
            }
        }

        [CustomXmlSerializableProperty(true)]
        public Color GridColor {
            get { return gridColor; }
            set {
                if (GridColor == value) return;
                gridColor = value;
                OnPropertiesChanged();
            }
        }

        [CustomXmlSerializableProperty(true)]
        public Color LabelColor {
            get { return labelColor; }
            set {
                if (LabelColor == value) return;
                labelColor = value;
                OnPropertiesChanged();
            }
        }

        [CustomXmlSerializableProperty(true)]
        public DashStyle GridLineStyle {
            get { return gridLineStyle; }
            set {
                if (GridLineStyle == value) return;
                gridLineStyle = value;
                OnPropertiesChanged();
            }
        }

        [DefaultValue(2), CustomXmlSerializableProperty(true)]
        public int MinGridVisibilityIndent {
            get { return minGridVisibilityIndent; }
            set {
                if (MinGridVisibilityIndent == value) return;
                minGridVisibilityIndent = value;
                OnPropertiesChanged();

            }
        }

        [DefaultValue(2), CustomXmlSerializableProperty(true)]
        public int TickLength {
            get { return tickLength; }
            set {
                if (TickLength == value) return;
                tickLength = value;
                OnPropertiesChanged();
            }
        }

        protected virtual void OnPropertiesChanged() {
            RaisePropertiesChanged();
            Invalidate();
        }

        protected virtual void OnColorPropertiesChanged() {
            foreach(Graph graph in Graphs) {
                graph.OnColorPropertiesChanged();
            }
            Invalidate();
        }

        protected virtual void RaisePropertiesChanged() {
            EventHandler handler = Events[propertiesChanged] as EventHandler;
            if (handler != null) handler(this, EventArgs.Empty);
        }

        public event EventHandler PropertiesChanged {
            add { Events.AddHandler(propertiesChanged, value); }
            remove { Events.RemoveHandler(propertiesChanged, value); }
        }

        protected virtual void DrawAxis(PaintEventArgs e) {
            Pen pen = new Pen(AxisColor);
            Rectangle rect = ViewPortBounds;
            e.Graphics.DrawLine(pen, rect.Location, new Point(rect.X, rect.Bottom));
            e.Graphics.DrawLine(pen, new Point(rect.X, rect.Bottom), new Point(rect.Right, rect.Bottom));
        }

        [Browsable(false)]
        public PointF ViewPortOrigin {
            get { return new PointF(ViewPortBounds.X, ViewPortBounds.Bottom); }
        }

        protected internal virtual float Y2ViewPort(float y) {
            return ViewPortBounds.Bottom - (y - OriginPoint.Y) * ScaleY;
        }

        public virtual float X2ViewPort(float x) {
            return ViewPortBounds.X + (x - OriginPoint.X) * ScaleX;
        }

        public virtual PointF Point2ViewPort(PointF pt) {
            return new PointF(X2ViewPort(pt.X), Y2ViewPort(pt.Y));
        }

        public virtual PointF ViewPort2Point(Point pt) {
            return new PointF(OriginPoint.X + (pt.X - ViewPortBounds.X) / ScaleX, OriginPoint.Y + (ViewPortBounds.Bottom - pt.Y) / ScaleY);
        }

        protected virtual float GetBeginValue(float val, float step) {
            float v = val / step;
            float res = (float)Math.Floor(v);
            if (Math.Abs(v - res) > 0.00001)
                res += 1.0f;
            return res * step;
        }

        protected virtual float GetYLen(float len) {
            return len / ScaleY;
        }

        protected virtual float GetXLen(float len) {
            return len / ScaleX;
        }

        protected virtual float YLen2Viewport(float len) {
            return len * ScaleY;
        }

        protected virtual float XLen2Viewport(float len) {
            return len * ScaleX;
        }

        protected virtual void DrawGridAndLabels(PaintEventArgs e) {
            Pen pen = new Pen(GridColor);
            pen.DashStyle = GridLineStyle;
            SolidBrush labelBrush = new SolidBrush(LabelColor);
            PointF pt1 = PointF.Empty, pt2 = PointF.Empty;
            RectangleF prevStringArea = RectangleF.Empty, stringArea = RectangleF.Empty;
            PointF lastGridPoint = new PointF(-100000.0f, -100000.0f);
            float beginValue;

            if ((int)YLen2Viewport(Tick) > MinGridVisibilityIndent) {
                beginValue = GetBeginValue(OriginPoint.Y, Tick);
                float ySize = beginValue + GraphWindowSize.Height;
                for (float i = beginValue; i < ySize; i += Tick) {
                    pt1.Y = Y2ViewPort(i);
                    pt1.X = (float)(ViewPortBounds.X - TickLength);
                    pt2.X = (float)ViewPortBounds.Right;
                    pt2.Y = pt1.Y;
                    if (DrawGrid) {
                        if (Math.Abs(pt1.Y - lastGridPoint.Y) >= MinGridVisibilityIndent) {
                            e.Graphics.DrawLine(pen, pt1, pt2);
                            lastGridPoint = pt1;
                        }
                    }
                    if (DrawLabels) {
                        stringArea.Size = e.Graphics.MeasureString(i.ToString(), LabelFont);
                        stringArea.Location = pt1;
                        stringArea.X -= stringArea.Width;
                        if (stringArea.IntersectsWith(prevStringArea)) continue;
                        e.Graphics.DrawString(i.ToString(), LabelFont, labelBrush, stringArea.Location);
                        prevStringArea = stringArea;
                    }
                }
            }
            
            //// draw horizontal
            //step = Tick * ScaleX;
            //if (step < MinGridVisibilityIndent)
            //    step = (float)Math.Floor(MinGridVisibilityIndent / ScaleX);

            if ((int)XLen2Viewport(Tick) > MinGridVisibilityIndent) {
                beginValue = GetBeginValue(OriginPoint.X, Tick);
                float xSize = beginValue + GraphWindowSize.Width;
                for (float i = beginValue; i < xSize; i += Tick) {
                    pt1.X = X2ViewPort(i);
                    pt1.Y = (float)(ViewPortBounds.Bottom + TickLength);
                    pt2.X = pt1.X;
                    pt2.Y = (float)ViewPortBounds.Top;
                    if (DrawGrid) {
                        if (Math.Abs(pt1.X - lastGridPoint.X) >= MinGridVisibilityIndent) {
                            e.Graphics.DrawLine(pen, pt1, pt2);
                            lastGridPoint = pt1;
                        }
                    }
                    if (DrawLabels) {
                        stringArea.Size = e.Graphics.MeasureString(i.ToString(), LabelFont);
                        stringArea.Location = pt1;
                        if (stringArea.IntersectsWith(prevStringArea)) continue;
                        e.Graphics.DrawString(i.ToString(), LabelFont, labelBrush, stringArea.Location);
                        prevStringArea = stringArea;
                    }
                }
            }
        }

        Point downPoint = InvalidDownPoint;
        public Point DownPoint {
            get { return downPoint; }
            set { downPoint = value; }
        }
        
        protected virtual bool IsViewportMoveOp(MouseEventArgs e) {
            return (e.Button == MouseButtons.Left && Control.ModifierKeys == Keys.Alt) || e.Button == MouseButtons.Middle;
        }

        public GraphNode FindActiveNode(Graph priorityGraph) { 
            GraphNode activeNode = null;
            foreach(Graph graph in Graphs) { 
                foreach(GraphNode node in graph.Nodes) { 
                    if(node.PressedInfo.HitTest != GraphNodeHitTest.None) {
                        if(activeNode == null || (node.OwnerGraph == priorityGraph && activeNode.OwnerGraph != priorityGraph) )
                            activeNode = node;
                    }
                }
            }
            return activeNode;
        }

        public virtual void ProcessHitTest(MouseEventArgs e) { 
            foreach(Graph graph in Graphs) {
                graph.ProcessHitTest(e);
            }
        }

        protected override void OnMouseDown(MouseEventArgs e) {
            base.OnMouseDown(e);
            DownPoint = e.Location;
            if (IsViewportMoveOp(e)) {
                Operation = GraphOpType.MoveOriginPoint;
            }
            else 
                ProcessHitTest(e);
        }

        protected override void OnKeyDown(KeyEventArgs e) {
            base.OnKeyDown(e);
            if(e.KeyCode == Keys.ShiftKey) {
                foreach (Graph graph in Graphs)
                    graph.ShowAuxillaryElements = !graph.ShowAuxillaryElements;
            }
        }

        protected override void OnMouseUp(MouseEventArgs e) {
            base.OnMouseUp(e);
            DownPoint = InvalidDownPoint;
            Operation = GraphOpType.None;
            ProcessHitTest(e);
        }

        protected virtual void UpdateOriginPositon(Point np) {
            PointF delta = GetOriginDelta(np);
            OriginPoint = new PointF(OriginPoint.X + delta.X, OriginPoint.Y + delta.Y);
        }

        protected internal virtual PointF GetOriginDelta(Point np) {
            float dx = (DownPoint.X - np.X) / ScaleX;
            float dy = (np.Y - DownPoint.Y) / ScaleY;
            return new PointF(dx, dy);
        }
        protected internal virtual PointF GetMoveDelta(Point np) {
            float dx = (np.X - DownPoint.X) / ScaleX;
            float dy = (DownPoint.Y - np.Y) / ScaleY;
            return new PointF(dx, dy);
        }

        protected override void OnMouseMove(MouseEventArgs e) {
            base.OnMouseMove(e);
            if (Operation == GraphOpType.MoveOriginPoint) {
                UpdateOriginPositon(e.Location);
                DownPoint = e.Location;
            }
            else {
                ProcessHitTest(e);
            } 
        }

        protected override void OnDoubleClick(EventArgs e) {
            base.OnDoubleClick(e);
            OriginPoint = DefaultOriginPoint;
        }
        
        protected virtual float UpdateScale(float scale, int delta) {
            float scaleInc = scale > 1.0f? ScalesIncrement: ScalesIncrementDecimal;
            if (scale > 1.0f) return Math.Max(1.0f, scale + scaleInc * delta / 120);
            return Math.Max(0.001f, scale + scaleInc * delta / 120);
        }
        protected virtual void UpdateScaleX(int delta) {
            ScaleX = UpdateScale(ScaleX, delta);
        }

        protected virtual void UpdateScaleY(int delta) {
            ScaleY = UpdateScale(ScaleY, delta);
        }

        protected override void OnMouseWheel(MouseEventArgs e) {
            base.OnMouseWheel(e);
            if (Control.ModifierKeys == Keys.Control) {
                UpdateScaleX(e.Delta);
                return;
            }
            else if (Control.ModifierKeys == Keys.Shift && ScaleX > 1.0f) {
                UpdateScaleY(e.Delta);
                return;
            }
            UpdateScaleX(e.Delta);
            UpdateScaleY(e.Delta);
            ProcessHitTest(e);
        }

        protected virtual void DrawGraphs(PaintEventArgs e) {
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
            foreach(Graph graph in Graphs) {
                graph.Draw(e, this);
            }
        }

        protected override void OnPaint(PaintEventArgs e) {
            //base.OnPaint(e);
            DrawGridAndLabels(e);
            DrawAxis(e);
            DrawGraphs(e);
        }
    }

    public class GraphCollection : CollectionBase {
        GraphViewer viewer = null;

        public GraphCollection() : this(null) { }

        public GraphCollection(GraphViewer viewer) {
            this.viewer = viewer;
        }
        
        public GraphViewer Viewer {
            get { return viewer; }
            set {
                if (Viewer == value) return;
                viewer = value;
                OnPropertiesChanged();
            }
        }
        
        protected internal virtual void OnPropertiesChanged() {
            if (Viewer != null)
                Viewer.Refresh();
        }

        public int Add(Graph graph) { return List.Add(graph); }
        public void Insert(int index, Graph graph) { List.Insert(index, graph); }
        public void Remove(Graph graph) { List.Remove(graph); }
        public Graph this[int index] { get { return List[index] as Graph; } set { List[index] = value; } }

        protected override void OnInsertComplete(int index, object value) {
            base.OnInsertComplete(index, value);
            ((Graph)value).OwnerColl = this;
        }

        protected override void OnRemoveComplete(int index, object value) {
            base.OnRemoveComplete(index, value);
            ((Graph)value).OwnerColl = null;
        }
    }
}
