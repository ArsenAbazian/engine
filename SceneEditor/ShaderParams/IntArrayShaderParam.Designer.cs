namespace sceneEditor.ShaderParams {
    partial class IntArrayShaderParam {
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
            ((System.ComponentModel.ISupportInitialize)(this.valueEditor)).BeginInit();
            this.paramBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // paramBox
            // 
            this.paramBox.Controls.Add(this.itemsList);
            this.paramBox.Size = new System.Drawing.Size(202, 128);
            this.paramBox.Controls.SetChildIndex(this.valueEditor, 0);
            this.paramBox.Controls.SetChildIndex(this.itemsList, 0);
            this.paramBox.Controls.SetChildIndex(this.paramEnabled, 0);
            // 
            // itemsList
            // 
            this.itemsList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.itemsList.FormattingEnabled = true;
            this.itemsList.Location = new System.Drawing.Point(6, 42);
            this.itemsList.Name = "itemsList";
            this.itemsList.Size = new System.Drawing.Size(190, 82);
            this.itemsList.TabIndex = 2;
            this.itemsList.SelectedIndexChanged += new System.EventHandler(this.itemsList_SelectedIndexChanged);
            // 
            // IntArrayShaderParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.MaximumSize = new System.Drawing.Size(10000, 10000);
            this.Name = "IntArrayShaderParam";
            this.Size = new System.Drawing.Size(202, 128);
            ((System.ComponentModel.ISupportInitialize)(this.valueEditor)).EndInit();
            this.paramBox.ResumeLayout(false);
            this.paramBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox itemsList;
    }
}
