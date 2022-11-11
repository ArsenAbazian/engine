namespace sceneEditor.ShaderParams {
    partial class BoolArrayShaderParam {
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
            this.valueList = new System.Windows.Forms.CheckedListBox();
            this.paramBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // paramBox
            // 
            this.paramBox.Controls.Add(this.valueList);
            this.paramBox.Size = new System.Drawing.Size(167, 170);
            this.paramBox.Controls.SetChildIndex(this.valueList, 0);
            this.paramBox.Controls.SetChildIndex(this.paramEnabled, 0);
            // 
            // valueList
            // 
            this.valueList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.valueList.FormattingEnabled = true;
            this.valueList.Location = new System.Drawing.Point(3, 42);
            this.valueList.Name = "valueList";
            this.valueList.Size = new System.Drawing.Size(161, 109);
            this.valueList.TabIndex = 1;
            this.valueList.SelectedValueChanged += new System.EventHandler(this.valueList_SelectedValueChanged);
            // 
            // BoolArrayShaderParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "BoolArrayShaderParam";
            this.Size = new System.Drawing.Size(167, 170);
            this.paramBox.ResumeLayout(false);
            this.paramBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.CheckedListBox valueList;
    }
}
