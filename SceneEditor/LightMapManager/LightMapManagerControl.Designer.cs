namespace sceneEditor.LightMapManager {
    partial class LightMapManagerControl {
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
            System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode("Landscapes");
            System.Windows.Forms.TreeNode treeNode7 = new System.Windows.Forms.TreeNode("Railway Systems");
            System.Windows.Forms.TreeNode treeNode8 = new System.Windows.Forms.TreeNode("Lights");
            System.Windows.Forms.TreeNode treeNode9 = new System.Windows.Forms.TreeNode("ModelRef");
            System.Windows.Forms.TreeNode treeNode10 = new System.Windows.Forms.TreeNode("Scene", new System.Windows.Forms.TreeNode[] {
            treeNode6,
            treeNode7,
            treeNode8,
            treeNode9});
            this.sceneContentControl1 = new sceneEditor.SceneContentControl();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.clearAllButton = new System.Windows.Forms.Button();
            this.checkAllButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.lightMapInfoControl1 = new sceneEditor.LightMapManager.LightMapInfoControl();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // sceneContentControl1
            // 
            this.sceneContentControl1.CheckBoxes = true;
            this.sceneContentControl1.Location = new System.Drawing.Point(6, 49);
            this.sceneContentControl1.Name = "sceneContentControl1";
            treeNode6.Name = "";
            treeNode6.Text = "Landscapes";
            treeNode7.Name = "";
            treeNode7.Text = "Railway Systems";
            treeNode8.Name = "";
            treeNode8.Text = "Lights";
            treeNode9.Name = "";
            treeNode9.Text = "ModelRef";
            treeNode10.Name = "";
            treeNode10.Text = "Scene";
            this.sceneContentControl1.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode10});
            this.sceneContentControl1.OptionsContent.ViewEffects = false;
            this.sceneContentControl1.OptionsContent.ViewFonts = false;
            this.sceneContentControl1.OptionsContent.ViewModels = false;
            this.sceneContentControl1.OptionsContent.ViewParticles = false;
            this.sceneContentControl1.OptionsContent.ViewSounds = false;
            this.sceneContentControl1.OptionsContent.ViewTextures = false;
            this.sceneContentControl1.Size = new System.Drawing.Size(243, 356);
            this.sceneContentControl1.TabIndex = 0;
            this.sceneContentControl1.BeforeCheck += new System.Windows.Forms.TreeViewCancelEventHandler(this.sceneContentControl1_BeforeCheck);
            this.sceneContentControl1.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.sceneContentControl1_AfterSelect);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.clearAllButton);
            this.groupBox1.Controls.Add(this.checkAllButton);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.sceneContentControl1);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(255, 440);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = " LightMap Items ";
            // 
            // clearAllButton
            // 
            this.clearAllButton.Location = new System.Drawing.Point(87, 411);
            this.clearAllButton.Name = "clearAllButton";
            this.clearAllButton.Size = new System.Drawing.Size(75, 23);
            this.clearAllButton.TabIndex = 3;
            this.clearAllButton.Text = "Clear All";
            this.clearAllButton.UseVisualStyleBackColor = true;
            this.clearAllButton.Click += new System.EventHandler(this.clearAllButton_Click);
            // 
            // checkAllButton
            // 
            this.checkAllButton.Location = new System.Drawing.Point(6, 411);
            this.checkAllButton.Name = "checkAllButton";
            this.checkAllButton.Size = new System.Drawing.Size(75, 23);
            this.checkAllButton.TabIndex = 2;
            this.checkAllButton.Text = "Check All";
            this.checkAllButton.UseVisualStyleBackColor = true;
            this.checkAllButton.Click += new System.EventHandler(this.checkAllButton_Click);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(6, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(243, 30);
            this.label1.TabIndex = 1;
            this.label1.Text = "Check items wich will take part in lightmap calculation";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.lightMapInfoControl1);
            this.groupBox2.Location = new System.Drawing.Point(264, 3);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(298, 368);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = " LightMap Parameters ";
            // 
            // lightMapInfoControl1
            // 
            this.lightMapInfoControl1.FileName = "FileName:";
            this.lightMapInfoControl1.IsAtlasTexture = false;
            this.lightMapInfoControl1.LightMapEnabled = false;
            this.lightMapInfoControl1.Location = new System.Drawing.Point(6, 19);
            this.lightMapInfoControl1.Name = "lightMapInfoControl1";
            this.lightMapInfoControl1.OwnerName = null;
            this.lightMapInfoControl1.Size = new System.Drawing.Size(286, 343);
            this.lightMapInfoControl1.TabIndex = 0;
            this.lightMapInfoControl1.TextureCoordIndex = -1;
            this.lightMapInfoControl1.TextureParamName = "";
            // 
            // LightMapManagerControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "LightMapManagerControl";
            this.Size = new System.Drawing.Size(566, 446);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private SceneContentControl sceneContentControl1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button clearAllButton;
        private System.Windows.Forms.Button checkAllButton;
        private System.Windows.Forms.GroupBox groupBox2;
        private LightMapInfoControl lightMapInfoControl1;

    }
}
