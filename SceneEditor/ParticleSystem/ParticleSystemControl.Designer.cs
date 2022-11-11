namespace sceneEditor.ParticleSystem {
    partial class ParticleSystemControl {
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.particleSystemList = new System.Windows.Forms.ListBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.nameTextBox = new sceneEditor.Controls.ValueTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.particleCountTextBox = new sceneEditor.Controls.ValueTextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.mpCountTextBox = new sceneEditor.Controls.ValueTextBox();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.particleSystemList);
            this.groupBox1.Controls.Add(this.pictureBox1);
            this.groupBox1.Location = new System.Drawing.Point(0, 83);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(323, 183);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = " Particle Type ";
            // 
            // particleSystemList
            // 
            this.particleSystemList.FormattingEnabled = true;
            this.particleSystemList.Items.AddRange(new object[] {
            "Smoke"});
            this.particleSystemList.Location = new System.Drawing.Point(6, 19);
            this.particleSystemList.Name = "particleSystemList";
            this.particleSystemList.Size = new System.Drawing.Size(146, 160);
            this.particleSystemList.TabIndex = 2;
            this.particleSystemList.SelectedIndexChanged += new System.EventHandler(this.particleSystemList_SelectedIndexChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(158, 19);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(160, 160);
            this.pictureBox1.TabIndex = 1;
            this.pictureBox1.TabStop = false;
            // 
            // nameTextBox
            // 
            this.nameTextBox.Location = new System.Drawing.Point(137, 7);
            this.nameTextBox.Name = "nameTextBox";
            this.nameTextBox.Size = new System.Drawing.Size(181, 20);
            this.nameTextBox.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 10);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Name:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 36);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(73, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "ParticleCount:";
            // 
            // particleCountTextBox
            // 
            this.particleCountTextBox.Location = new System.Drawing.Point(137, 33);
            this.particleCountTextBox.Name = "particleCountTextBox";
            this.particleCountTextBox.Size = new System.Drawing.Size(88, 20);
            this.particleCountTextBox.TabIndex = 3;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 62);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(128, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "MaxRenderParticleCount:";
            // 
            // mpCountTextBox
            // 
            this.mpCountTextBox.Location = new System.Drawing.Point(137, 59);
            this.mpCountTextBox.Name = "mpCountTextBox";
            this.mpCountTextBox.Size = new System.Drawing.Size(88, 20);
            this.mpCountTextBox.TabIndex = 5;
            // 
            // ParticleSystemControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.mpCountTextBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.particleCountTextBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.nameTextBox);
            this.Controls.Add(this.groupBox1);
            this.Name = "ParticleSystemControl";
            this.Size = new System.Drawing.Size(323, 268);
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ListBox particleSystemList;
        private System.Windows.Forms.PictureBox pictureBox1;
        private sceneEditor.Controls.ValueTextBox nameTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private sceneEditor.Controls.ValueTextBox particleCountTextBox;
        private System.Windows.Forms.Label label3;
        private sceneEditor.Controls.ValueTextBox mpCountTextBox;
    }
}
