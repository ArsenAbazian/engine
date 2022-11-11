namespace sceneEditor.Lights {
    partial class a3dDirectionalLightControl {
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
            this.vectorControl1 = new sceneEditor.Controls.VectorControl();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.vectorControl1);
            this.groupBox1.Location = new System.Drawing.Point(2, 60);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(221, 61);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = " Direction ";
            // 
            // vectorControl1
            // 
            this.vectorControl1.Location = new System.Drawing.Point(8, 15);
            this.vectorControl1.Name = "vectorControl1";
            this.vectorControl1.Size = new System.Drawing.Size(204, 42);
            this.vectorControl1.TabIndex = 0;
            this.vectorControl1.V0 = 0F;
            this.vectorControl1.V1 = 0F;
            this.vectorControl1.V2 = 0F;
            this.vectorControl1.V3 = 0F;
            // 
            // a3dDirectionalLightControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "a3dDirectionalLightControl";
            this.Size = new System.Drawing.Size(223, 123);
            this.Controls.SetChildIndex(this.groupBox1, 0);
            this.Controls.SetChildIndex(this.lightName, 0);
            this.Controls.SetChildIndex(this.lightColor, 0);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private sceneEditor.Controls.VectorControl vectorControl1;

    }
}
