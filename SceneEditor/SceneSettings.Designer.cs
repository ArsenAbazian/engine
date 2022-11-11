namespace sceneEditor {
	partial class SceneSettings {
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
			this.selDirButton = new System.Windows.Forms.Button();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.modelDirLabel = new System.Windows.Forms.Label();
			this.modelText = new System.Windows.Forms.TextBox();
			this.modelDirButton = new System.Windows.Forms.Button();
			this.landDirButton = new System.Windows.Forms.Button();
			this.landText = new System.Windows.Forms.TextBox();
			this.landDirLabel = new System.Windows.Forms.Label();
			this.textureDirButton = new System.Windows.Forms.Button();
			this.textureText = new System.Windows.Forms.TextBox();
			this.textureDirLabel = new System.Windows.Forms.Label();
			this.effectDirButton = new System.Windows.Forms.Button();
			this.effectText = new System.Windows.Forms.TextBox();
			this.effectDirLabel = new System.Windows.Forms.Label();
			this.particleDirButton = new System.Windows.Forms.Button();
			this.particleText = new System.Windows.Forms.TextBox();
			this.particleDirLabel = new System.Windows.Forms.Label();
			this.railDirButton = new System.Windows.Forms.Button();
			this.railText = new System.Windows.Forms.TextBox();
			this.railDirLabel = new System.Windows.Forms.Label();
			this.fontDirButton = new System.Windows.Forms.Button();
			this.fontText = new System.Windows.Forms.TextBox();
			this.fontDirLabel = new System.Windows.Forms.Label();
			this.soundDirButton = new System.Windows.Forms.Button();
			this.soundText = new System.Windows.Forms.TextBox();
			this.sound = new System.Windows.Forms.Label();
			this.folderBrowser = new System.Windows.Forms.FolderBrowserDialog();
			this.dataText = new System.Windows.Forms.TextBox();
			this.defButton = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.SuspendLayout();
			// 
			// selDirButton
			// 
			this.selDirButton.Location = new System.Drawing.Point(247, 17);
			this.selDirButton.Name = "selDirButton";
			this.selDirButton.Size = new System.Drawing.Size(36, 23);
			this.selDirButton.TabIndex = 0;
			this.selDirButton.Text = "...";
			this.selDirButton.UseVisualStyleBackColor = true;
			this.selDirButton.Click += new System.EventHandler(this.selDirButton_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.dataText);
			this.groupBox1.Controls.Add(this.selDirButton);
			this.groupBox1.Location = new System.Drawing.Point(3, 3);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(289, 50);
			this.groupBox1.TabIndex = 2;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = " Scene Data Root Directory ";
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.defButton);
			this.groupBox2.Controls.Add(this.soundDirButton);
			this.groupBox2.Controls.Add(this.soundText);
			this.groupBox2.Controls.Add(this.sound);
			this.groupBox2.Controls.Add(this.fontDirButton);
			this.groupBox2.Controls.Add(this.fontText);
			this.groupBox2.Controls.Add(this.fontDirLabel);
			this.groupBox2.Controls.Add(this.railDirButton);
			this.groupBox2.Controls.Add(this.railText);
			this.groupBox2.Controls.Add(this.railDirLabel);
			this.groupBox2.Controls.Add(this.particleDirButton);
			this.groupBox2.Controls.Add(this.particleText);
			this.groupBox2.Controls.Add(this.particleDirLabel);
			this.groupBox2.Controls.Add(this.effectDirButton);
			this.groupBox2.Controls.Add(this.effectText);
			this.groupBox2.Controls.Add(this.effectDirLabel);
			this.groupBox2.Controls.Add(this.textureDirButton);
			this.groupBox2.Controls.Add(this.textureText);
			this.groupBox2.Controls.Add(this.textureDirLabel);
			this.groupBox2.Controls.Add(this.landDirButton);
			this.groupBox2.Controls.Add(this.landText);
			this.groupBox2.Controls.Add(this.landDirLabel);
			this.groupBox2.Controls.Add(this.modelDirButton);
			this.groupBox2.Controls.Add(this.modelText);
			this.groupBox2.Controls.Add(this.modelDirLabel);
			this.groupBox2.Location = new System.Drawing.Point(3, 59);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(289, 254);
			this.groupBox2.TabIndex = 3;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = " Data Directories ";
			// 
			// modelDirLabel
			// 
			this.modelDirLabel.AutoSize = true;
			this.modelDirLabel.Location = new System.Drawing.Point(6, 70);
			this.modelDirLabel.Name = "modelDirLabel";
			this.modelDirLabel.Size = new System.Drawing.Size(84, 13);
			this.modelDirLabel.TabIndex = 0;
			this.modelDirLabel.Text = "Model Directory:";
			// 
			// modelText
			// 
			this.modelText.Location = new System.Drawing.Point(96, 67);
			this.modelText.Name = "modelText";
			this.modelText.Size = new System.Drawing.Size(140, 20);
			this.modelText.TabIndex = 1;
			// 
			// modelDirButton
			// 
			this.modelDirButton.Location = new System.Drawing.Point(247, 65);
			this.modelDirButton.Name = "modelDirButton";
			this.modelDirButton.Size = new System.Drawing.Size(36, 23);
			this.modelDirButton.TabIndex = 2;
			this.modelDirButton.Text = "...";
			this.modelDirButton.UseVisualStyleBackColor = true;
			this.modelDirButton.Click += new System.EventHandler(this.modelDirButton_Click);
			// 
			// landDirButton
			// 
			this.landDirButton.Location = new System.Drawing.Point(247, 91);
			this.landDirButton.Name = "landDirButton";
			this.landDirButton.Size = new System.Drawing.Size(36, 23);
			this.landDirButton.TabIndex = 5;
			this.landDirButton.Text = "...";
			this.landDirButton.UseVisualStyleBackColor = true;
			this.landDirButton.Click += new System.EventHandler(this.landDirButton_Click);
			// 
			// landText
			// 
			this.landText.Location = new System.Drawing.Point(96, 93);
			this.landText.Name = "landText";
			this.landText.Size = new System.Drawing.Size(140, 20);
			this.landText.TabIndex = 4;
			// 
			// landDirLabel
			// 
			this.landDirLabel.AutoSize = true;
			this.landDirLabel.Location = new System.Drawing.Point(6, 96);
			this.landDirLabel.Name = "landDirLabel";
			this.landDirLabel.Size = new System.Drawing.Size(79, 13);
			this.landDirLabel.TabIndex = 3;
			this.landDirLabel.Text = "Land Directory:";
			// 
			// textureDirButton
			// 
			this.textureDirButton.Location = new System.Drawing.Point(247, 11);
			this.textureDirButton.Name = "textureDirButton";
			this.textureDirButton.Size = new System.Drawing.Size(36, 23);
			this.textureDirButton.TabIndex = 8;
			this.textureDirButton.Text = "...";
			this.textureDirButton.UseVisualStyleBackColor = true;
			this.textureDirButton.Click += new System.EventHandler(this.textureDirButton_Click);
			// 
			// textureText
			// 
			this.textureText.Location = new System.Drawing.Point(96, 13);
			this.textureText.Name = "textureText";
			this.textureText.Size = new System.Drawing.Size(140, 20);
			this.textureText.TabIndex = 7;
			// 
			// textureDirLabel
			// 
			this.textureDirLabel.AutoSize = true;
			this.textureDirLabel.Location = new System.Drawing.Point(6, 16);
			this.textureDirLabel.Name = "textureDirLabel";
			this.textureDirLabel.Size = new System.Drawing.Size(91, 13);
			this.textureDirLabel.TabIndex = 6;
			this.textureDirLabel.Text = "Texture Directory:";
			// 
			// effectDirButton
			// 
			this.effectDirButton.Location = new System.Drawing.Point(247, 37);
			this.effectDirButton.Name = "effectDirButton";
			this.effectDirButton.Size = new System.Drawing.Size(36, 23);
			this.effectDirButton.TabIndex = 11;
			this.effectDirButton.Text = "...";
			this.effectDirButton.UseVisualStyleBackColor = true;
			this.effectDirButton.Click += new System.EventHandler(this.effectDirButton_Click);
			// 
			// effectText
			// 
			this.effectText.Location = new System.Drawing.Point(96, 39);
			this.effectText.Name = "effectText";
			this.effectText.Size = new System.Drawing.Size(140, 20);
			this.effectText.TabIndex = 10;
			// 
			// effectDirLabel
			// 
			this.effectDirLabel.AutoSize = true;
			this.effectDirLabel.Location = new System.Drawing.Point(6, 42);
			this.effectDirLabel.Name = "effectDirLabel";
			this.effectDirLabel.Size = new System.Drawing.Size(83, 13);
			this.effectDirLabel.TabIndex = 9;
			this.effectDirLabel.Text = "Effect Directory:";
			// 
			// particleDirButton
			// 
			this.particleDirButton.Location = new System.Drawing.Point(247, 121);
			this.particleDirButton.Name = "particleDirButton";
			this.particleDirButton.Size = new System.Drawing.Size(36, 23);
			this.particleDirButton.TabIndex = 14;
			this.particleDirButton.Text = "...";
			this.particleDirButton.UseVisualStyleBackColor = true;
			this.particleDirButton.Click += new System.EventHandler(this.particleDirButton_Click);
			// 
			// particleText
			// 
			this.particleText.Location = new System.Drawing.Point(96, 123);
			this.particleText.Name = "particleText";
			this.particleText.Size = new System.Drawing.Size(140, 20);
			this.particleText.TabIndex = 13;
			// 
			// particleDirLabel
			// 
			this.particleDirLabel.AutoSize = true;
			this.particleDirLabel.Location = new System.Drawing.Point(6, 126);
			this.particleDirLabel.Name = "particleDirLabel";
			this.particleDirLabel.Size = new System.Drawing.Size(90, 13);
			this.particleDirLabel.TabIndex = 12;
			this.particleDirLabel.Text = "Particle Directory:";
			// 
			// railDirButton
			// 
			this.railDirButton.Location = new System.Drawing.Point(247, 147);
			this.railDirButton.Name = "railDirButton";
			this.railDirButton.Size = new System.Drawing.Size(36, 23);
			this.railDirButton.TabIndex = 17;
			this.railDirButton.Text = "...";
			this.railDirButton.UseVisualStyleBackColor = true;
			this.railDirButton.Click += new System.EventHandler(this.railDirButton_Click);
			// 
			// railText
			// 
			this.railText.Location = new System.Drawing.Point(96, 149);
			this.railText.Name = "railText";
			this.railText.Size = new System.Drawing.Size(140, 20);
			this.railText.TabIndex = 16;
			// 
			// railDirLabel
			// 
			this.railDirLabel.AutoSize = true;
			this.railDirLabel.Location = new System.Drawing.Point(6, 152);
			this.railDirLabel.Name = "railDirLabel";
			this.railDirLabel.Size = new System.Drawing.Size(78, 13);
			this.railDirLabel.TabIndex = 15;
			this.railDirLabel.Text = "Rails Directory:";
			// 
			// fontDirButton
			// 
			this.fontDirButton.Location = new System.Drawing.Point(247, 173);
			this.fontDirButton.Name = "fontDirButton";
			this.fontDirButton.Size = new System.Drawing.Size(36, 23);
			this.fontDirButton.TabIndex = 20;
			this.fontDirButton.Text = "...";
			this.fontDirButton.UseVisualStyleBackColor = true;
			this.fontDirButton.Click += new System.EventHandler(this.fontDirButton_Click);
			// 
			// fontText
			// 
			this.fontText.Location = new System.Drawing.Point(96, 175);
			this.fontText.Name = "fontText";
			this.fontText.Size = new System.Drawing.Size(140, 20);
			this.fontText.TabIndex = 19;
			// 
			// fontDirLabel
			// 
			this.fontDirLabel.AutoSize = true;
			this.fontDirLabel.Location = new System.Drawing.Point(6, 178);
			this.fontDirLabel.Name = "fontDirLabel";
			this.fontDirLabel.Size = new System.Drawing.Size(76, 13);
			this.fontDirLabel.TabIndex = 18;
			this.fontDirLabel.Text = "Font Directory:";
			// 
			// soundDirButton
			// 
			this.soundDirButton.Location = new System.Drawing.Point(247, 199);
			this.soundDirButton.Name = "soundDirButton";
			this.soundDirButton.Size = new System.Drawing.Size(36, 23);
			this.soundDirButton.TabIndex = 23;
			this.soundDirButton.Text = "...";
			this.soundDirButton.UseVisualStyleBackColor = true;
			this.soundDirButton.Click += new System.EventHandler(this.soundDirButton_Click);
			// 
			// soundText
			// 
			this.soundText.Location = new System.Drawing.Point(96, 201);
			this.soundText.Name = "soundText";
			this.soundText.Size = new System.Drawing.Size(140, 20);
			this.soundText.TabIndex = 22;
			// 
			// sound
			// 
			this.sound.AutoSize = true;
			this.sound.Location = new System.Drawing.Point(6, 204);
			this.sound.Name = "sound";
			this.sound.Size = new System.Drawing.Size(86, 13);
			this.sound.TabIndex = 21;
			this.sound.Text = "Sound Directory:";
			// 
			// dataText
			// 
			this.dataText.Location = new System.Drawing.Point(9, 19);
			this.dataText.Name = "dataText";
			this.dataText.Size = new System.Drawing.Size(227, 20);
			this.dataText.TabIndex = 1;
			// 
			// defButton
			// 
			this.defButton.Location = new System.Drawing.Point(106, 225);
			this.defButton.Name = "defButton";
			this.defButton.Size = new System.Drawing.Size(75, 23);
			this.defButton.TabIndex = 24;
			this.defButton.Text = "By Default";
			this.defButton.UseVisualStyleBackColor = true;
			this.defButton.Click += new System.EventHandler(this.defButton_Click);
			// 
			// SceneSettings
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.groupBox1);
			this.Name = "SceneSettings";
			this.Size = new System.Drawing.Size(295, 316);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button selDirButton;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Button textureDirButton;
		private System.Windows.Forms.TextBox textureText;
		private System.Windows.Forms.Label textureDirLabel;
		private System.Windows.Forms.Button landDirButton;
		private System.Windows.Forms.TextBox landText;
		private System.Windows.Forms.Label landDirLabel;
		private System.Windows.Forms.Button modelDirButton;
		private System.Windows.Forms.TextBox modelText;
		private System.Windows.Forms.Label modelDirLabel;
		private System.Windows.Forms.Button soundDirButton;
		private System.Windows.Forms.TextBox soundText;
		private System.Windows.Forms.Label sound;
		private System.Windows.Forms.Button fontDirButton;
		private System.Windows.Forms.TextBox fontText;
		private System.Windows.Forms.Label fontDirLabel;
		private System.Windows.Forms.Button railDirButton;
		private System.Windows.Forms.TextBox railText;
		private System.Windows.Forms.Label railDirLabel;
		private System.Windows.Forms.Button particleDirButton;
		private System.Windows.Forms.TextBox particleText;
		private System.Windows.Forms.Label particleDirLabel;
		private System.Windows.Forms.Button effectDirButton;
		private System.Windows.Forms.TextBox effectText;
		private System.Windows.Forms.Label effectDirLabel;
		private System.Windows.Forms.FolderBrowserDialog folderBrowser;
		private System.Windows.Forms.TextBox dataText;
		private System.Windows.Forms.Button defButton;
	}
}
