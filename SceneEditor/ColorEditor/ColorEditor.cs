using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace imageGallery {
	public partial class ColorEditor : UserControl {
        private static readonly object valueChanged = new object();
        
        public ColorEditor() {
			InitializeComponent();
		}
	
        public event EventHandler ValueChanged {
            add { Events.AddHandler(valueChanged, value); }
            remove { Events.RemoveHandler(valueChanged, value); }
        }

        protected virtual void RaiseValueChanged() {
            EventHandler handler = Events[valueChanged] as EventHandler;
            if (handler != null) handler(this, EventArgs.Empty);
        }
        
        public Color Color { 
            get { return Color.FromArgb(alphaSlider.Value.A, redSlider.Value.R, greenSlider.Value.G, blueSlider.Value.B); }
            set { UpdateEditors(value); }
        }

        protected virtual void UpdateEditors(Color c) {
            redSlider.Points[0].Color = Color.FromArgb(255, redSlider.Points[0].Color.R, c.G, c.B);
            redSlider.Points[1].Color = Color.FromArgb(255, redSlider.Points[1].Color.R, c.G, c.B);

            greenSlider.Points[0].Color = Color.FromArgb(255, c.R, greenSlider.Points[0].Color.G, c.B);
            greenSlider.Points[1].Color = Color.FromArgb(255, c.R, greenSlider.Points[1].Color.G, c.B);

            blueSlider.Points[0].Color = Color.FromArgb(255, c.R, c.G, blueSlider.Points[0].Color.B);
            blueSlider.Points[1].Color = Color.FromArgb(255, c.R, c.G, blueSlider.Points[1].Color.B);

            if(!suppressRed) redSlider.Position = (float)c.R / 255.0f;
            if(!suppressGreen) greenSlider.Position = (float)c.G / 255.0f;
            if(!suppressBlue) blueSlider.Position = (float)c.B / 255.0f;
            if(!suppressAlpha) alphaSlider.Position = (float)c.A / 255.0f;

            if(!suppressRed) redText.Value = new decimal(redSlider.Position);
            if(!suppressGreen) greenText.Value = new decimal(greenSlider.Position);
            if(!suppressBlue) blueText.Value = new decimal(blueSlider.Position);
            if(!suppressAlpha) alphaText.Value = new decimal(alphaSlider.Position);

            resultSlider.Points[0].Color = c;
            resultSlider.Points[1].Color = c;
            redSlider.Invalidate();
            greenSlider.Invalidate();
            alphaSlider.Invalidate();
            blueSlider.Invalidate();
            resultSlider.Invalidate();
        }

        private void redSlider_PositionChanged(object sender, EventArgs e) {
            UpdateEditors(Color.FromArgb(alphaSlider.Value.A, redSlider.Value.R, greenSlider.Value.G, blueSlider.Value.B));
            RaiseValueChanged();
        }

        private void greenSlider_PositionChanged(object sender, EventArgs e) {
            UpdateEditors(Color.FromArgb(alphaSlider.Value.A, redSlider.Value.R, greenSlider.Value.G, blueSlider.Value.B));
            RaiseValueChanged();
        }

        private void blueSlider_PositionChanged(object sender, EventArgs e) {
            UpdateEditors(Color.FromArgb(alphaSlider.Value.A, redSlider.Value.R, greenSlider.Value.G, blueSlider.Value.B));
            RaiseValueChanged();
        }

        private void alphaSlider_PositionChanged(object sender, EventArgs e) {
            UpdateEditors(Color.FromArgb(alphaSlider.Value.A, redSlider.Value.R, greenSlider.Value.G, blueSlider.Value.B));
            RaiseValueChanged();
        }

        private void colorSlider5_PositionChanged(object sender, EventArgs e) {
            UpdateEditors(Color.FromArgb(alphaSlider.Value.A, commonSlider.Value));
            RaiseValueChanged();
        }

        bool suppressRed = false;
        bool suppressGreen = false;
        bool suppressBlue = false;
        bool suppressAlpha = false;
        private void redText_ValueChanged(object sender, EventArgs e) {
            if (suppressRed) return;
            suppressRed = true;
            redSlider.Position = float.Parse(redText.Value.ToString());
            suppressRed = false;
        }

        private void greenText_ValueChanged(object sender, EventArgs e) {
            if (suppressGreen) return;
            suppressGreen = true;
            greenSlider.Position = float.Parse(greenText.Value.ToString());
            suppressGreen = false;
        }

        private void blueText_ValueChanged(object sender, EventArgs e) {
            if (suppressBlue) return;
            suppressBlue = true;
            blueSlider.Position = float.Parse(blueText.Value.ToString());
            suppressBlue = false;
        }

        private void alphaText_ValueChanged(object sender, EventArgs e) {
            if (suppressAlpha) return;
            suppressAlpha = true;
            alphaSlider.Position = float.Parse(alphaText.Value.ToString());
            suppressAlpha = false;
        }
    }
}
