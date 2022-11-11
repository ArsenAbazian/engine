namespace sceneEditor.Dependency {
    partial class DependencyPropertyTreeForm {
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
            this.dependencyPropertyTreeControl1 = new sceneEditor.Dependency.DependencyPropertyTreeControl();
            this.SuspendLayout();
            // 
            // dependencyPropertyTreeControl1
            // 
            this.dependencyPropertyTreeControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dependencyPropertyTreeControl1.Location = new System.Drawing.Point(0, 0);
            this.dependencyPropertyTreeControl1.Name = "dependencyPropertyTreeControl1";
            this.dependencyPropertyTreeControl1.Size = new System.Drawing.Size(662, 374);
            this.dependencyPropertyTreeControl1.TabIndex = 0;
            // 
            // DependencyPropertyTreeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(662, 374);
            this.Controls.Add(this.dependencyPropertyTreeControl1);
            this.Name = "DependencyPropertyTreeForm";
            this.Text = "DependencyPropertyTreeForm";
            this.ResumeLayout(false);

        }

        #endregion

        private DependencyPropertyTreeControl dependencyPropertyTreeControl1;
    }
}