using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel;

namespace imageGallery {
	
	public class ColorPicker : ComboBox {
		private static readonly object colorChanged = new object();
        Color borderColor;

		public ColorPicker() {
			Text = "";
			DropDownStyle = ComboBoxStyle.DropDownList;
			DrawMode = DrawMode.OwnerDrawVariable;
		}
		
        Color color;
        public Color Color {
            get { return color; }
            set {
                if (Color == value) return;
                color = value;
                BackColor = Color.FromArgb(255, value);
                OnColorChanged();
            }
        }

        public Color BorderColor {
			get { return borderColor; }
			set {
				borderColor = value;
				OnBorderColorChanged();
			}
		}
		protected virtual void OnBorderColorChanged() {
			Invalidate();
		}

        protected virtual void RaiseColorChanged() {
            EventHandler handler = Events[colorChanged] as EventHandler;
            if (handler != null) handler(this, EventArgs.Empty);
        }

		public event EventHandler ColorChanged {
            add { Events.AddHandler(colorChanged, value); }
            remove { Events.RemoveHandler(colorChanged, value); }
        }

        protected virtual void OnColorChanged() {
            RaiseColorChanged();
            Invalidate();
		}
		[Browsable(false)]
		public Rectangle BorderBounds { 
			get { 
				Rectangle rect = ClientRectangle;
				rect.Width--;
				rect.Height--;
				return rect;
			} 
		}

		protected override void OnPaint(PaintEventArgs e) {
			base.OnPaint(e);
			e.Graphics.DrawRectangle(new Pen(BorderColor), BorderBounds);
		}

        protected virtual ColorEditorForm CreateColorForm() {
            ColorEditorForm cf = new ColorEditorForm();
            cf.OwnerEditor = this;
            cf.Disposed += new EventHandler(OnColorFormDisposed);
            cf.ColorEditor.ValueChanged += new EventHandler(OnEditValueChanged);
            return cf;
        }
        
        protected virtual void OnColorFormDisposed(object sender, EventArgs e) {
            ColorForm.ColorEditor.ValueChanged -= new EventHandler(OnEditValueChanged);
            ColorForm.Disposed -= new EventHandler(OnColorFormDisposed);
            Invalidate();
        }

        protected virtual void OnEditValueChanged(object sender, EventArgs e) {
            Color = ColorForm.ColorEditor.Color;
        }

        ColorEditorForm colorForm;
        public ColorEditorForm ColorForm { 
            get {
                if (colorForm == null || colorForm.IsDisposed)
                    colorForm = CreateColorForm();
                return colorForm;
            }
        }

        protected override void OnDropDown(EventArgs e) {
            base.OnDropDown(e);
            ColorForm.ColorEditor.Color = Color;
            if (ColorForm.IsDisposed) return;
            ColorForm.Show();
        }
	}
}
