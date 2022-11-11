namespace sceneEditor.ShaderParams {
    partial class TechniqueShaderParam {
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.techList = new System.Windows.Forms.ComboBox();
            this.paramBox.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // paramBox
            // 
            this.paramBox.Controls.Add(this.groupBox1);
            this.paramBox.Size = new System.Drawing.Size(206, 58);
            this.paramBox.Controls.SetChildIndex(this.groupBox1, 0);
            this.paramBox.Controls.SetChildIndex(this.paramEnabled, 0);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.techList);
            this.groupBox1.Location = new System.Drawing.Point(77, 9);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(126, 45);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = " Selected Technique ";
            // 
            // techList
            // 
            this.techList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.techList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.techList.FormattingEnabled = true;
            this.techList.Location = new System.Drawing.Point(6, 19);
            this.techList.Name = "techList";
            this.techList.Size = new System.Drawing.Size(114, 21);
            this.techList.TabIndex = 0;
            this.techList.SelectedIndexChanged += new System.EventHandler(this.techList_SelectedIndexChanged);
            // 
            // TechniqueShaderParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "TechniqueShaderParam";
            this.Size = new System.Drawing.Size(206, 58);
            this.paramBox.ResumeLayout(false);
            this.paramBox.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox techList;
    }
}
