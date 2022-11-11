using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel;

namespace sceneEditor.TimeLineEditor {
    
    public class TimeLineEditor : Control {
        private static readonly object valueChanged = new object();

        static Color DefaultLineColor = Color.DarkGray;
        static Color DefaultLabelColor = Color.Black;
        static Color DefaultThumbColor = Color.FromArgb(0x40A8C0FF);
        static Padding DefaultContentPadding = new Padding(10,0,32,0);

        Color lineColor = DefaultLineColor;
        Color labelColor = DefaultLabelColor;
        Color thumbColor = DefaultThumbColor;
        Padding contentPadding = DefaultContentPadding;

        public TimeLineEditor() {
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
        }
        
        protected virtual void OnPropertiesChanged() {
            Invalidate();
        }

        bool ShouldSerializeLineColor() { return !Color.Equals(DefaultLineColor, LineColor); }
        public Color LineColor {
            get { return lineColor; }
            set {
                if (LineColor == value) return;
                lineColor = value;
                OnPropertiesChanged();
            }
        }

        bool ShouldSerializeLabelColor() { return !Color.Equals(DefaultLabelColor, LabelColor); }
        public Color LabelColor {
            get { return labelColor; }
            set {
                if (LabelColor == value) return;
                labelColor = value;
                OnPropertiesChanged();
            }
        }

        long labelMiliSec = 10;
        [DefaultValue(10)]
        public long LabelMiliSec {
            get { return labelMiliSec; }
            set {
                if (labelMiliSec == value) return;
                labelMiliSec = value;
                OnPropertiesChanged();
            }
        }

        int tickHeight = 10;
        [DefaultValue(10)]
        public int TickHeight {
            get { return tickHeight; }
            set {
                if (TickHeight == value) return;
                tickHeight = value;
                OnPropertiesChanged();
            }
        }

        long endTime = 1000;
        [DefaultValue(1000)]
        public long EndTime {
            get { return endTime; }
            set {
                if (EndTime == value) return;
                endTime = value;
                OnPropertiesChanged();
            }
        }

        int tickMinIndent = 8;
        [DefaultValue(8)]
        public int TickMinIndent {
            get { return tickMinIndent; }
            set {
                if (TickMinIndent == value) return;
                tickMinIndent = value;
                OnPropertiesChanged();
            }
        }
        int labelTick = 5;
        [DefaultValue(5)]
        public int LabelTick {
            get { return labelTick; }
            set {
                if (LabelTick == value) return;
                labelTick = value;
                OnPropertiesChanged();
            }
        }

        bool ShouldSerializeContentPadding() { return ContentPadding != DefaultContentPadding; }
        public Padding ContentPadding {
            get { return contentPadding; }
            set {
                if (ContentPadding == value) return;
                contentPadding = value;
                OnPropertiesChanged();
            }
        }

        int labelTopIndent = 5;
        [DefaultValue(5)]
        public int LabelTopIndent {
            get { return labelTopIndent; } 
            set {
                if (LabelTopIndent == value) return;
                labelTopIndent = value;
                OnPropertiesChanged();
            } 
        }

        bool unlimited = false;
        [DefaultValue(false)]
        public bool Unlimited {
            get { return unlimited; }
            set {
                if (Unlimited == value) return;
                unlimited = value;
                OnPropertiesChanged();
            }
        }

        int thumbWidth = 8;
        [DefaultValue(8)]
        public int ThumbWidth {
            get { return thumbWidth; }
            set {
                if (ThumbWidth == value) return;
                thumbWidth = value;
                OnPropertiesChanged();
            }
        }

        bool ShouldSerializeThumbColor() { return Color.Equals(ThumbColor, DefaultThumbColor); }
        public Color ThumbColor {
            get { return thumbColor; }
            set {
                if (ThumbColor == value) return;
                thumbColor = value;
                OnPropertiesChanged();
            }
        }

        public int LineTopIndent { get { return 16; } }

        public Rectangle ContentRectangle { 
            get {
                return new Rectangle(DisplayRectangle.X + ContentPadding.Left, DisplayRectangle.Y + ContentPadding.Top, DisplayRectangle.Width - ContentPadding.Horizontal, DisplayRectangle.Height - ContentPadding.Vertical);
            } 
        }

        public Point LineStartPoint {
            get { return new Point(ContentRectangle.X, ContentRectangle.Y + LineTopIndent); }
        }
        public Point LineEndPoint {
            get { return new Point(ContentRectangle.Right, ContentRectangle.Y + LineTopIndent); }
        }
        
        public int TimeAreaWidth { get { return ContentRectangle.Width; } }
        int[] ratio = new int[] { 1,  2,  5,  10, 20, 25, 50, 100, 200, 250, 500, 1000, 2000, 2500, 5000, 10000, 20000, 25000, 50000, 100000 };
        public long TimeDelta {
            get {

                for (int r = 0; r < ratio.Length; r++) {
                    if (ratio[r] * TimeAreaWidth / EndTime > TickMinIndent)
                        return ratio[r];
                }
                return EndTime;
            }
        }

        long value = 0;
        [DefaultValue(0)]
        public long Value {
            get { return value; }
            set {
                if(!Unlimited)
                    value = Math.Min(EndTime, value);
                if (Value == value) return;
                this.value = value;
                RaiseValueChanged();
                OnPropertiesChanged();
            }
        }

        public event EventHandler ValueChanged {
            add { Events.AddHandler(valueChanged, value); }
            remove { Events.RemoveHandler(valueChanged, value); }
        }
        protected virtual void RaiseValueChanged() {
            EventHandler handler = (EventHandler)Events[valueChanged];
            if (handler != null) handler(this, EventArgs.Empty);
        }

        public Point ThumbPosition {
            get { return new Point(LineStartPoint.X + (int)(((double)Value / EndTime) * TimeAreaWidth), LineStartPoint.Y); }
        }

        public Rectangle ThumbRectangle {
            get {
                return new Rectangle(new Point(ThumbPosition.X - ThumbWidth / 2, ThumbPosition.Y), new Size(ThumbWidth, ContentRectangle.Bottom - LineStartPoint.Y - 1));
            }
        }

        protected virtual void DrawLine(PaintEventArgs e) {
            e.Graphics.DrawLine(new Pen(LineColor), LineStartPoint, LineEndPoint);
        }

        protected virtual void DrawTick(PaintEventArgs e) { 
            long timeDelta = TimeDelta;
            Pen linePen = new Pen(LineColor);
            for (long i = 0; i <= EndTime; i += timeDelta) { 
                int x = LineStartPoint.X + (int)(((double)i / EndTime) * TimeAreaWidth);
                e.Graphics.DrawLine(linePen, new Point(x, LineStartPoint.Y), new Point(x, LineStartPoint.Y + TickHeight));
            }
        }
        protected virtual void DrawLabels(PaintEventArgs e) {
            long timeDelta = TimeDelta * LabelTick;
            Pen linePen = new Pen(LineColor);
            Brush labelBrush = new SolidBrush(LabelColor);
            StringFormat format = new StringFormat();
            format.Alignment = StringAlignment.Center;
            format.LineAlignment = StringAlignment.Near;
            
            for (long i = 0; i <= EndTime; i += timeDelta) {
                int x = LineStartPoint.X + (int)(((double)i / EndTime) * TimeAreaWidth);
                e.Graphics.DrawLine(linePen, new Point(x, LineStartPoint.Y), new Point(x, ContentRectangle.Bottom));
                e.Graphics.DrawString(i.ToString(), Font, labelBrush, new Point(x, LineStartPoint.Y + TickHeight + LabelTopIndent), format);
            }
        }

        protected virtual void DrawThumb(PaintEventArgs e) {
            if (Value > EndTime) return;
            Pen linePen = new Pen(LineColor);
            Brush thumbBrush = new SolidBrush(ThumbColor);
            e.Graphics.FillRectangle(thumbBrush, ThumbRectangle);
            e.Graphics.DrawRectangle(linePen, ThumbRectangle);
            e.Graphics.DrawLine(linePen, ThumbPosition, new Point(ThumbPosition.X, ThumbPosition.Y - 3));
            e.Graphics.DrawLine(linePen, new Point(ThumbPosition.X - 32, ThumbPosition.Y - 3), new Point(ThumbPosition.X + 32, ThumbPosition.Y - 3));
            StringFormat f = new StringFormat();
            f.LineAlignment = StringAlignment.Far;
            f.Alignment = StringAlignment.Center;
            Brush labelBrush = new SolidBrush(LabelColor);
            e.Graphics.DrawString(Value.ToString(), Font, labelBrush, new Point(ThumbPosition.X, ThumbPosition.Y - 3), f);
        }

        protected override void OnPaint(PaintEventArgs e) {
            base.OnPaint(e);
            DrawLine(e);
            DrawTick(e);
            DrawLabels(e);
            DrawThumb(e);
        }
        protected virtual long TimeFromPos(int x) {
            return Math.Min(EndTime, Math.Max(0, (long)(((double)(x - LineStartPoint.X) / TimeAreaWidth) * EndTime)));
        }
        protected virtual void UpdateCursor(MouseEventArgs e) {
            if (ThumbRectangle.Contains(e.Location))
                Cursor.Current = Cursors.SizeWE;
            else
                Cursor.Current = Cursors.Arrow;
        }
        protected override void OnMouseDown(MouseEventArgs e) {
            base.OnMouseDown(e);
            if (e.Button == MouseButtons.Left && !ThumbRectangle.Contains(e.Location))
                Value = TimeFromPos(e.Location.X);
        }
        protected override void OnMouseMove(MouseEventArgs e) {
            base.OnMouseMove(e);
            UpdateCursor(e);
            if(e.Button == MouseButtons.Left) {
                Value = TimeFromPos(e.Location.X);
                Invalidate();
            }
        }
    }
}
