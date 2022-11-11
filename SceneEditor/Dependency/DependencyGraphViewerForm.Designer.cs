namespace sceneEditor.Dependency {
    partial class DependencyGraphViewerForm {
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
            this.dependencyGraphViewer1 = new sceneEditor.Dependency.DependencyGraphViewer();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.updateViewMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // dependencyGraphViewer1
            // 
            this.dependencyGraphViewer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dependencyGraphViewer1.Location = new System.Drawing.Point(0, 24);
            this.dependencyGraphViewer1.Name = "dependencyGraphViewer1";
            this.dependencyGraphViewer1.Property = null;
            this.dependencyGraphViewer1.Size = new System.Drawing.Size(723, 333);
            this.dependencyGraphViewer1.TabIndex = 0;
            this.dependencyGraphViewer1.Text = "dependencyGraphViewer1";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.viewToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(723, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.updateViewMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
            this.viewToolStripMenuItem.Text = "View";
            // 
            // updateViewMenuItem
            // 
            this.updateViewMenuItem.Name = "updateViewMenuItem";
            this.updateViewMenuItem.Size = new System.Drawing.Size(152, 22);
            this.updateViewMenuItem.Text = "Update View";
            this.updateViewMenuItem.Click += new System.EventHandler(this.updateViewMenuItem_Click);
            // 
            // DependencyGraphViewerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(723, 357);
            this.Controls.Add(this.dependencyGraphViewer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "DependencyGraphViewerForm";
            this.Text = "DependencyGraphViewerForm";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private DependencyGraphViewer dependencyGraphViewer1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem updateViewMenuItem;
    }
}