namespace sceneEditor.Dependency {
    partial class SimpleDependencyPropertyControl {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if(disposing && (components != null)) {
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
            this.label1 = new System.Windows.Forms.Label();
            this.dependencyPropertyTreeControl1 = new sceneEditor.Dependency.DependencyPropertyTreeControl();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.SystemColors.ControlDark;
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label1.ForeColor = System.Drawing.Color.Gold;
            this.label1.Location = new System.Drawing.Point(0, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(466, 30);
            this.label1.TabIndex = 0;
            this.label1.Text = "Select Source Dependency Property";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // dependencyPropertyTreeControl1
            // 
            this.dependencyPropertyTreeControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dependencyPropertyTreeControl1.Location = new System.Drawing.Point(0, 30);
            this.dependencyPropertyTreeControl1.Name = "dependencyPropertyTreeControl1";
            this.dependencyPropertyTreeControl1.RootNode = null;
            this.dependencyPropertyTreeControl1.Size = new System.Drawing.Size(466, 255);
            this.dependencyPropertyTreeControl1.TabIndex = 1;
            // 
            // SimpleDependencyPropertyControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.dependencyPropertyTreeControl1);
            this.Controls.Add(this.label1);
            this.Name = "SimpleDependencyPropertyControl";
            this.Size = new System.Drawing.Size(466, 285);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private DependencyPropertyTreeControl dependencyPropertyTreeControl1;
    }
}
