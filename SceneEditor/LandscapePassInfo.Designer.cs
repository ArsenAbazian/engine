using sceneEditor.Controls;
namespace sceneEditor {
	partial class LandscapePassInfo {
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
            this.passEnabled = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.mapFileName = new ValueTextBox();
            this.mfnButton = new System.Windows.Forms.Button();
            this.msfButton = new System.Windows.Forms.Button();
            this.maskFileName = new ValueTextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.mapUSize = new ValueTextBox();
            this.mapVSize = new ValueTextBox();
            this.vmpfViewButton = new System.Windows.Forms.Button();
            this.msfViewButton = new System.Windows.Forms.Button();
            this.imageFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.detButton = new System.Windows.Forms.Button();
            this.dfnButton = new System.Windows.Forms.Button();
            this.detFileName = new ValueTextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.detVSize = new ValueTextBox();
            this.detUSize = new ValueTextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.maxDetAlpha = new ValueTextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.minDetAlphaDist = new ValueTextBox();
            this.maxDetAlphaDist = new ValueTextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.animated = new System.Windows.Forms.CheckBox();
            this.pnLabel = new System.Windows.Forms.Label();
            this.passName = new ValueTextBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // passEnabled
            // 
            this.passEnabled.AutoSize = true;
            this.passEnabled.Location = new System.Drawing.Point(6, 3);
            this.passEnabled.Name = "passEnabled";
            this.passEnabled.Size = new System.Drawing.Size(65, 17);
            this.passEnabled.TabIndex = 0;
            this.passEnabled.Text = "Enabled";
            this.passEnabled.UseVisualStyleBackColor = true;
            this.passEnabled.CheckedChanged += new System.EventHandler(this.passEnabled_CheckedChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 56);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(75, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "MapFileName:";
            // 
            // mapFileName
            // 
            this.mapFileName.Location = new System.Drawing.Point(84, 53);
            this.mapFileName.Name = "mapFileName";
            this.mapFileName.ReadOnly = true;
            this.mapFileName.Size = new System.Drawing.Size(260, 20);
            this.mapFileName.TabIndex = 2;
            // 
            // mfnButton
            // 
            this.mfnButton.Location = new System.Drawing.Point(350, 50);
            this.mfnButton.Name = "mfnButton";
            this.mfnButton.Size = new System.Drawing.Size(33, 23);
            this.mfnButton.TabIndex = 3;
            this.mfnButton.Text = "...";
            this.mfnButton.UseVisualStyleBackColor = true;
            this.mfnButton.Click += new System.EventHandler(this.mfnButton_Click);
            // 
            // msfButton
            // 
            this.msfButton.Location = new System.Drawing.Point(350, 76);
            this.msfButton.Name = "msfButton";
            this.msfButton.Size = new System.Drawing.Size(33, 23);
            this.msfButton.TabIndex = 6;
            this.msfButton.Text = "...";
            this.msfButton.UseVisualStyleBackColor = true;
            this.msfButton.Click += new System.EventHandler(this.msfButton_Click);
            // 
            // maskFileName
            // 
            this.maskFileName.Location = new System.Drawing.Point(84, 79);
            this.maskFileName.Name = "maskFileName";
            this.maskFileName.ReadOnly = true;
            this.maskFileName.Size = new System.Drawing.Size(260, 20);
            this.maskFileName.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 82);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(80, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "MaskFileName:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 19);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(66, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "MapUVSize:";
            // 
            // mapUSize
            // 
            this.mapUSize.Location = new System.Drawing.Point(90, 16);
            this.mapUSize.Name = "mapUSize";
            this.mapUSize.Size = new System.Drawing.Size(95, 20);
            this.mapUSize.TabIndex = 8;
            this.mapUSize.Text = "1.0";
            this.mapUSize.ValueChanged += new System.EventHandler(this.mapUSize_ValueChanged);
            // 
            // mapVSize
            // 
            this.mapVSize.Location = new System.Drawing.Point(90, 42);
            this.mapVSize.Name = "mapVSize";
            this.mapVSize.Size = new System.Drawing.Size(95, 20);
            this.mapVSize.TabIndex = 10;
            this.mapVSize.Text = "1.0";
            this.mapVSize.ValueChanged += new System.EventHandler(this.mapVSize_ValueChanged);
            // 
            // vmpfViewButton
            // 
            this.vmpfViewButton.Location = new System.Drawing.Point(389, 50);
            this.vmpfViewButton.Name = "vmpfViewButton";
            this.vmpfViewButton.Size = new System.Drawing.Size(75, 23);
            this.vmpfViewButton.TabIndex = 11;
            this.vmpfViewButton.Text = "View Image";
            this.vmpfViewButton.UseVisualStyleBackColor = true;
            this.vmpfViewButton.Click += new System.EventHandler(this.vmpfViewButton_Click);
            // 
            // msfViewButton
            // 
            this.msfViewButton.Location = new System.Drawing.Point(389, 76);
            this.msfViewButton.Name = "msfViewButton";
            this.msfViewButton.Size = new System.Drawing.Size(75, 23);
            this.msfViewButton.TabIndex = 12;
            this.msfViewButton.Text = "View Image";
            this.msfViewButton.UseVisualStyleBackColor = true;
            this.msfViewButton.Click += new System.EventHandler(this.msfViewButton_Click);
            // 
            // imageFileDialog
            // 
            this.imageFileDialog.FileName = "Image";
            this.imageFileDialog.Filter = "Png files (*.png)|*.png|Bmp files (*.bmp)|*.bmp|Jpg files (*.jpg)|*.jpg|All files" +
                " (*.*)|*.*";
            // 
            // detButton
            // 
            this.detButton.Location = new System.Drawing.Point(389, 102);
            this.detButton.Name = "detButton";
            this.detButton.Size = new System.Drawing.Size(75, 23);
            this.detButton.TabIndex = 16;
            this.detButton.Text = "View Image";
            this.detButton.UseVisualStyleBackColor = true;
            this.detButton.Click += new System.EventHandler(this.detButton_Click);
            // 
            // dfnButton
            // 
            this.dfnButton.Location = new System.Drawing.Point(350, 102);
            this.dfnButton.Name = "dfnButton";
            this.dfnButton.Size = new System.Drawing.Size(33, 23);
            this.dfnButton.TabIndex = 15;
            this.dfnButton.Text = "...";
            this.dfnButton.UseVisualStyleBackColor = true;
            this.dfnButton.Click += new System.EventHandler(this.dfnButton_Click);
            // 
            // detFileName
            // 
            this.detFileName.Location = new System.Drawing.Point(84, 105);
            this.detFileName.Name = "detFileName";
            this.detFileName.ReadOnly = true;
            this.detFileName.Size = new System.Drawing.Size(260, 20);
            this.detFileName.TabIndex = 14;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 108);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(81, 13);
            this.label5.TabIndex = 13;
            this.label5.Text = "DetailFileName:";
            // 
            // detVSize
            // 
            this.detVSize.Location = new System.Drawing.Point(90, 94);
            this.detVSize.Name = "detVSize";
            this.detVSize.Size = new System.Drawing.Size(95, 20);
            this.detVSize.TabIndex = 20;
            this.detVSize.Text = "1.0";
            this.detVSize.ValueChanged += new System.EventHandler(this.detVSize_ValueChanged);
            // 
            // detUSize
            // 
            this.detUSize.Location = new System.Drawing.Point(90, 68);
            this.detUSize.Name = "detUSize";
            this.detUSize.Size = new System.Drawing.Size(95, 20);
            this.detUSize.TabIndex = 18;
            this.detUSize.Text = "1.0";
            this.detUSize.ValueChanged += new System.EventHandler(this.detUSize_ValueChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(5, 72);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(65, 13);
            this.label7.TabIndex = 17;
            this.label7.Text = "DetailUSize:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.detVSize);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.mapUSize);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.detUSize);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.mapVSize);
            this.groupBox1.Location = new System.Drawing.Point(6, 131);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(191, 121);
            this.groupBox1.TabIndex = 21;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = " UV ";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 102);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(64, 13);
            this.label6.TabIndex = 19;
            this.label6.Text = "DetailVSize:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 46);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(58, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "MapVSize:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.maxDetAlpha);
            this.groupBox2.Controls.Add(this.label10);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.minDetAlphaDist);
            this.groupBox2.Controls.Add(this.maxDetAlphaDist);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Location = new System.Drawing.Point(203, 131);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(261, 121);
            this.groupBox2.TabIndex = 22;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = " Detail Params ";
            // 
            // maxDetAlpha
            // 
            this.maxDetAlpha.Location = new System.Drawing.Point(158, 70);
            this.maxDetAlpha.Name = "maxDetAlpha";
            this.maxDetAlpha.Size = new System.Drawing.Size(95, 20);
            this.maxDetAlpha.TabIndex = 24;
            this.maxDetAlpha.Text = "0.6";
            this.maxDetAlpha.ValueChanged += new System.EventHandler(this.maxDetAlpha_ValueChanged);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(6, 73);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(84, 13);
            this.label10.TabIndex = 23;
            this.label10.Text = "MaxDetailAlpha:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 19);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(123, 13);
            this.label8.TabIndex = 19;
            this.label8.Text = "MinDetailAlphaDistance:";
            // 
            // minDetAlphaDist
            // 
            this.minDetAlphaDist.Location = new System.Drawing.Point(158, 16);
            this.minDetAlphaDist.Name = "minDetAlphaDist";
            this.minDetAlphaDist.Size = new System.Drawing.Size(95, 20);
            this.minDetAlphaDist.TabIndex = 20;
            this.minDetAlphaDist.Text = "10.0";
            this.minDetAlphaDist.ValueChanged += new System.EventHandler(this.minDetAlphaDist_ValueChanged);
            // 
            // maxDetAlphaDist
            // 
            this.maxDetAlphaDist.Location = new System.Drawing.Point(158, 44);
            this.maxDetAlphaDist.Name = "maxDetAlphaDist";
            this.maxDetAlphaDist.Size = new System.Drawing.Size(95, 20);
            this.maxDetAlphaDist.TabIndex = 22;
            this.maxDetAlphaDist.Text = "1.0";
            this.maxDetAlphaDist.ValueChanged += new System.EventHandler(this.maxDetAlphaDist_ValueChanged);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(6, 47);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(126, 13);
            this.label9.TabIndex = 21;
            this.label9.Text = "MaxDetailAlphaDistance:";
            // 
            // animated
            // 
            this.animated.AutoSize = true;
            this.animated.Location = new System.Drawing.Point(77, 3);
            this.animated.Name = "animated";
            this.animated.Size = new System.Drawing.Size(70, 17);
            this.animated.TabIndex = 23;
            this.animated.Text = "Animated";
            this.animated.UseVisualStyleBackColor = true;
            this.animated.CheckedChanged += new System.EventHandler(this.animated_CheckedChanged);
            // 
            // pnLabel
            // 
            this.pnLabel.AutoSize = true;
            this.pnLabel.Location = new System.Drawing.Point(3, 30);
            this.pnLabel.Name = "pnLabel";
            this.pnLabel.Size = new System.Drawing.Size(61, 13);
            this.pnLabel.TabIndex = 24;
            this.pnLabel.Text = "PassName:";
            // 
            // passName
            // 
            this.passName.Location = new System.Drawing.Point(84, 27);
            this.passName.Name = "passName";
            this.passName.Size = new System.Drawing.Size(260, 20);
            this.passName.TabIndex = 25;
            this.passName.TextChanged += new System.EventHandler(this.passName_TextChanged);
            // 
            // LandscapePassInfo
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.passName);
            this.Controls.Add(this.pnLabel);
            this.Controls.Add(this.animated);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.detButton);
            this.Controls.Add(this.dfnButton);
            this.Controls.Add(this.detFileName);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.msfViewButton);
            this.Controls.Add(this.vmpfViewButton);
            this.Controls.Add(this.msfButton);
            this.Controls.Add(this.maskFileName);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.mfnButton);
            this.Controls.Add(this.mapFileName);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.passEnabled);
            this.Name = "LandscapePassInfo";
            this.Size = new System.Drawing.Size(468, 254);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.CheckBox passEnabled;
		private System.Windows.Forms.Label label1;
		private ValueTextBox mapFileName;
		private System.Windows.Forms.Button mfnButton;
		private System.Windows.Forms.Button msfButton;
		private ValueTextBox maskFileName;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private ValueTextBox mapUSize;
		private ValueTextBox mapVSize;
		private System.Windows.Forms.Button vmpfViewButton;
		private System.Windows.Forms.Button msfViewButton;
		private System.Windows.Forms.OpenFileDialog imageFileDialog;
		private System.Windows.Forms.Button detButton;
		private System.Windows.Forms.Button dfnButton;
		private ValueTextBox detFileName;
		private System.Windows.Forms.Label label5;
		private ValueTextBox detVSize;
		private ValueTextBox detUSize;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox groupBox2;
		private ValueTextBox maxDetAlpha;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.Label label8;
		private ValueTextBox minDetAlphaDist;
		private ValueTextBox maxDetAlphaDist;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox animated;
        private System.Windows.Forms.Label pnLabel;
        private ValueTextBox passName;
	}
}
