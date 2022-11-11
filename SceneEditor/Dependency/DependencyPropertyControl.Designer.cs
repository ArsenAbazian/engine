namespace sceneEditor.Dependency {
    partial class DependencyPropertyControl {
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.dependencyPropertyListControl1 = new sceneEditor.Dependency.DependencyPropertyListControl();
            this.graphControl1 = new sceneEditor.graphViewer.GraphControl();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.dependencyPropertyListControl1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.graphControl1);
            this.splitContainer1.Panel2Collapsed = true;
            this.splitContainer1.Size = new System.Drawing.Size(239, 437);
            this.splitContainer1.SplitterDistance = 208;
            this.splitContainer1.TabIndex = 0;
            // 
            // dependencyPropertyListControl1
            // 
            this.dependencyPropertyListControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dependencyPropertyListControl1.Location = new System.Drawing.Point(0, 0);
            this.dependencyPropertyListControl1.Name = "dependencyPropertyListControl1";
            this.dependencyPropertyListControl1.PropertyControl = null;
            this.dependencyPropertyListControl1.PropertyOwner = null;
            this.dependencyPropertyListControl1.Size = new System.Drawing.Size(239, 437);
            this.dependencyPropertyListControl1.TabIndex = 0;
            // 
            // graphControl1
            // 
            this.graphControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.graphControl1.Location = new System.Drawing.Point(0, 0);
            this.graphControl1.Name = "graphControl1";
            this.graphControl1.Padding = new System.Windows.Forms.Padding(2);
            this.graphControl1.Size = new System.Drawing.Size(96, 100);
            this.graphControl1.TabIndex = 0;
            // 
            // DependencyPropertyControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Name = "DependencyPropertyControl";
            this.Size = new System.Drawing.Size(239, 437);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private DependencyPropertyListControl dependencyPropertyListControl1;
        protected internal sceneEditor.graphViewer.GraphControl graphControl1;
    }
}
