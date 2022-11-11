namespace sceneEditor {
	partial class SceneSettingsForm {
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

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
            this.button1 = new System.Windows.Forms.Button();
            this.sceneSettings1 = new sceneEditor.SceneSettings();
            this.button2 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.button1.Location = new System.Drawing.Point(141, 322);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "OK";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // sceneSettings1
            // 
            this.sceneSettings1.EffectDirectory = "";
            this.sceneSettings1.FontDirectory = "";
            this.sceneSettings1.LandDirectory = "";
            this.sceneSettings1.Location = new System.Drawing.Point(2, 1);
            this.sceneSettings1.MinimumSize = new System.Drawing.Size(295, 283);
            this.sceneSettings1.ModelDirectory = "";
            this.sceneSettings1.Name = "sceneSettings1";
            this.sceneSettings1.ParticleDirectory = "";
            this.sceneSettings1.RailDirectory = "";
            this.sceneSettings1.SceneDataDirectory = "";
            this.sceneSettings1.Size = new System.Drawing.Size(295, 315);
            this.sceneSettings1.SoundDirectory = "";
            this.sceneSettings1.TabIndex = 0;
            this.sceneSettings1.TextureDirectory = "";
            // 
            // button2
            // 
            this.button2.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.button2.Location = new System.Drawing.Point(222, 322);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "Cancel";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // SceneSettingsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(300, 348);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.sceneSettings1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "SceneSettingsForm";
            this.Text = "SceneSettingsForm";
            this.TopMost = true;
            this.ResumeLayout(false);

		}

		#endregion

		private SceneSettings sceneSettings1;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Button button2;
	}
}