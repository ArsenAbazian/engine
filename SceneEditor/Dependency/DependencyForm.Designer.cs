namespace sceneEditor.Dependency {
    partial class DependencyForm {
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
            this.dependencyPropertyControl1 = new sceneEditor.Dependency.DependencyPropertyControl();
            this.SuspendLayout();
            // 
            // dependencyPropertyControl1
            // 
            this.dependencyPropertyControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dependencyPropertyControl1.Location = new System.Drawing.Point(0, 0);
            this.dependencyPropertyControl1.Name = "dependencyPropertyControl1";
            this.dependencyPropertyControl1.Size = new System.Drawing.Size(200, 348);
            this.dependencyPropertyControl1.TabIndex = 0;
            // 
            // DependencyForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(200, 348);
            this.Controls.Add(this.dependencyPropertyControl1);
            this.Name = "DependencyForm";
            this.Text = "DependencyForm";
            this.ResumeLayout(false);

        }

        #endregion

        private DependencyPropertyControl dependencyPropertyControl1;

    }
}