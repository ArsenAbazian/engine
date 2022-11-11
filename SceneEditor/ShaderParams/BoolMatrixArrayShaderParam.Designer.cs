namespace sceneEditor.ShaderParams {
    partial class BoolMatrixArrayShaderParam {
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
            this.itemIndexList = new System.Windows.Forms.ComboBox();
            this.paramBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // paramBox
            // 
            this.paramBox.Controls.Add(this.itemIndexList);
            this.paramBox.Controls.SetChildIndex(this.itemIndexList, 0);
            this.paramBox.Controls.SetChildIndex(this.paramEnabled, 0);
            // 
            // itemIndexList
            // 
            this.itemIndexList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.itemIndexList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.itemIndexList.FormattingEnabled = true;
            this.itemIndexList.Location = new System.Drawing.Point(76, 17);
            this.itemIndexList.Name = "itemIndexList";
            this.itemIndexList.Size = new System.Drawing.Size(121, 21);
            this.itemIndexList.TabIndex = 17;
            this.itemIndexList.SelectedIndexChanged += new System.EventHandler(this.itemIndexList_SelectedIndexChanged);
            // 
            // BoolMatrixArrayShaderParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "BoolMatrixArrayShaderParam";
            this.paramBox.ResumeLayout(false);
            this.paramBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox itemIndexList;
    }
}
