namespace sceneEditor {
	partial class SceneHierarchyControl {
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
			this.sceneView = new System.Windows.Forms.TreeView();
			this.SuspendLayout();
			// 
			// sceneView
			// 
			this.sceneView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.sceneView.BackColor = System.Drawing.SystemColors.Window;
			this.sceneView.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.sceneView.ForeColor = System.Drawing.SystemColors.WindowText;
			this.sceneView.FullRowSelect = true;
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
			this.sceneView.ShowRootLines = false;
			this.sceneView.Size = new System.Drawing.Size(196, 311);
			this.sceneView.TabIndex = 0;
			this.sceneView.DrawNode += new System.Windows.Forms.DrawTreeNodeEventHandler(this.sceneView_DrawNode);
			this.sceneView.BeforeCollapse += new System.Windows.Forms.TreeViewCancelEventHandler(this.sceneView_BeforeCollapse);
			// 
			// SceneHierarchyControl
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.sceneView);
			this.Name = "SceneHierarchyControl";
			this.Size = new System.Drawing.Size(202, 317);
			this.ResumeLayout(false);

		}

		#endregion

		public System.Windows.Forms.TreeView sceneView;
	}
}
