using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel;
using System.Collections;
using System.Drawing.Drawing2D;

namespace imageGallery {

    public enum ObjectState { Normal, Hot, Pressed }
    
    public class ColorPoint {
        ColorPointCollection holder;
        Color color;
		float offset;

		public ColorPoint() { }

		public ColorPoint(Color color, float offset) {
			this.color = color;
			this.offset = offset;
		}

		public Color Color {
			get { return color; }
			set {
				if(Color == value) return;
				color = value;
				OnColorChanged();
 			}
		}

		public float Offset {
			get { return offset; }
			set {
				if(Offset == value) return;
				offset = value;
				OnOffsetChanged();
			}
		}

        public ColorPointCollection Holder {
            get { return holder; }
            set { holder = value; }
        }

        protected virtual void OnPropertiesChanged() {
            if (Holder != null)
                Holder.RaiseCollectionChanged();
        }
        
        protected virtual void OnColorChanged() {
            OnPropertiesChanged();
        }
		protected virtual void OnOffsetChanged() {
            OnPropertiesChanged();
        }
	}

	public class ColorPointCollection : CollectionBase {
		private readonly object collectionChnaged = new object();
		public EventHandlerList events;

		public ColorPointCollection() {
			this.events = new EventHandlerList();
		}

		protected EventHandlerList Events { get { return events; } }

		public int Add(ColorPoint point) { return InnerList.Add(point); }
		public void Insert(int index, ColorPoint point) { InnerList.Insert(index, point); }
		public void Remove(ColorPoint point) { InnerList.Remove(point); }
		public ColorPoint this[int index] { get { return InnerList[index] as ColorPoint; } set { InnerList[index] = value; } }

		public event EventHandler CollectionChanged {
			add { Events.AddHandler(collectionChnaged, value); }
			remove { Events.RemoveHandler(collectionChnaged, value); }
		}

		protected internal virtual void RaiseCollectionChanged() {
			EventHandler handler = Events[collectionChnaged] as EventHandler;
			if(handler != null) handler(this, EventArgs.Empty);
		}

		protected override void OnClearComplete() {
			base.OnClearComplete();
			RaiseCollectionChanged();
		}
		protected override void OnSetComplete(int index, object oldValue, object newValue) {
			base.OnSetComplete(index, oldValue, newValue);
			RaiseCollectionChanged();
		}
		protected override void OnInsertComplete(int index, object value) {
			base.OnInsertComplete(index, value);
			RaiseCollectionChanged();
		}
		protected override void OnRemoveComplete(int index, object value) {
			base.OnRemoveComplete(index, value);
			RaiseCollectionChanged();
		}
	}
	
	class ColorSlider : Control {
		private static readonly object positionChanged = new object();
        
        ColorPointCollection coll;
		Color borderColor; 
        Color trackBorderColor;
        Color trackFillColor;
        Color thumbNormalFillColor;
        Color thumbNormalBorderColor;
        Color thumbHotFillColor;
        Color thumbHotBorderColor;
        Color thumbPressedFillColor;
        Color thumbPressedBorderColor;
        Padding trackPadding;
        bool allowUpperThumb;
        bool allowLowerThumb;
        int thumbIndent;
        Size thumbSize;
        float position;
        ObjectState upperThumbState, lowerThumbState;

		public ColorSlider() {
            this.upperThumbState = ObjectState.Normal;
            this.lowerThumbState = ObjectState.Normal;
            this.allowUpperThumb = true;
            this.allowLowerThumb = true;
            this.borderColor = Color.LightGray;
            this.thumbNormalBorderColor = Color.DimGray;
            this.thumbNormalFillColor = Color.DarkGray;
            this.thumbHotBorderColor = Color.DarkGray;
            this.thumbHotFillColor = Color.LightGray;
            this.thumbPressedBorderColor = Color.LightGray;
            this.thumbPressedFillColor = Color.WhiteSmoke;
            this.thumbSize = new Size(6, 9);
            this.thumbIndent = 2;
            this.trackPadding = new Padding(5, 15, 5, 15);
            DoubleBuffered = true;
        }

        protected override Size DefaultSize { get { return new Size(266, 42); } }

        protected virtual void OnPropertiesChanged() {
            Invalidate();
        }
        protected virtual void OnCollectionChanged(object sender, EventArgs e) {
            OnPropertiesChanged();
        }
		protected virtual ColorPointCollection CreateColorPointCollection() { 
            ColorPointCollection cc = new ColorPointCollection();
            cc.CollectionChanged += new EventHandler(OnCollectionChanged);
            return cc;
        }
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        public ColorPointCollection Points {
			get {
				if(coll == null) 
					coll = CreateColorPointCollection();
				return coll;
			}
		}
        protected bool ShouldSerializeBorderColor() { return BorderColor != Color.LightGray; }
        public Color BorderColor { 
			get { return borderColor; }
			set {
				if(BorderColor == value) return;
				borderColor = value;
				OnPropertiesChanged();
			}
		}
        protected bool ShouldSerializeTrackBorderColor() { return TrackBorderColor != Color.Transparent; }
		public Color TrackBorderColor {
			get { return trackBorderColor; }
			set {
				if(TrackBorderColor == value) return;
				trackBorderColor = value;
                OnPropertiesChanged();
			}
		}
        protected bool ShouldSerializeThumbNormalFillColor() { return ThumbNormalFillColor != Color.DarkGray; }
        public Color ThumbNormalFillColor {
            get { return thumbNormalFillColor; }
            set {
                if (ThumbNormalFillColor == value) return;
                thumbNormalFillColor = value;
                OnPropertiesChanged();
            }
        }
        protected bool ShouldSerializeThumbNormalBorderColor() { return ThumbNormalBorderColor != Color.DimGray; }
        public Color ThumbNormalBorderColor {
            get { return thumbNormalBorderColor; }
            set {
                if (ThumbNormalBorderColor == value) return;
                thumbNormalBorderColor = value;
                OnPropertiesChanged();
            }
        }
        protected bool ShouldSerializeThumbHotFillColor() { return ThumbHotFillColor != Color.LightGray; }
        public Color ThumbHotFillColor {
            get { return thumbHotFillColor; }
            set {
                if (ThumbHotFillColor == value) return;
                thumbHotFillColor = value;
                OnPropertiesChanged();
            }
        }
        protected bool ShouldSerializeThumbHotBorderColor() { return ThumbHotBorderColor != Color.DarkGray; }
        public Color ThumbHotBorderColor {
            get { return thumbHotBorderColor; }
            set {
                if (ThumbHotBorderColor == value) return;
                thumbHotBorderColor = value;
                OnPropertiesChanged();
            }
        }
        protected bool ShouldSerializeThumbPressedFillColor() { return ThumbPressedFillColor != Color.WhiteSmoke; }
        public Color ThumbPressedFillColor {
            get { return thumbPressedFillColor; }
            set {
                if (ThumbPressedFillColor == value) return;
                thumbPressedFillColor = value;
                OnPropertiesChanged();
            }
        }

        protected bool ShouldSerializeThumbPressedBorderColor() { return ThumbPressedBorderColor != Color.LightGray; }
        public Color ThumbPressedBorderColor {
            get { return thumbPressedBorderColor; }
            set {
                if (ThumbPressedBorderColor == value) return;
                thumbPressedBorderColor = value;
                OnPropertiesChanged();
            }
        }
        protected bool ShouldSerializeThumbSize() { return ThumbSize != new Size(6,9); }
        public Size ThumbSize {
            get { return thumbSize; }
            set {
                if (ThumbSize == value) return;
                thumbSize = value;
                OnPropertiesChanged();
            }
        }
        [DefaultValue(true)]
        public bool AllowUpperThumb { 
            get { return allowUpperThumb; }
            set {
                if (AllowUpperThumb == value) return;
                allowUpperThumb = value;
                OnPropertiesChanged();
            }
        }
        [DefaultValue(true)]
        public bool AllowLowerThumb {
            get { return allowLowerThumb; }
            set {
                if (AllowLowerThumb == value) return;
                allowLowerThumb = value;
                OnPropertiesChanged();
            }
        }
        [DefaultValue(2)]
        public int ThumbIndent {
            get { return thumbIndent; }
            set {
                if (ThumbIndent == value) return;
                thumbIndent = value;
                OnPropertiesChanged();
            }
        }
        [DefaultValue(0.0f)]
		public float Position { 
			get { return position; }
			set {
				if(Position == value) return;
				position = Math.Max(0.0f, Math.Min(1.0f, value));
				OnPositionChanged();
			}
		}
        protected bool ShouldSerializeTrackPadding() { return TrackPadding != new Padding(5, 15, 5, 15); }
        public Padding TrackPadding {
            get { return trackPadding; }
            set {
                if (TrackPadding == value) return;
                trackPadding = value;
                OnPropertiesChanged();
            }
        }

        [Browsable(false)]
        public Rectangle TrackBounds { 
            get {
                Rectangle rect = ClientRectangle;
                rect.Offset(TrackPadding.Left, TrackPadding.Top);
                rect.Width -= TrackPadding.Horizontal;
                rect.Height -= TrackPadding.Vertical;
                return rect;
            }
        }

        protected ObjectState UpperThumbState {
            get { return upperThumbState; }
            set {
                if (UpperThumbState == value) return;
                upperThumbState = value;
                OnPropertiesChanged();
            }
        }

        protected ObjectState LowerThumbState {
            get { return lowerThumbState; }
            set {
                if (LowerThumbState == value) return;
                lowerThumbState = value;
                OnPropertiesChanged();
            }
        }

        protected virtual void RaisePositionChanged() { 
            EventHandler handler = Events[positionChanged] as EventHandler;
            if (handler != null) handler(this, EventArgs.Empty);
        }

        public event EventHandler PositionChanged {
            add { Events.AddHandler(positionChanged, value); }
            remove { Events.RemoveHandler(positionChanged, value); }
        }

		protected virtual void OnPositionChanged() {
            RaisePositionChanged();
            OnPropertiesChanged();
        }
		
        protected virtual Color GetResultColor() {
            if (Points.Count == 0) return Color.Transparent;
            if (Points.Count < 2) return Points[0].Color;
            for(int i = 0; i < Points.Count - 1; i++) { 
                if(Position >= Points[i].Offset && Position <= Points[i+1].Offset) {
                    return Color.FromArgb(
                        Points[i].Color.A + (int)((Points[i+1].Color.A - Points[i].Color.A) / (Points[i+1].Offset - Points[i].Offset) * (Position - Points[i].Offset)),
                        Points[i].Color.R + (int)((Points[i+1].Color.R - Points[i].Color.R) / (Points[i+1].Offset - Points[i].Offset) * (Position - Points[i].Offset)),
                        Points[i].Color.G + (int)((Points[i+1].Color.G - Points[i].Color.G) / (Points[i+1].Offset - Points[i].Offset) * (Position - Points[i].Offset)),
                        Points[i].Color.B + (int)((Points[i+1].Color.B - Points[i].Color.B) / (Points[i+1].Offset - Points[i].Offset) * (Position - Points[i].Offset))
                    );
                }
            }
            return Points[Points.Count - 1].Color;
        }

        public Color Value { get { return GetResultColor(); } }

        [Browsable(false)] 
        public Rectangle BorderRect { 
            get {
                Rectangle rect = ClientRectangle;
                rect.Width--;
                rect.Height--;
                return rect;
            }
        }

        public Color TrackFillColor {
            get { return trackFillColor; }
            set {
                if (TrackFillColor == value) return;
                trackFillColor = value;
                OnPropertiesChanged();
            }
        }

        [Browsable(false)]
        public int ThumbX { 
            get { return TrackBounds.X + (int)(TrackBounds.Width * Position); }
        }


        [Browsable(false)]
        public Rectangle UpperThumbBounds { 
            get { return new Rectangle(new Point(ThumbX - ThumbSize.Width / 2, TrackBounds.Y - ThumbIndent - ThumbSize.Height), ThumbSize); }
        }
        [Browsable(false)]
        public Rectangle LowerThumbBounds { 
            get { return new Rectangle(new Point(ThumbX - ThumbSize.Width / 2, TrackBounds.Bottom + ThumbIndent), ThumbSize); }
        }

        protected virtual void DrawBorder(PaintEventArgs e) {
            e.Graphics.DrawRectangle(new Pen(BorderColor), BorderRect);
        }

        protected virtual void ClearTrack(PaintEventArgs e) {
            e.Graphics.FillRectangle(new SolidBrush(TrackFillColor), TrackBounds);
        }

        protected virtual void FillTrack(PaintEventArgs e) {
            ClearTrack(e);
            Rectangle tb = TrackBounds;
            Rectangle rect = TrackBounds;
            for(int i = 0; i < Points.Count - 1; i++) {
                rect.X = tb.X + (int)(tb.Width * Points[i].Offset + 0.5f);
                rect.Width = (int)(tb.Width * (Points[i+1].Offset - Points[i].Offset) + 0.5f);
                if (rect.Width == 0) continue;
                LinearGradientBrush brush = new LinearGradientBrush(rect, Points[i].Color, Points[i + 1].Color, LinearGradientMode.Horizontal);
                e.Graphics.FillRectangle(brush, rect);
            }
        }

        protected virtual void DrawTrackBorder(PaintEventArgs e) {
            e.Graphics.DrawRectangle(new Pen(TrackBorderColor), TrackBounds);
        }

        protected virtual void DrawTrack(PaintEventArgs e) {
            FillTrack(e);
            DrawTrackBorder(e);
        }

        protected Color GetBorderColorByState(ObjectState state) {
            if (state == ObjectState.Normal) return ThumbNormalBorderColor;
            else if (state == ObjectState.Hot) return ThumbHotBorderColor;
            return ThumbPressedBorderColor;
        }

        protected Color GetFillColorByState(ObjectState state) {
            if (state == ObjectState.Normal) return ThumbNormalFillColor;
            else if (state == ObjectState.Hot) return ThumbHotFillColor;
            return ThumbPressedFillColor;
        }

        protected virtual void DrawThumb(PaintEventArgs e, GraphicsPath path, ObjectState state) {
            e.Graphics.FillPath(new SolidBrush(GetFillColorByState(state)), path);
            e.Graphics.DrawPath(new Pen(GetBorderColorByState(state)), path);
        }

        protected virtual void DrawUpperThumb(PaintEventArgs e) { 
            Point[] pt = new Point[4];
            byte[] types = new byte[] { (byte)PathPointType.Line, (byte)PathPointType.Line, (byte)PathPointType.Line, (byte)PathPointType.Line };
            pt[0] = new Point(ThumbX, UpperThumbBounds.Bottom);
            pt[1] = UpperThumbBounds.Location;
            pt[2] = new Point(UpperThumbBounds.Right, UpperThumbBounds.Y);
            pt[3] = pt[0];

            GraphicsPath path = new GraphicsPath(pt, types);
            DrawThumb(e, path, UpperThumbState);
        }

        protected virtual void DrawLowerThumb(PaintEventArgs e) {
            Point[] pt = new Point[4];
            byte[] types = new byte[] { (byte)PathPointType.Line, (byte)PathPointType.Line, (byte)PathPointType.Line, (byte)PathPointType.Line };
            pt[0] = new Point(ThumbX, LowerThumbBounds.Y);
            pt[1] = new Point(LowerThumbBounds.Right, LowerThumbBounds.Bottom);
            pt[2] = new Point(LowerThumbBounds.X, LowerThumbBounds.Bottom);
            pt[3] = pt[0];

            GraphicsPath path = new GraphicsPath(pt, types);
            DrawThumb(e, path, LowerThumbState);
        }

        protected virtual void DrawThumbs(PaintEventArgs e) {
            if (AllowUpperThumb) DrawUpperThumb(e);
            if (AllowLowerThumb) DrawLowerThumb(e);
        }
        
        protected virtual void DrawObject(PaintEventArgs e) {
            DrawBorder(e);
            DrawTrack(e);
            DrawThumbs(e);
        }
        
        protected override void OnPaint(PaintEventArgs e) {
            base.OnPaint(e);
            DrawObject(e);
        }

        protected virtual float GetPositionFromLocation(int x) {
            if (TrackBounds.Width == 0) return 0.0f;
            x = Math.Max(x, TrackBounds.X);
            x = Math.Min(x, TrackBounds.Right);
            return ((float)(x - TrackBounds.X)) / TrackBounds.Width;
        }

        protected override void OnMouseDown(MouseEventArgs e) {
            base.OnMouseDown(e);
            if(e.Button != MouseButtons.Left) return;
            if (TrackBounds.Contains(e.Location)) {
                Position = GetPositionFromLocation(e.Location.X);
                return;
            }
            else if(UpperThumbBounds.Contains(e.Location)) {
                UpperThumbState = ObjectState.Pressed;
            }
            else if(LowerThumbBounds.Contains(e.Location)) {
                LowerThumbState = ObjectState.Pressed;
            }
        }

        protected override void OnMouseUp(MouseEventArgs e) {
            base.OnMouseUp(e);
            UpperThumbState = ObjectState.Normal;
            LowerThumbState = ObjectState.Normal;
        }

        protected override void OnMouseMove(MouseEventArgs e) {
            base.OnMouseMove(e);
            if (UpperThumbState == ObjectState.Pressed || LowerThumbState == ObjectState.Pressed) {
                Position = GetPositionFromLocation(e.Location.X);
                return;
            }
            if (UpperThumbBounds.Contains(e.Location))
                UpperThumbState = ObjectState.Hot;
            else 
                UpperThumbState = ObjectState.Normal;
            if (LowerThumbBounds.Contains(e.Location))
                LowerThumbState = ObjectState.Hot;
            else
                LowerThumbState = ObjectState.Normal;
        }
    }
}
