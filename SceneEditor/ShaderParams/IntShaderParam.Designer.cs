namespace sceneEditor.ShaderParams {
    partial class IntShaderParam {
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
            this.valueEditor = new System.Windows.Forms.NumericUpDown();
            this.paramBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.valueEditor)).BeginInit();
            this.SuspendLayout();
            // 
            // paramBox
            // 
            this.paramBox.Controls.Add(this.valueEditor);
            this.paramBox.Size = new System.Drawing.Size(202, 43);
            this.paramBox.Controls.SetChildIndex(this.valueEditor, 0);
            this.paramBox.Controls.SetChildIndex(this.paramEnabled, 0);
            // 
            // valueEditor
            // 
            this.valueEditor.Location = new System.Drawing.Point(77, 18);
            this.valueEditor.Name = "valueEditor";
            this.valueEditor.Size = new System.Drawing.Size(120, 20);
            this.valueEditor.TabIndex = 1;
            this.valueEditor.ValueChanged += new System.EventHandler(this.valueEditor_ValueChanged);
            // 
            // IntShaderParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "IntShaderParam";
            this.Size = new System.Drawing.Size(202, 43);
            this.paramBox.ResumeLayout(false);
            this.paramBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.valueEditor)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        protected System.Windows.Forms.NumericUpDown valueEditor;

    }
}
