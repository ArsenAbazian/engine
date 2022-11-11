namespace sceneEditor.ShaderParams {
    partial class TextureShaderParam {
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.imgButton = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.paramBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // paramBox
            // 
            this.paramBox.Controls.Add(this.imgButton);
            this.paramBox.Controls.Add(this.pictureBox1);
            this.paramBox.Controls.SetChildIndex(this.pictureBox1, 0);
            this.paramBox.Controls.SetChildIndex(this.imgButton, 0);
            this.paramBox.Controls.SetChildIndex(this.paramEnabled, 0);
            // 
            // imgButton
            // 
            this.imgButton.Location = new System.Drawing.Point(77, 15);
            this.imgButton.Name = "imgButton";
            this.imgButton.Size = new System.Drawing.Size(75, 23);
            this.imgButton.TabIndex = 1;
            this.imgButton.Text = "Change...";
            this.imgButton.UseVisualStyleBackColor = true;
            this.imgButton.Click += new System.EventHandler(this.imgButton_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox1.BackColor = System.Drawing.Color.White;
            this.pictureBox1.Location = new System.Drawing.Point(6, 44);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(287, 100);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1.TabIndex = 2;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseDown);
            // 
            // TextureShaderParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.Name = "TextureShaderParam";
            this.paramBox.ResumeLayout(false);
            this.paramBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button imgButton;
        private System.Windows.Forms.PictureBox pictureBox1;
    }
}
