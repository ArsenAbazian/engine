namespace sceneEditor.ShaderParams {
    partial class FloatShaderParam {
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
            ((System.ComponentModel.ISupportInitialize)(this.valueText)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.valueSlider)).BeginInit();
            this.paramBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // valueText
            // 
            this.valueText.DecimalPlaces = 5;
            this.valueText.Increment = new decimal(new int[] {
            1,
            0,
            0,
            196608});
            // 
            // FloatShaderParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "FloatShaderParam";
            ((System.ComponentModel.ISupportInitialize)(this.valueText)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.valueSlider)).EndInit();
            this.paramBox.ResumeLayout(false);
            this.paramBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
    }
}
