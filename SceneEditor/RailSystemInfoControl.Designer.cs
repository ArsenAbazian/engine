namespace sceneEditor {
	partial class RailSystemInfoControl {
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
            this.lmfButton = new System.Windows.Forms.Button();
            this.lineFileName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.rfnButton = new System.Windows.Forms.Button();
            this.railFileName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.rmfnButton = new System.Windows.Forms.Button();
            this.roundFileName = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.lefnButton = new System.Windows.Forms.Button();
            this.lineEffect = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.refnButton = new System.Windows.Forms.Button();
            this.roundEffect = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.r2efnButton = new System.Windows.Forms.Button();
            this.round2Effect = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.railFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.modelFialog = new System.Windows.Forms.OpenFileDialog();
            this.effectDialog = new System.Windows.Forms.OpenFileDialog();
            this.railName = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lmfButton
            // 
            this.lmfButton.Location = new System.Drawing.Point(393, 52);
            this.lmfButton.Name = "lmfButton";
            this.lmfButton.Size = new System.Drawing.Size(33, 23);
            this.lmfButton.TabIndex = 12;
            this.lmfButton.Text = "...";
            this.lmfButton.UseVisualStyleBackColor = true;
            this.lmfButton.Click += new System.EventHandler(this.lmfButton_Click);
            // 
            // lineFileName
            // 
            this.lineFileName.Location = new System.Drawing.Point(127, 55);
            this.lineFileName.Name = "lineFileName";
            this.lineFileName.ReadOnly = true;
            this.lineFileName.Size = new System.Drawing.Size(260, 20);
            this.lineFileName.TabIndex = 11;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(5, 58);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(103, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "LineModelFileName:";
            // 
            // rfnButton
            // 
            this.rfnButton.Location = new System.Drawing.Point(393, 26);
            this.rfnButton.Name = "rfnButton";
            this.rfnButton.Size = new System.Drawing.Size(33, 23);
            this.rfnButton.TabIndex = 9;
            this.rfnButton.Text = "...";
            this.rfnButton.UseVisualStyleBackColor = true;
            this.rfnButton.Click += new System.EventHandler(this.rfnButton_Click);
            // 
            // railFileName
            // 
            this.railFileName.Location = new System.Drawing.Point(127, 29);
            this.railFileName.Name = "railFileName";
            this.railFileName.ReadOnly = true;
            this.railFileName.Size = new System.Drawing.Size(260, 20);
            this.railFileName.TabIndex = 8;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(5, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(72, 13);
            this.label1.TabIndex = 7;
            this.label1.Text = "RailFileName:";
            // 
            // rmfnButton
            // 
            this.rmfnButton.Location = new System.Drawing.Point(393, 78);
            this.rmfnButton.Name = "rmfnButton";
            this.rmfnButton.Size = new System.Drawing.Size(33, 23);
            this.rmfnButton.TabIndex = 15;
            this.rmfnButton.Text = "...";
            this.rmfnButton.UseVisualStyleBackColor = true;
            this.rmfnButton.Click += new System.EventHandler(this.rmfnButton_Click);
            // 
            // roundFileName
            // 
            this.roundFileName.Location = new System.Drawing.Point(127, 81);
            this.roundFileName.Name = "roundFileName";
            this.roundFileName.ReadOnly = true;
            this.roundFileName.Size = new System.Drawing.Size(260, 20);
            this.roundFileName.TabIndex = 14;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(5, 84);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(115, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = "RoundModelFileName:";
            // 
            // lefnButton
            // 
            this.lefnButton.Location = new System.Drawing.Point(393, 104);
            this.lefnButton.Name = "lefnButton";
            this.lefnButton.Size = new System.Drawing.Size(33, 23);
            this.lefnButton.TabIndex = 18;
            this.lefnButton.Text = "...";
            this.lefnButton.UseVisualStyleBackColor = true;
            this.lefnButton.Click += new System.EventHandler(this.lefnButton_Click);
            // 
            // lineEffect
            // 
            this.lineEffect.Location = new System.Drawing.Point(127, 107);
            this.lineEffect.Name = "lineEffect";
            this.lineEffect.ReadOnly = true;
            this.lineEffect.Size = new System.Drawing.Size(260, 20);
            this.lineEffect.TabIndex = 17;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(5, 110);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(102, 13);
            this.label4.TabIndex = 16;
            this.label4.Text = "LineEffectFileName:";
            // 
            // refnButton
            // 
            this.refnButton.Location = new System.Drawing.Point(393, 130);
            this.refnButton.Name = "refnButton";
            this.refnButton.Size = new System.Drawing.Size(33, 23);
            this.refnButton.TabIndex = 21;
            this.refnButton.Text = "...";
            this.refnButton.UseVisualStyleBackColor = true;
            this.refnButton.Click += new System.EventHandler(this.refnButton_Click);
            // 
            // roundEffect
            // 
            this.roundEffect.Location = new System.Drawing.Point(127, 133);
            this.roundEffect.Name = "roundEffect";
            this.roundEffect.ReadOnly = true;
            this.roundEffect.Size = new System.Drawing.Size(260, 20);
            this.roundEffect.TabIndex = 20;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(5, 136);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(114, 13);
            this.label5.TabIndex = 19;
            this.label5.Text = "RoundEffectFileName:";
            // 
            // r2efnButton
            // 
            this.r2efnButton.Location = new System.Drawing.Point(393, 156);
            this.r2efnButton.Name = "r2efnButton";
            this.r2efnButton.Size = new System.Drawing.Size(33, 23);
            this.r2efnButton.TabIndex = 24;
            this.r2efnButton.Text = "...";
            this.r2efnButton.UseVisualStyleBackColor = true;
            this.r2efnButton.Click += new System.EventHandler(this.r2efnButton_Click);
            // 
            // round2Effect
            // 
            this.round2Effect.Location = new System.Drawing.Point(127, 159);
            this.round2Effect.Name = "round2Effect";
            this.round2Effect.ReadOnly = true;
            this.round2Effect.Size = new System.Drawing.Size(260, 20);
            this.round2Effect.TabIndex = 23;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(5, 162);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(120, 13);
            this.label6.TabIndex = 22;
            this.label6.Text = "Round2EffectFileName:";
            // 
            // railFileDialog
            // 
            this.railFileDialog.FileName = "openFileDialog1";
            this.railFileDialog.Filter = "Rail files (*rl)|*.rl|All files (*.*)|*.*";
            // 
            // modelFialog
            // 
            this.modelFialog.FileName = "openFileDialog1";
            this.modelFialog.Filter = "Model files (*.mdl)|*.mdl|All files (*.*)|*.*";
            // 
            // effectDialog
            // 
            this.effectDialog.FileName = "openFileDialog2";
            this.effectDialog.Filter = "FX Effect files (*.fx)|*.fx|All files (*.*)|*.*";
            // 
            // railName
            // 
            this.railName.Location = new System.Drawing.Point(127, 3);
            this.railName.Name = "railName";
            this.railName.Size = new System.Drawing.Size(260, 20);
            this.railName.TabIndex = 26;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(5, 6);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(38, 13);
            this.label7.TabIndex = 25;
            this.label7.Text = "Name:";
            // 
            // RailSystemInfoControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.railName);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.r2efnButton);
            this.Controls.Add(this.round2Effect);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.refnButton);
            this.Controls.Add(this.roundEffect);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.lefnButton);
            this.Controls.Add(this.lineEffect);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.rmfnButton);
            this.Controls.Add(this.roundFileName);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.lmfButton);
            this.Controls.Add(this.lineFileName);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.rfnButton);
            this.Controls.Add(this.railFileName);
            this.Controls.Add(this.label1);
            this.Name = "RailSystemInfoControl";
            this.Size = new System.Drawing.Size(429, 181);
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button lmfButton;
		private System.Windows.Forms.TextBox lineFileName;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button rfnButton;
		private System.Windows.Forms.TextBox railFileName;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button rmfnButton;
		private System.Windows.Forms.TextBox roundFileName;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Button lefnButton;
		private System.Windows.Forms.TextBox lineEffect;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Button refnButton;
		private System.Windows.Forms.TextBox roundEffect;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Button r2efnButton;
		private System.Windows.Forms.TextBox round2Effect;
        private System.Windows.Forms.Label label6;
		private System.Windows.Forms.OpenFileDialog railFileDialog;
		private System.Windows.Forms.OpenFileDialog modelFialog;
		private System.Windows.Forms.OpenFileDialog effectDialog;
        private System.Windows.Forms.TextBox railName;
        private System.Windows.Forms.Label label7;
	}
}
