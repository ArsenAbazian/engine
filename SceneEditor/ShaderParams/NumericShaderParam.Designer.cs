namespace sceneEditor.ShaderParams {
    partial class NumericShaderParam {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
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
            this.valueText = new System.Windows.Forms.NumericUpDown();
            this.valueSlider = new System.Windows.Forms.TrackBar();
            this.minLabel = new System.Windows.Forms.Label();
            this.maxLabel = new System.Windows.Forms.Label();
            this.paramBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.valueText)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.valueSlider)).BeginInit();
            this.SuspendLayout();
            // 
            // paramBox
            // 
            this.paramBox.Controls.Add(this.maxLabel);
            this.paramBox.Controls.Add(this.valueText);
            this.paramBox.Controls.Add(this.valueSlider);
            this.paramBox.Controls.Add(this.minLabel);
            this.paramBox.Size = new System.Drawing.Size(203, 84);
            this.paramBox.Controls.SetChildIndex(this.minLabel, 0);
            this.paramBox.Controls.SetChildIndex(this.valueSlider, 0);
            this.paramBox.Controls.SetChildIndex(this.valueText, 0);
            this.paramBox.Controls.SetChildIndex(this.paramEnabled, 0);
            this.paramBox.Controls.SetChildIndex(this.maxLabel, 0);
            // 
            // valueText
            // 
            this.valueText.Location = new System.Drawing.Point(77, 18);
            this.valueText.Name = "valueText";
            this.valueText.Size = new System.Drawing.Size(120, 20);
            this.valueText.TabIndex = 1;
            this.valueText.ValueChanged += new System.EventHandler(this.OnValueTextValueChanged);
            // 
            // valueSlider
            // 
            this.valueSlider.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.valueSlider.AutoSize = false;
            this.valueSlider.Location = new System.Drawing.Point(6, 42);
            this.valueSlider.Maximum = 10000;
            this.valueSlider.Name = "valueSlider";
            this.valueSlider.Size = new System.Drawing.Size(191, 21);
            this.valueSlider.TabIndex = 2;
            this.valueSlider.TickFrequency = 5000;
            this.valueSlider.ValueChanged += new System.EventHandler(this.OnValueSliderValueChanged);
            // 
            // minLabel
            // 
            this.minLabel.AutoSize = true;
            this.minLabel.Location = new System.Drawing.Point(11, 66);
            this.minLabel.Name = "minLabel";
            this.minLabel.Size = new System.Drawing.Size(13, 13);
            this.minLabel.TabIndex = 3;
            this.minLabel.Text = "0";
            // 
            // maxLabel
            // 
            this.maxLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.maxLabel.Location = new System.Drawing.Point(154, 66);
            this.maxLabel.Name = "maxLabel";
            this.maxLabel.Size = new System.Drawing.Size(38, 13);
            this.maxLabel.TabIndex = 4;
            this.maxLabel.Text = "0";
            this.maxLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // NumericShaderParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "NumericShaderParam";
            this.Size = new System.Drawing.Size(203, 84);
            this.paramBox.ResumeLayout(false);
            this.paramBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.valueText)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.valueSlider)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        protected System.Windows.Forms.NumericUpDown valueText;
        protected System.Windows.Forms.TrackBar valueSlider;
        private System.Windows.Forms.Label maxLabel;
        private System.Windows.Forms.Label minLabel;
    }
}
