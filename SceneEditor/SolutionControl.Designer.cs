namespace sceneEditor {
	partial class SolutionControl {
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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Textures");
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("Effects");
            System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("Models");
            System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("Landscapes");
            System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("Particle Systems");
            System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode("Railway Systems");
            System.Windows.Forms.TreeNode treeNode7 = new System.Windows.Forms.TreeNode("Lights");
            System.Windows.Forms.TreeNode treeNode8 = new System.Windows.Forms.TreeNode("Fonts");
            System.Windows.Forms.TreeNode treeNode9 = new System.Windows.Forms.TreeNode("Sounds");
            System.Windows.Forms.TreeNode treeNode10 = new System.Windows.Forms.TreeNode("Scene", new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2,
            treeNode3,
            treeNode4,
            treeNode5,
            treeNode6,
            treeNode7,
            treeNode8,
            treeNode9});
            this.sceneView = new sceneEditor.SceneContentControl();
            this.menu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.fullExpandCollapseMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.propMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.changeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.updateViewMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dependencyMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.sceneViewPage = new System.Windows.Forms.TabPage();
            this.propPage = new System.Windows.Forms.TabPage();
            this.propGrid = new System.Windows.Forms.PropertyGrid();
            this.menu.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.sceneViewPage.SuspendLayout();
            this.propPage.SuspendLayout();
            this.SuspendLayout();
            // 
            // sceneView
            // 
            this.sceneView.BackColor = System.Drawing.SystemColors.Window;
            this.sceneView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.sceneView.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.sceneView.ForeColor = System.Drawing.SystemColors.WindowText;
            this.sceneView.FullRowSelect = true;
            this.sceneView.ImageIndex = 0;
            this.sceneView.LineColor = System.Drawing.Color.Gray;
            this.sceneView.Location = new System.Drawing.Point(3, 3);
            this.sceneView.Name = "sceneView";
            treeNode1.Name = "texturesNode";
            treeNode1.Text = "Textures";
            treeNode2.Name = "effectsNode";
            treeNode2.Text = "Effects";
            treeNode3.Name = "modelsNode";
            treeNode3.Text = "Models";
            treeNode4.Name = "landNode";
            treeNode4.Text = "Landscapes";
            treeNode5.Name = "psNode";
            treeNode5.Text = "Particle Systems";
            treeNode6.Name = "rsNode";
            treeNode6.Text = "Railway Systems";
            treeNode7.Name = "lightNode";
            treeNode7.Text = "Lights";
            treeNode8.Name = "fontNode";
            treeNode8.Text = "Fonts";
            treeNode9.Name = "soundNode";
            treeNode9.Text = "Sounds";
            treeNode10.Name = "sceneNone";
            treeNode10.Text = "Scene";
            this.sceneView.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode10});
            this.sceneView.SelectedImageIndex = 0;
            this.sceneView.ShowRootLines = false;
            this.sceneView.Size = new System.Drawing.Size(195, 285);
            this.sceneView.TabIndex = 0;
            this.sceneView.BeforeCollapse += new System.Windows.Forms.TreeViewCancelEventHandler(this.sceneView_BeforeCollapse);
            this.sceneView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.sceneView_AfterSelect);
            this.sceneView.MouseDown += new System.Windows.Forms.MouseEventHandler(this.sceneView_MouseDown);
            // 
            // menu
            // 
            this.menu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fullExpandCollapseMenuItem,
            this.propMenuItem,
            this.addMenuItem,
            this.changeToolStripMenuItem,
            this.removeMenuItem,
            this.updateViewMenuItem,
            this.dependencyMenuItem});
            this.menu.Name = "menu";
            this.menu.Size = new System.Drawing.Size(191, 180);
            // 
            // fullExpandCollapseMenuItem
            // 
            this.fullExpandCollapseMenuItem.Name = "fullExpandCollapseMenuItem";
            this.fullExpandCollapseMenuItem.Size = new System.Drawing.Size(190, 22);
            this.fullExpandCollapseMenuItem.Text = "&Full Expand / Collapse";
            this.fullExpandCollapseMenuItem.Click += new System.EventHandler(this.fullExpandCollapseMenuItem_Click);
            // 
            // propMenuItem
            // 
            this.propMenuItem.Name = "propMenuItem";
            this.propMenuItem.Size = new System.Drawing.Size(190, 22);
            this.propMenuItem.Text = "&Properties";
            this.propMenuItem.Click += new System.EventHandler(this.propMenuItem_Click);
            // 
            // addMenuItem
            // 
            this.addMenuItem.Name = "addMenuItem";
            this.addMenuItem.Size = new System.Drawing.Size(190, 22);
            this.addMenuItem.Text = "&Add New";
            this.addMenuItem.Click += new System.EventHandler(this.addMenuItem_Click);
            // 
            // changeToolStripMenuItem
            // 
            this.changeToolStripMenuItem.Name = "changeToolStripMenuItem";
            this.changeToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.changeToolStripMenuItem.Text = "&Change";
            this.changeToolStripMenuItem.Click += new System.EventHandler(this.changeToolStripMenuItem_Click);
            // 
            // removeMenuItem
            // 
            this.removeMenuItem.Name = "removeMenuItem";
            this.removeMenuItem.Size = new System.Drawing.Size(190, 22);
            this.removeMenuItem.Text = "&Remove";
            // 
            // updateViewMenuItem
            // 
            this.updateViewMenuItem.Name = "updateViewMenuItem";
            this.updateViewMenuItem.Size = new System.Drawing.Size(190, 22);
            this.updateViewMenuItem.Text = "&UpdateView";
            this.updateViewMenuItem.Click += new System.EventHandler(this.updateViewMenuItem_Click);
            // 
            // dependencyMenuItem
            // 
            this.dependencyMenuItem.Name = "dependencyMenuItem";
            this.dependencyMenuItem.Size = new System.Drawing.Size(190, 22);
            this.dependencyMenuItem.Text = "Dependency";
            this.dependencyMenuItem.Click += new System.EventHandler(this.dependencyMenuItem_Click);
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.sceneViewPage);
            this.tabControl.Controls.Add(this.propPage);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.Multiline = true;
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(209, 317);
            this.tabControl.TabIndex = 1;
            // 
            // sceneViewPage
            // 
            this.sceneViewPage.Controls.Add(this.sceneView);
            this.sceneViewPage.Location = new System.Drawing.Point(4, 22);
            this.sceneViewPage.Name = "sceneViewPage";
            this.sceneViewPage.Padding = new System.Windows.Forms.Padding(3);
            this.sceneViewPage.Size = new System.Drawing.Size(201, 291);
            this.sceneViewPage.TabIndex = 0;
            this.sceneViewPage.Text = "Scene View";
            this.sceneViewPage.UseVisualStyleBackColor = true;
            // 
            // propPage
            // 
            this.propPage.Controls.Add(this.propGrid);
            this.propPage.Location = new System.Drawing.Point(4, 22);
            this.propPage.Name = "propPage";
            this.propPage.Padding = new System.Windows.Forms.Padding(3);
            this.propPage.Size = new System.Drawing.Size(201, 291);
            this.propPage.TabIndex = 1;
            this.propPage.Text = "Properties";
            this.propPage.UseVisualStyleBackColor = true;
            // 
            // propGrid
            // 
            this.propGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propGrid.Location = new System.Drawing.Point(3, 3);
            this.propGrid.Name = "propGrid";
            this.propGrid.Size = new System.Drawing.Size(195, 285);
            this.propGrid.TabIndex = 0;
            // 
            // SolutionControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControl);
            this.Name = "SolutionControl";
            this.Size = new System.Drawing.Size(209, 317);
            this.menu.ResumeLayout(false);
            this.tabControl.ResumeLayout(false);
            this.sceneViewPage.ResumeLayout(false);
            this.propPage.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

		public SceneContentControl sceneView;
        private System.Windows.Forms.ContextMenuStrip menu;
        private System.Windows.Forms.ToolStripMenuItem propMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeMenuItem;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage sceneViewPage;
        private System.Windows.Forms.TabPage propPage;
        private System.Windows.Forms.PropertyGrid propGrid;
        private System.Windows.Forms.ToolStripMenuItem updateViewMenuItem;
        private System.Windows.Forms.ToolStripMenuItem changeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fullExpandCollapseMenuItem;
        private System.Windows.Forms.ToolStripMenuItem dependencyMenuItem;
	}
}
