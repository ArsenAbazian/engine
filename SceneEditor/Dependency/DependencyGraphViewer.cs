using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;
using System.Collections;

namespace sceneEditor.Dependency {
    public class DependencyGraphViewer : ContainerControl {
        static Color DefaultLineColor = Color.DarkGray;
        static Color DefaultCaptionAreaColor = Color.LightGray;
        static Color DefaultDependencyAreaColor = Color.White;
        static Color DefaultDependencyTextColor = Color.Blue;
        public static int BlockIndent = 50;
        public static int BlockVerticalIndent = 20;

        Color lineColor = DefaultLineColor;
        Color blockCaptionAreaColor = DefaultCaptionAreaColor;
        Color blockDependencyAreaColor = DefaultDependencyAreaColor;
        Color depTextColor = DefaultDependencyTextColor;
        a3dDependencyPropertyMapper property;
        private ContextMenuStrip menu;
        private IContainer components;
        private ToolStripMenuItem propMenuItem;
        private ToolStripMenuItem simpleMenuItem;
        private ToolStripMenuItem graphMenuItem;
        private ToolStripMenuItem scriptMenuItem;
        private ToolStripMenuItem noneMenuItem;
        private ToolStripSeparator toolStripSeparator1;
        DependencyPropertyBlock rootBlock;

        public DependencyGraphViewer() {
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            InitializeComponent();
        }

        bool ShouldSerializeDependencyTextColor() { return DependencyTextColor != DefaultDependencyTextColor; }
        public Color DependencyTextColor {
            get { return depTextColor; }
            set {
                if (DependencyTextColor == value) return;
                depTextColor = value;
                OnPropertiesChanged();
            }
        }
        
        bool ShouldSerializeLineColor() { return LineColor != DefaultLineColor; }
        public Color LineColor {
            get { return lineColor; }
            set {
                if (LineColor == value) return;
                lineColor = value;
                OnPropertiesChanged();
            }
        }

        bool ShouldSerializeCaptionAreaColor() { return CaptionAreaColor != DefaultCaptionAreaColor; }
        public Color CaptionAreaColor {
            get { return blockCaptionAreaColor; }
            set {
                if (CaptionAreaColor == value) return;
                blockCaptionAreaColor = value;
                OnPropertiesChanged();
            }
        }

        bool ShouldSerializeDependencyAreaColor() { return DependencyAreaColor != DefaultDependencyAreaColor; }
        public Color DependencyAreaColor {
            get { return blockDependencyAreaColor; }
            set {
                if (DependencyAreaColor == value) return;
                blockDependencyAreaColor = value;
                OnPropertiesChanged();
            }
        }

        public a3dDependencyPropertyMapper Property {
            get { return property; }
            set {
                if (Property == value) return;
                property = value;
                OnPropertyChanged();
            }
        }

        DependencyPropertyBlock selectedBlock;
        public DependencyPropertyBlock SelectedBlock {
            get { return selectedBlock; }
            set { 
                if(SelectedBlock == value) return;
                selectedBlock = value;
                OnSelectedBlockChanged();
            }
        }

        protected virtual void OnSelectedBlockChanged() {
            foreach (object item in menu.Items) {
                ToolStripMenuItem menuItem = item as ToolStripMenuItem;
                if (menuItem == null) continue;
                menuItem.Enabled = SelectedBlock != null;
            }
            propMenuItem.Enabled = SelectedDependency != null;
            OnPropertiesChanged();
        }

        public DependencyPropertyBlock RootBlock { get { return rootBlock; } }
        public a3dDependencyBaseMapper SelectedDependency { 
            get { return SelectedBlock == null ? null : SelectedBlock.Property.Dependency; }
            set {
                if(SelectedBlock == null) return;
                SelectedBlock.Property.Dependency = value;
                Regenerate();
                
                OnPropertiesChanged();
            }
        }

        protected virtual void Regenerate() {
            SelectedBlock.Update();
            CalcBestSize();
            UpdateView();
        }

        protected virtual void OnPropertyChanged() {
            this.rootBlock = new DependencyPropertyBlock(Property, this);
            CalcBestSize();
            OnPropertiesChanged();
        }

        protected virtual void OnPropertiesChanged() {
            Invalidate();
        }

        protected virtual void CalcBestSize() {
            Graphics g = Graphics.FromHwnd(this.Handle);
            RootBlock.Bounds = new Rectangle(Point.Empty, RootBlock.CalcBestSize(g));
            g.Dispose();
        }

        protected virtual void UpdateLayout(Point loc) {
            RootBlock.UpdateLayout(loc);
            Invalidate();
        }

        protected override void OnPaint(PaintEventArgs pe) {
            base.OnPaint(pe);
            if(RootBlock != null)
                RootBlock.Draw(pe);
        }

        protected internal void UpdateView() {
            UpdateLayout(new Point(BlockVerticalIndent, BlockVerticalIndent));
        }

        protected override void OnMouseDown(MouseEventArgs e) {
            base.OnMouseDown(e);
            if(RootBlock != null)
                SelectedBlock = RootBlock.ProcessHitTest(e);
            if (e.Button == MouseButtons.Right)
                menu.Show(PointToScreen(e.Location));
        }

        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            this.menu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.propMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.noneMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.simpleMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.graphMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.scriptMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.menu.SuspendLayout();
            this.SuspendLayout();
            // 
            // menu
            // 
            this.menu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.simpleMenuItem,
            this.graphMenuItem,
            this.scriptMenuItem,
            this.noneMenuItem,
            this.toolStripSeparator1,
            this.propMenuItem});
            this.menu.Name = "menu";
            this.menu.Size = new System.Drawing.Size(179, 114);
            this.menu.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.menu_ItemClicked);
            // 
            // propMenuItem
            // 
            this.propMenuItem.Name = "propMenuItem";
            this.propMenuItem.Size = new System.Drawing.Size(178, 22);
            this.propMenuItem.Text = "Properties";
            // 
            // noneMenuItem
            // 
            this.noneMenuItem.Name = "noneMenuItem";
            this.noneMenuItem.Size = new System.Drawing.Size(178, 22);
            this.noneMenuItem.Text = "Clear Dependency";
            // 
            // simpleMenuItem
            // 
            this.simpleMenuItem.Name = "simpleMenuItem";
            this.simpleMenuItem.Size = new System.Drawing.Size(178, 22);
            this.simpleMenuItem.Text = "Simple Dependency";
            // 
            // graphMenuItem
            // 
            this.graphMenuItem.Name = "graphMenuItem";
            this.graphMenuItem.Size = new System.Drawing.Size(178, 22);
            this.graphMenuItem.Text = "Graph Dependency";
            // 
            // scriptMenuItem
            // 
            this.scriptMenuItem.Name = "scriptMenuItem";
            this.scriptMenuItem.Size = new System.Drawing.Size(178, 22);
            this.scriptMenuItem.Text = "Script Dependency";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(175, 6);
            this.menu.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        private void menu_ItemClicked(object sender, ToolStripItemClickedEventArgs e) {
            if (e.ClickedItem == propMenuItem)
                ShowProperties();
            else if (e.ClickedItem == simpleMenuItem)
                SimpleDependency();
            else if (e.ClickedItem == graphMenuItem)
                GraphDependency();
            else if (e.ClickedItem == scriptMenuItem)
                ScriptDependency();
            else if (e.ClickedItem == noneMenuItem)
                ClearDependency();
        }

        protected virtual void ClearDependency() {
            if(MessageBox.Show("You will clear dependency. Are you sure?", "Dependency", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No) return;
            SelectedDependency = null;
        }

        protected virtual void ScriptDependency() {
            if(SelectedDependency != null && MessageBox.Show("You will change dependency to SCRIPT. Are you sure?", "Dependency", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No) return;
            
        }

        public MainForm MainForm { get; set; }

        bool hideOnDialog = true;
        public bool HideOnDialog {
            get { return hideOnDialog; }
            set { hideOnDialog = value; }
        }

        IDependencyTreeNode rootNode;
        public IDependencyTreeNode RootNode {
            get { return rootNode; }
            set { rootNode = value; }
        }

        protected virtual void SimpleDependency() {
            if(SelectedDependency != null && MessageBox.Show("You will change dependency to SIMPLE. Are you sure?", "Dependency", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No) return;
            if(HideOnDialog) FindForm().Hide();
            try {
                using(SimpleDependencyPropertyForm form = new SimpleDependencyPropertyForm()) {
                    form.RootNode = RootNode;
                    form.Destination = SelectedBlock.Property;
                    if(form.ShowDialog() == DialogResult.OK) {
                        a3dSimpleDependencyMapper simpleDep = new a3dSimpleDependencyMapper();
                        simpleDep.Source = form.SelectedProperty;
                        SelectedDependency = simpleDep;
                    }
                }
            }
            finally {
                if(HideOnDialog) FindForm().Show();
            }
        }

        protected virtual void GraphDependency() {
            if(SelectedDependency != null && MessageBox.Show("You will change dependency to GRAPH. Are you sure?", "Dependency", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No) return;
            if(HideOnDialog) FindForm().Hide();
            try {
                using(GraphDependencyPropertyForm form = new GraphDependencyPropertyForm()) {
                    form.Destination = SelectedBlock.Property;
                    form.MainForm = MainForm;
                    if(form.ShowDialog() == DialogResult.OK) {
                        a3dGraphDependencyMapper graphDep = new a3dGraphDependencyMapper();
                        graphDep.Graph = form.Graph;
                        graphDep.Graph2 = form.Graph2;
                        graphDep.Source = form.Source1;
                        graphDep.Source2 = form.Source2;
                        graphDep.LoopGraph = form.LoopGraph;
                        graphDep.LoopGraph2 = form.LoopGraph2;
                        graphDep.Operation = form.Operation;
                        SelectedDependency = graphDep;
                    }
                }
            }
            finally {
                if(HideOnDialog) FindForm().Show();
            }
        }

        protected virtual void ShowProperties() {
            if(SelectedDependency is a3dSimpleDependencyMapper)
                ShowSimpleDependencyProperties();
            else if(SelectedDependency is a3dGraphDependencyMapper)
                ShowGraphDependencyProperties();
            else if(SelectedDependency is a3dScriptDependencyMapper)
                ShowScriptDependencyProperties();
        }

        protected virtual void ShowScriptDependencyProperties() {
            throw new NotImplementedException();
        }

        protected virtual void ShowGraphDependencyProperties() {
            throw new NotImplementedException();
        }

        protected virtual void ShowSimpleDependencyProperties() {
            Hide();
            try {
                using(SimpleDependencyPropertyForm form = new SimpleDependencyPropertyForm()) {
                    form.ShowDialog();
                }
            }
            finally { Show(); }
        }
    }

    public class DependencyPropertyBlockCollection : CollectionBase {
        public int Add(DependencyPropertyBlock block) { return List.Add(block); }
        public DependencyPropertyBlock this[int index] { get { return List[index] as DependencyPropertyBlock; } set { List[index] = value; } }
    }

    public class DependencyPropertyBlockHelper {
        public static void InitializeBlockNodes(DependencyPropertyBlock block) { 
            block.Nodes.Clear();
            if (block.Property == null) return;
            a3dGraphDependencyMapper gd = block.Property.Dependency as a3dGraphDependencyMapper;
            if(gd != null) {
                if(gd.Source != null)
                    block.Nodes.Add(new DependencyPropertyBlock(gd.Source, block.Viewer));
                if(gd.Source2 != null)
                    block.Nodes.Add(new DependencyPropertyBlock(gd.Source2, block.Viewer));
                return;
            }
            a3dSimpleDependencyMapper sd = block.Property.Dependency as a3dSimpleDependencyMapper;
            if(sd != null) {
                block.Nodes.Add(new DependencyPropertyBlock(sd.Source, block.Viewer));
                return;
            }
        }
    }
    
    public class DependencyPropertyBlock {
        static int DependencyAreaHeight = 30;

        DependencyPropertyBlockCollection nodes;
        a3dDependencyPropertyMapper property;
        DependencyGraphViewer viewer;
        Rectangle bounds;

        public DependencyPropertyBlock(a3dDependencyPropertyMapper property, DependencyGraphViewer viewer) {
            this.property = property;
            this.viewer = viewer;
            Update();
        }

        public void Update() {
            DependencyPropertyBlockHelper.InitializeBlockNodes(this);
        }

        public DependencyPropertyBlockCollection Nodes { 
            get { 
                if(nodes == null)
                    nodes = new DependencyPropertyBlockCollection();
                return nodes; 
            } 
        }

        public Rectangle Bounds {
            get { return bounds; }
            set { bounds = value; }
        }

        public a3dDependencyPropertyMapper Property {
            get { return property; }
        }

        public DependencyGraphViewer Viewer { get { return viewer; } }

        public string Caption {
            get {
                if (Property == null) return "undefined";
                return Property.ToString() + " : " + Property.PropertyType.ToString();
            }
        }

        public Size CalcBestSize(Graphics g) {
            foreach (DependencyPropertyBlock node in Nodes) { 
                node.Bounds = new Rectangle(Point.Empty, node.CalcBestSize(g));
            }
            Size res = g.MeasureString(Caption, Viewer.Font, 0).ToSize();
            return new Size(res.Width + 10, res.Height + 10 + DependencyAreaHeight);
        }

        public Rectangle CaptionRect {
            get {
                return new Rectangle(Bounds.Location, new Size(Bounds.Width, Bounds.Height - DependencyAreaHeight));
            }
        }

        public Rectangle DependencyRect {
            get {
                return new Rectangle(Bounds.X, Bounds.Bottom - DependencyAreaHeight, Bounds.Width, DependencyAreaHeight);
            }
        }

        public void Draw(PaintEventArgs e) {
            DrawSelectedBorder(e);
            DrawBlockRects(e);
            DrawDependencyType(e);
            DrawInputArrows(e);

            foreach (DependencyPropertyBlock node in Nodes) {
                node.Draw(e);
            }
        }

        Point inputPoint, outputPoint;
        public Point InputPoint { 
            get { 
                if(inputPoint == Point.Empty)
                    inputPoint = new Point(Bounds.Right, Bounds.Y + Bounds.Height / 2);
                return inputPoint;
            } 
        }
        public Point OutputPoint { 
            get {
                if(outputPoint == Point.Empty)
                    outputPoint = new Point(Bounds.Left, Bounds.Y + Bounds.Height / 2);
                return outputPoint;
            } 
        }

        protected virtual void DrawInputArrows(PaintEventArgs e) {
            if(Nodes.Count == 0) return;
            Pen pen = new Pen(Viewer.LineColor, 1);
            int bIndent = 20;
            int endX = 0;
            for(int i = 0; i <= Nodes.Count / 2; i++) {
                endX = InputPoint.X + bIndent + i * 3;
                e.Graphics.DrawLine(pen, Nodes[i].OutputPoint, new Point(InputPoint.X + bIndent + i * 3, Nodes[i].OutputPoint.Y));
                e.Graphics.DrawLine(pen, new Point(InputPoint.X + bIndent + 3 * i, InputPoint.Y), new Point(InputPoint.X + bIndent + i * 3, Nodes[i].OutputPoint.Y));
                int n = Nodes.Count - 1 - i;
                e.Graphics.DrawLine(pen, Nodes[n].OutputPoint, new Point(InputPoint.X + bIndent + i * 3, Nodes[n].OutputPoint.Y));
                e.Graphics.DrawLine(pen, new Point(InputPoint.X + bIndent + 3 * i, InputPoint.Y), new Point(InputPoint.X + bIndent + i * 3, Nodes[n].OutputPoint.Y));
            }
            e.Graphics.DrawLine(pen, InputPoint, new Point(endX, InputPoint.Y));
            e.Graphics.DrawLine(pen, InputPoint, new Point(InputPoint.X + 10, InputPoint.Y - 3));
            e.Graphics.DrawLine(pen, InputPoint, new Point(InputPoint.X + 10, InputPoint.Y + 3));
        }

        protected virtual void DrawSelectedBorder(PaintEventArgs e) {
            if(Viewer.SelectedBlock != this) return;
            Pen p = new Pen(Color.Blue, 2);
            Rectangle r = Bounds;
            r.Inflate(4,4);
            e.Graphics.DrawRectangle(p, r);
        }

        protected virtual void DrawBlockRects(PaintEventArgs e) {
            Pen p = new Pen(Viewer.LineColor);
            Brush b = new SolidBrush(Viewer.CaptionAreaColor);
            Brush t = new SolidBrush(Viewer.ForeColor);
            Rectangle rect = CaptionRect;
            e.Graphics.FillRectangle(b, rect);
            e.Graphics.DrawRectangle(p, rect);
            rect.Inflate(-5, -5);
            e.Graphics.DrawString(Caption, Viewer.Font, t, rect.Location);
            rect = DependencyRect;
            b = new SolidBrush(Viewer.DependencyAreaColor);
            e.Graphics.FillRectangle(b, rect);
            e.Graphics.DrawRectangle(p, rect);
        }

        protected virtual void DrawDependencyType(PaintEventArgs e) {

            string s = "NONE";
            if (Property.Dependency is a3dSimpleDependencyMapper)
                s = "SIMPLE";
            else if (Property.Dependency is a3dGraphDependencyMapper)
                s = "GRAPH";
            else if (Property.Dependency is a3dScriptDependencyMapper)
                s = "SCRIPT";
            using (StringFormat sf = new StringFormat()) {
                sf.Alignment = StringAlignment.Center;
                sf.LineAlignment = StringAlignment.Center;
                e.Graphics.DrawString(s, Viewer.Font, new SolidBrush(Viewer.DependencyTextColor), DependencyRect, sf);
            }
        }

        protected internal virtual int UpdateLayout(Point loc) {
            Bounds = new Rectangle(loc, Bounds.Size);
            int height = 0, total = 0;
            foreach (DependencyPropertyBlock node in Nodes) { 
                height = node.UpdateLayout(new Point(loc.X + Bounds.Width + DependencyGraphViewer.BlockIndent, loc.Y));
                node.Bounds = new Rectangle(new Point(node.Bounds.X, node.Bounds.Y + (height - node.Bounds.Height) / 2), Bounds.Size);
                loc.Y += DependencyGraphViewer.BlockVerticalIndent + height;
                total += DependencyGraphViewer.BlockVerticalIndent + height;
            }
            if (total != 0) {
                total -= DependencyGraphViewer.BlockVerticalIndent;
                Bounds = new Rectangle(Bounds.X, Bounds.Y + (total - Bounds.Height) / 2, Bounds.Width, Bounds.Height);
            }
            return Math.Max(total, Bounds.Height);
        }

        protected internal DependencyPropertyBlock ProcessHitTest(MouseEventArgs e) {
            if (Bounds.Contains(e.Location)) return this;
            foreach (DependencyPropertyBlock node in Nodes) {
                DependencyPropertyBlock b = node.ProcessHitTest(e);
                if (b != null) return b;
            }
            return null;
        }
    }
}
