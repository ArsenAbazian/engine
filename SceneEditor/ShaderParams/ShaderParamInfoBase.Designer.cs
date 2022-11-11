namespace sceneEditor.ShaderParams {
    partial class ShaderParamInfoBase {
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
            this.paramBox = new System.Windows.Forms.GroupBox();
            this.paramEnabled = new System.Windows.Forms.CheckBox();
            this.paramBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // paramBox
            // 
            this.paramBox.Controls.Add(this.paramEnabled);
            this.paramBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.paramBox.Location = new System.Drawing.Point(0, 0);
            this.paramBox.Name = "paramBox";
            this.paramBox.Size = new System.Drawing.Size(299, 150);
            this.paramBox.TabIndex = 0;
            this.paramBox.TabStop = false;
            this.paramBox.Text = " Param Name";
            // 
            // paramEnabled
            // 
            this.paramEnabled.AutoSize = true;
            this.paramEnabled.Location = new System.Drawing.Point(6, 19);
            this.paramEnabled.Name = "paramEnabled";
            this.paramEnabled.Size = new System.Drawing.Size(65, 17);
            this.paramEnabled.TabIndex = 0;
            this.paramEnabled.Text = "Enabled";
            this.paramEnabled.UseVisualStyleBackColor = true;
            this.paramEnabled.CheckedChanged += new System.EventHandler(this.paramEnabled_CheckedChanged);
            // 
            // ShaderParamInfoBase
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.paramBox);
            this.Name = "ShaderParamInfoBase";
            this.Size = new System.Drawing.Size(299, 150);
            this.paramBox.ResumeLayout(false);
            this.paramBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        protected System.Windows.Forms.GroupBox paramBox;
        protected System.Windows.Forms.CheckBox paramEnabled;
    }
}
