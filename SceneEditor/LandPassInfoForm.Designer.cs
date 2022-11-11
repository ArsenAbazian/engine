namespace sceneEditor {
	partial class LandPassInfoForm {
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
            this.landscapePassesControl1 = new sceneEditor.LandscapePassesControl();
            this.okButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // landscapePassesControl1
            // 
            this.landscapePassesControl1.Landscape = null;
            this.landscapePassesControl1.Location = new System.Drawing.Point(0, 0);
            this.landscapePassesControl1.Name = "landscapePassesControl1";
            this.landscapePassesControl1.Size = new System.Drawing.Size(697, 340);
            this.landscapePassesControl1.TabIndex = 0;
            // 
            // okButton
            // 
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.Location = new System.Drawing.Point(615, 309);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 1;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // LandPassInfoForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(696, 338);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.landscapePassesControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "LandPassInfoForm";
            this.Text = "LandPassInfoForm";
            this.TopMost = true;
            this.ResumeLayout(false);

		}

		#endregion

		private LandscapePassesControl landscapePassesControl1;
		private System.Windows.Forms.Button okButton;
	}
}