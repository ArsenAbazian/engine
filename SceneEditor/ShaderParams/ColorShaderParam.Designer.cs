namespace sceneEditor.ShaderParams {
    partial class ColorShaderParam {
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
            this.colorPicker1 = new imageGallery.ColorPicker();
            this.paramBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // paramBox
            // 
            this.paramBox.Controls.Add(this.colorPicker1);
            this.paramBox.Size = new System.Drawing.Size(299, 43);
            this.paramBox.Controls.SetChildIndex(this.colorPicker1, 0);
            this.paramBox.Controls.SetChildIndex(this.paramEnabled, 0);
            // 
            // colorPicker1
            // 
            this.colorPicker1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.colorPicker1.BorderColor = System.Drawing.Color.Empty;
            this.colorPicker1.Color = System.Drawing.Color.Empty;
            this.colorPicker1.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.colorPicker1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.colorPicker1.FormattingEnabled = true;
            this.colorPicker1.Location = new System.Drawing.Point(77, 17);
            this.colorPicker1.Name = "colorPicker1";
            this.colorPicker1.Size = new System.Drawing.Size(216, 21);
            this.colorPicker1.TabIndex = 1;
            this.colorPicker1.ColorChanged += new System.EventHandler(this.colorPicker1_ColorChanged);
            // 
            // ColorShaderParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "ColorShaderParam";
            this.Size = new System.Drawing.Size(299, 43);
            this.paramBox.ResumeLayout(false);
            this.paramBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private imageGallery.ColorPicker colorPicker1;
    }
}
