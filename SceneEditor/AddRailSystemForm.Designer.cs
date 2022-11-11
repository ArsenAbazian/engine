namespace sceneEditor {
	partial class AddRailSystemForm {
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.railSystemInfoControl1 = new sceneEditor.RailSystemInfoControl();
            this.cancelButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.railSystemInfoControl1);
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(440, 200);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = " Rail System Params ";
            // 
            // railSystemInfoControl1
            // 
            this.railSystemInfoControl1.LineEffectFileName = "";
            this.railSystemInfoControl1.LineModelFileName = "";
            this.railSystemInfoControl1.Location = new System.Drawing.Point(5, 17);
            this.railSystemInfoControl1.Name = "railSystemInfoControl1";
            this.railSystemInfoControl1.RailFileName = "";
            this.railSystemInfoControl1.RailSystemName = "";
            this.railSystemInfoControl1.Round2EffectFileName = "";
            this.railSystemInfoControl1.RoundEffectFileName = "";
            this.railSystemInfoControl1.RoundModelFileName = "";
            this.railSystemInfoControl1.Size = new System.Drawing.Size(429, 181);
            this.railSystemInfoControl1.TabIndex = 0;
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(365, 206);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 1;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // okButton
            // 
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.Location = new System.Drawing.Point(284, 206);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 2;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // AddRailSystemForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(441, 230);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "AddRailSystemForm";
            this.Text = "AddRailSystemForm";
            this.TopMost = true;
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private RailSystemInfoControl railSystemInfoControl1;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.Button okButton;
	}
}