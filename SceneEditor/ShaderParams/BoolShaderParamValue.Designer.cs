namespace sceneEditor.ShaderParams {
    partial class BoolShaderParam {
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
            this.value = new System.Windows.Forms.CheckBox();
            this.paramBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // paramBox
            // 
            this.paramBox.Controls.Add(this.value);
            this.paramBox.Size = new System.Drawing.Size(251, 40);
            this.paramBox.Controls.SetChildIndex(this.value, 0);
            this.paramBox.Controls.SetChildIndex(this.paramEnabled, 0);
            // 
            // value
            // 
            this.value.AutoSize = true;
            this.value.Location = new System.Drawing.Point(77, 19);
            this.value.Name = "value";
            this.value.Size = new System.Drawing.Size(53, 17);
            this.value.TabIndex = 1;
            this.value.Text = "Value";
            this.value.UseVisualStyleBackColor = true;
            this.value.CheckedChanged += new System.EventHandler(this.value_CheckedChanged);
            // 
            // BoolShaderParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "BoolShaderParam";
            this.Size = new System.Drawing.Size(251, 40);
            this.paramBox.ResumeLayout(false);
            this.paramBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        protected System.Windows.Forms.CheckBox value;

    }
}
