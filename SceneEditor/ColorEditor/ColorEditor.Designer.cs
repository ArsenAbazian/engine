namespace imageGallery {
	partial class ColorEditor {
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if(disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
            imageGallery.ColorPoint colorPoint1 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint2 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint3 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint4 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint5 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint6 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint7 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint8 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint9 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint10 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint11 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint12 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint13 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint14 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint15 = new imageGallery.ColorPoint();
            imageGallery.ColorPoint colorPoint16 = new imageGallery.ColorPoint();
            this.redText = new System.Windows.Forms.NumericUpDown();
            this.greenText = new System.Windows.Forms.NumericUpDown();
            this.blueText = new System.Windows.Forms.NumericUpDown();
            this.alphaText = new System.Windows.Forms.NumericUpDown();
            this.resultSlider = new imageGallery.ColorSlider();
            this.commonSlider = new imageGallery.ColorSlider();
            this.alphaSlider = new imageGallery.ColorSlider();
            this.blueSlider = new imageGallery.ColorSlider();
            this.greenSlider = new imageGallery.ColorSlider();
            this.redSlider = new imageGallery.ColorSlider();
            ((System.ComponentModel.ISupportInitialize)(this.redText)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.greenText)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.blueText)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.alphaText)).BeginInit();
            this.SuspendLayout();
            // 
            // redText
            // 
            this.redText.DecimalPlaces = 3;
            this.redText.Increment = new decimal(new int[] {
            1,
            0,
            0,
            196608});
            this.redText.Location = new System.Drawing.Point(273, 9);
            this.redText.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.redText.Name = "redText";
            this.redText.Size = new System.Drawing.Size(58, 20);
            this.redText.TabIndex = 6;
            this.redText.ValueChanged += new System.EventHandler(this.redText_ValueChanged);
            // 
            // greenText
            // 
            this.greenText.DecimalPlaces = 3;
            this.greenText.Increment = new decimal(new int[] {
            1,
            0,
            0,
            196608});
            this.greenText.Location = new System.Drawing.Point(273, 37);
            this.greenText.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.greenText.Name = "greenText";
            this.greenText.Size = new System.Drawing.Size(58, 20);
            this.greenText.TabIndex = 7;
            this.greenText.ValueChanged += new System.EventHandler(this.greenText_ValueChanged);
            // 
            // blueText
            // 
            this.blueText.DecimalPlaces = 3;
            this.blueText.Increment = new decimal(new int[] {
            1,
            0,
            0,
            196608});
            this.blueText.Location = new System.Drawing.Point(273, 65);
            this.blueText.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.blueText.Name = "blueText";
            this.blueText.Size = new System.Drawing.Size(58, 20);
            this.blueText.TabIndex = 8;
            this.blueText.ValueChanged += new System.EventHandler(this.blueText_ValueChanged);
            // 
            // alphaText
            // 
            this.alphaText.DecimalPlaces = 3;
            this.alphaText.Increment = new decimal(new int[] {
            1,
            0,
            0,
            196608});
            this.alphaText.Location = new System.Drawing.Point(273, 93);
            this.alphaText.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.alphaText.Name = "alphaText";
            this.alphaText.Size = new System.Drawing.Size(58, 20);
            this.alphaText.TabIndex = 9;
            this.alphaText.ValueChanged += new System.EventHandler(this.alphaText_ValueChanged);
            // 
            // resultSlider
            // 
            this.resultSlider.AllowLowerThumb = false;
            this.resultSlider.AllowUpperThumb = false;
            this.resultSlider.Location = new System.Drawing.Point(304, 115);
            this.resultSlider.Margin = new System.Windows.Forms.Padding(1);
            this.resultSlider.Name = "resultSlider";
            colorPoint1.Color = System.Drawing.Color.Transparent;
            colorPoint1.Holder = null;
            colorPoint1.Offset = 0F;
            colorPoint2.Color = System.Drawing.Color.Transparent;
            colorPoint2.Holder = null;
            colorPoint2.Offset = 1F;
            this.resultSlider.Points.Add(colorPoint1);
            this.resultSlider.Points.Add(colorPoint2);
            this.resultSlider.Size = new System.Drawing.Size(27, 18);
            this.resultSlider.TabIndex = 5;
            this.resultSlider.Text = "colorSlider6";
            this.resultSlider.TrackBorderColor = System.Drawing.Color.Empty;
            this.resultSlider.TrackFillColor = System.Drawing.Color.Empty;
            this.resultSlider.TrackPadding = new System.Windows.Forms.Padding(0);
            // 
            // commonSlider
            // 
            this.commonSlider.AllowLowerThumb = false;
            this.commonSlider.AllowUpperThumb = false;
            this.commonSlider.Location = new System.Drawing.Point(3, 115);
            this.commonSlider.Margin = new System.Windows.Forms.Padding(1);
            this.commonSlider.Name = "commonSlider";
            colorPoint3.Color = System.Drawing.Color.Red;
            colorPoint3.Holder = null;
            colorPoint3.Offset = 0F;
            colorPoint4.Color = System.Drawing.Color.Yellow;
            colorPoint4.Holder = null;
            colorPoint4.Offset = 0.2F;
            colorPoint5.Color = System.Drawing.Color.Lime;
            colorPoint5.Holder = null;
            colorPoint5.Offset = 0.4F;
            colorPoint6.Color = System.Drawing.Color.Cyan;
            colorPoint6.Holder = null;
            colorPoint6.Offset = 0.6F;
            colorPoint7.Color = System.Drawing.Color.Blue;
            colorPoint7.Holder = null;
            colorPoint7.Offset = 0.8F;
            colorPoint8.Color = System.Drawing.Color.Fuchsia;
            colorPoint8.Holder = null;
            colorPoint8.Offset = 1F;
            this.commonSlider.Points.Add(colorPoint3);
            this.commonSlider.Points.Add(colorPoint4);
            this.commonSlider.Points.Add(colorPoint5);
            this.commonSlider.Points.Add(colorPoint6);
            this.commonSlider.Points.Add(colorPoint7);
            this.commonSlider.Points.Add(colorPoint8);
            this.commonSlider.Size = new System.Drawing.Size(299, 18);
            this.commonSlider.TabIndex = 4;
            this.commonSlider.Text = "colorSlider5";
            this.commonSlider.TrackBorderColor = System.Drawing.Color.Empty;
            this.commonSlider.TrackFillColor = System.Drawing.Color.Empty;
            this.commonSlider.TrackPadding = new System.Windows.Forms.Padding(5, 2, 5, 2);
            this.commonSlider.PositionChanged += new System.EventHandler(this.colorSlider5_PositionChanged);
            // 
            // alphaSlider
            // 
            this.alphaSlider.AllowUpperThumb = false;
            this.alphaSlider.Location = new System.Drawing.Point(3, 87);
            this.alphaSlider.Margin = new System.Windows.Forms.Padding(1);
            this.alphaSlider.Name = "alphaSlider";
            colorPoint9.Color = System.Drawing.Color.Transparent;
            colorPoint9.Holder = null;
            colorPoint9.Offset = 0F;
            colorPoint10.Color = System.Drawing.Color.White;
            colorPoint10.Holder = null;
            colorPoint10.Offset = 1F;
            this.alphaSlider.Points.Add(colorPoint9);
            this.alphaSlider.Points.Add(colorPoint10);
            this.alphaSlider.Size = new System.Drawing.Size(266, 26);
            this.alphaSlider.TabIndex = 3;
            this.alphaSlider.Text = "colorSlider4";
            this.alphaSlider.TrackBorderColor = System.Drawing.Color.Empty;
            this.alphaSlider.TrackFillColor = System.Drawing.Color.Black;
            this.alphaSlider.TrackPadding = new System.Windows.Forms.Padding(5, 2, 5, 15);
            this.alphaSlider.PositionChanged += new System.EventHandler(this.alphaSlider_PositionChanged);
            // 
            // blueSlider
            // 
            this.blueSlider.AllowUpperThumb = false;
            this.blueSlider.Location = new System.Drawing.Point(3, 59);
            this.blueSlider.Margin = new System.Windows.Forms.Padding(1);
            this.blueSlider.Name = "blueSlider";
            colorPoint11.Color = System.Drawing.Color.Black;
            colorPoint11.Holder = null;
            colorPoint11.Offset = 0F;
            colorPoint12.Color = System.Drawing.Color.Blue;
            colorPoint12.Holder = null;
            colorPoint12.Offset = 1F;
            this.blueSlider.Points.Add(colorPoint11);
            this.blueSlider.Points.Add(colorPoint12);
            this.blueSlider.Size = new System.Drawing.Size(266, 26);
            this.blueSlider.TabIndex = 2;
            this.blueSlider.Text = "colorSlider3";
            this.blueSlider.TrackBorderColor = System.Drawing.Color.Empty;
            this.blueSlider.TrackFillColor = System.Drawing.Color.Empty;
            this.blueSlider.TrackPadding = new System.Windows.Forms.Padding(5, 2, 5, 15);
            this.blueSlider.PositionChanged += new System.EventHandler(this.blueSlider_PositionChanged);
            // 
            // greenSlider
            // 
            this.greenSlider.AllowUpperThumb = false;
            this.greenSlider.Location = new System.Drawing.Point(3, 31);
            this.greenSlider.Margin = new System.Windows.Forms.Padding(1);
            this.greenSlider.Name = "greenSlider";
            colorPoint13.Color = System.Drawing.Color.Black;
            colorPoint13.Holder = null;
            colorPoint13.Offset = 0F;
            colorPoint14.Color = System.Drawing.Color.Lime;
            colorPoint14.Holder = null;
            colorPoint14.Offset = 1F;
            this.greenSlider.Points.Add(colorPoint13);
            this.greenSlider.Points.Add(colorPoint14);
            this.greenSlider.Size = new System.Drawing.Size(266, 26);
            this.greenSlider.TabIndex = 1;
            this.greenSlider.Text = "colorSlider2";
            this.greenSlider.TrackBorderColor = System.Drawing.Color.Empty;
            this.greenSlider.TrackFillColor = System.Drawing.Color.Empty;
            this.greenSlider.TrackPadding = new System.Windows.Forms.Padding(5, 2, 5, 15);
            this.greenSlider.PositionChanged += new System.EventHandler(this.greenSlider_PositionChanged);
            // 
            // redSlider
            // 
            this.redSlider.AllowUpperThumb = false;
            this.redSlider.Location = new System.Drawing.Point(3, 3);
            this.redSlider.Margin = new System.Windows.Forms.Padding(1);
            this.redSlider.Name = "redSlider";
            colorPoint15.Color = System.Drawing.Color.Black;
            colorPoint15.Holder = null;
            colorPoint15.Offset = 0F;
            colorPoint16.Color = System.Drawing.Color.Red;
            colorPoint16.Holder = null;
            colorPoint16.Offset = 1F;
            this.redSlider.Points.Add(colorPoint15);
            this.redSlider.Points.Add(colorPoint16);
            this.redSlider.Size = new System.Drawing.Size(266, 26);
            this.redSlider.TabIndex = 0;
            this.redSlider.Text = "colorSlider1";
            this.redSlider.TrackBorderColor = System.Drawing.Color.Empty;
            this.redSlider.TrackFillColor = System.Drawing.Color.Empty;
            this.redSlider.TrackPadding = new System.Windows.Forms.Padding(5, 2, 5, 15);
            this.redSlider.PositionChanged += new System.EventHandler(this.redSlider_PositionChanged);
            // 
            // ColorEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.alphaText);
            this.Controls.Add(this.blueText);
            this.Controls.Add(this.greenText);
            this.Controls.Add(this.redText);
            this.Controls.Add(this.resultSlider);
            this.Controls.Add(this.commonSlider);
            this.Controls.Add(this.alphaSlider);
            this.Controls.Add(this.blueSlider);
            this.Controls.Add(this.greenSlider);
            this.Controls.Add(this.redSlider);
            this.Name = "ColorEditor";
            this.Size = new System.Drawing.Size(335, 135);
            ((System.ComponentModel.ISupportInitialize)(this.redText)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.greenText)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.blueText)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.alphaText)).EndInit();
            this.ResumeLayout(false);

		}

		#endregion

        private ColorSlider redSlider;
        private ColorSlider greenSlider;
        private ColorSlider blueSlider;
        private ColorSlider alphaSlider;
        private ColorSlider commonSlider;
        private ColorSlider resultSlider;
        private System.Windows.Forms.NumericUpDown redText;
        private System.Windows.Forms.NumericUpDown greenText;
        private System.Windows.Forms.NumericUpDown blueText;
        private System.Windows.Forms.NumericUpDown alphaText;
	}
}
