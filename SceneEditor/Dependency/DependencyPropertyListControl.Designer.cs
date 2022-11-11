namespace sceneEditor.Dependency {
    partial class DependencyPropertyListControl {
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
            this.components = new System.ComponentModel.Container();
            this.dependencyPropertyList = new System.Windows.Forms.ListBox();
            this.contextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.dependencyValueControl1 = new sceneEditor.Dependency.DependencyValueControl();
            this.showDependencyGraphToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenu.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // dependencyPropertyList
            // 
            this.dependencyPropertyList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.dependencyPropertyList.ContextMenuStrip = this.contextMenu;
            this.dependencyPropertyList.FormattingEnabled = true;
            this.dependencyPropertyList.Location = new System.Drawing.Point(3, 3);
            this.dependencyPropertyList.Name = "dependencyPropertyList";
            this.dependencyPropertyList.Size = new System.Drawing.Size(207, 303);
            this.dependencyPropertyList.TabIndex = 0;
            this.dependencyPropertyList.SelectedIndexChanged += new System.EventHandler(this.dependencyPropertyList_SelectedIndexChanged);
            // 
            // contextMenu
            // 
            this.contextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.showDependencyGraphToolStripMenuItem});
            this.contextMenu.Name = "contextMenuStrip1";
            this.contextMenu.Size = new System.Drawing.Size(207, 48);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.dependencyValueControl1);
            this.groupBox1.Location = new System.Drawing.Point(0, 312);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(213, 49);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = " Value ";
            // 
            // dependencyValueControl1
            // 
            this.dependencyValueControl1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.dependencyValueControl1.Location = new System.Drawing.Point(6, 22);
            this.dependencyValueControl1.Name = "dependencyValueControl1";
            this.dependencyValueControl1.Property = null;
            this.dependencyValueControl1.Size = new System.Drawing.Size(201, 21);
            this.dependencyValueControl1.TabIndex = 0;
            this.dependencyValueControl1.ValueChanged += new System.EventHandler(this.dependencyValueControl1_ValueChanged);
            // 
            // showDependencyGraphToolStripMenuItem
            // 
            this.showDependencyGraphToolStripMenuItem.Name = "showDependencyGraphToolStripMenuItem";
            this.showDependencyGraphToolStripMenuItem.Size = new System.Drawing.Size(206, 22);
            this.showDependencyGraphToolStripMenuItem.Text = "Show Dependency Graph";
            this.showDependencyGraphToolStripMenuItem.Click += new System.EventHandler(this.showDependencyGraphToolStripMenuItem_Click);
            // 
            // DependencyPropertyListControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.dependencyPropertyList);
            this.Name = "DependencyPropertyListControl";
            this.Size = new System.Drawing.Size(213, 364);
            this.contextMenu.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox dependencyPropertyList;
        private System.Windows.Forms.ContextMenuStrip contextMenu;
        private System.Windows.Forms.GroupBox groupBox1;
        private DependencyValueControl dependencyValueControl1;
        private System.Windows.Forms.ToolStripMenuItem showDependencyGraphToolStripMenuItem;
    }
}
