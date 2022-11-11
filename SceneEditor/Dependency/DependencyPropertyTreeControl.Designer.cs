namespace sceneEditor.Dependency {
    partial class DependencyPropertyTreeControl {
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
            this.dependencyTree1 = new sceneEditor.Dependency.DependencyTree();
            this.label1 = new System.Windows.Forms.Label();
            this.dependencyPropertyListControl1 = new sceneEditor.Dependency.DependencyPropertyListControl();
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
            this.splitContainer1.Panel1.Controls.Add(this.dependencyTree1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.label1);
            this.splitContainer1.Panel2.Controls.Add(this.dependencyPropertyListControl1);
            this.splitContainer1.Size = new System.Drawing.Size(600, 458);
            this.splitContainer1.SplitterDistance = 332;
            this.splitContainer1.TabIndex = 0;
            // 
            // dependencyTree1
            // 
            this.dependencyTree1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dependencyTree1.Location = new System.Drawing.Point(0, 0);
            this.dependencyTree1.Name = "dependencyTree1";
            this.dependencyTree1.RootNode = null;
            this.dependencyTree1.Size = new System.Drawing.Size(332, 458);
            this.dependencyTree1.TabIndex = 0;
            this.dependencyTree1.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.dependencyTree1_AfterSelect);
            // 
            // label1
            // 
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label1.ForeColor = System.Drawing.SystemColors.WindowText;
            this.label1.Location = new System.Drawing.Point(0, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(264, 458);
            this.label1.TabIndex = 1;
            this.label1.Text = "There is no DependencyPropertyOwner for this node";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.label1.Visible = false;
            // 
            // dependencyPropertyListControl1
            // 
            this.dependencyPropertyListControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dependencyPropertyListControl1.Location = new System.Drawing.Point(0, 0);
            this.dependencyPropertyListControl1.Name = "dependencyPropertyListControl1";
            this.dependencyPropertyListControl1.PropertyControl = null;
            this.dependencyPropertyListControl1.PropertyOwner = null;
            this.dependencyPropertyListControl1.Size = new System.Drawing.Size(264, 458);
            this.dependencyPropertyListControl1.TabIndex = 0;
            // 
            // DependencyPropertyTreeControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Name = "DependencyPropertyTreeControl";
            this.Size = new System.Drawing.Size(600, 458);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private DependencyTree dependencyTree1;
        private DependencyPropertyListControl dependencyPropertyListControl1;
        private System.Windows.Forms.Label label1;
    }
}
