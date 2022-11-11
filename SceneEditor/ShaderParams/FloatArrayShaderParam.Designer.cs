namespace sceneEditor.ShaderParams {
    partial class FloatArrayShaderParam {
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
            this.itemsList = new System.Windows.Forms.ListBox();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.paramBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.SuspendLayout();
            // 
            // paramBox
            // 
            this.paramBox.Controls.Add(this.numericUpDown1);
            this.paramBox.Controls.Add(this.itemsList);
            this.paramBox.Size = new System.Drawing.Size(202, 130);
            this.paramBox.Controls.SetChildIndex(this.paramEnabled, 0);
            this.paramBox.Controls.SetChildIndex(this.itemsList, 0);
            this.paramBox.Controls.SetChildIndex(this.numericUpDown1, 0);
            // 
            // itemsList
            // 
            this.itemsList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.itemsList.FormattingEnabled = true;
            this.itemsList.Location = new System.Drawing.Point(7, 44);
            this.itemsList.Name = "itemsList";
            this.itemsList.Size = new System.Drawing.Size(190, 82);
            this.itemsList.TabIndex = 3;
            this.itemsList.SelectedIndexChanged += new System.EventHandler(this.itemsList_SelectedIndexChanged);
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Location = new System.Drawing.Point(78, 18);
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(119, 20);
            this.numericUpDown1.TabIndex = 4;
            this.numericUpDown1.ValueChanged += new System.EventHandler(this.numericUpDown1_ValueChanged);
            // 
            // FloatArrayShaderParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "FloatArrayShaderParam";
            this.Size = new System.Drawing.Size(202, 130);
            this.paramBox.ResumeLayout(false);
            this.paramBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox itemsList;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
    }
}
