using sceneEditor;
namespace sceneEditor {
	partial class LandscapePassesControl {
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.passCollectionControl1 = new sceneEditor.PassCollectionControl();
            this.landscapePassInfo1 = new sceneEditor.LandscapePassInfo();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.passCollectionControl1);
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(216, 337);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = " Pass List ";
            // 
            // passCollectionControl1
            // 
            this.passCollectionControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.passCollectionControl1.Landscape = null;
            this.passCollectionControl1.Location = new System.Drawing.Point(3, 16);
            this.passCollectionControl1.Name = "passCollectionControl1";
            this.passCollectionControl1.PassInfoControl = this.landscapePassInfo1;
            this.passCollectionControl1.Size = new System.Drawing.Size(210, 318);
            this.passCollectionControl1.TabIndex = 0;
            // 
            // landscapePassInfo1
            // 
            this.landscapePassInfo1.Enabled = false;
            this.landscapePassInfo1.Location = new System.Drawing.Point(2, 19);
            this.landscapePassInfo1.Name = "landscapePassInfo1";
            this.landscapePassInfo1.PassListControl = this.passCollectionControl1;
            this.landscapePassInfo1.Size = new System.Drawing.Size(467, 256);
            this.landscapePassInfo1.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.landscapePassInfo1);
            this.groupBox2.Location = new System.Drawing.Point(221, 0);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(473, 276);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = " Pass Property ";
            // 
            // LandscapePassesControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "LandscapePassesControl";
            this.Size = new System.Drawing.Size(697, 340);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private PassCollectionControl passCollectionControl1;
		private System.Windows.Forms.GroupBox groupBox2;
		private LandscapePassInfo landscapePassInfo1;
	}
}
