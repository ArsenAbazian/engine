namespace sceneEditor {
	partial class SettingsForm {
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
			this.settingsControl1 = new sceneEditor.SettingsControl();
			this.SuspendLayout();
			// 
			// settingsControl1
			// 
			this.settingsControl1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.settingsControl1.Location = new System.Drawing.Point(0, 0);
			this.settingsControl1.Name = "settingsControl1";
			this.settingsControl1.Size = new System.Drawing.Size(284, 264);
			this.settingsControl1.TabIndex = 0;
			// 
			// SettingsForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(284, 264);
			this.Controls.Add(this.settingsControl1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
			this.Name = "SettingsForm";
			this.Text = "Settings";
			this.TopMost = true;
			this.ResumeLayout(false);

		}

		#endregion

		private SettingsControl settingsControl1;
	}
}