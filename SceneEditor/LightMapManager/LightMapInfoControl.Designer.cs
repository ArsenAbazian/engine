namespace sceneEditor.LightMapManager {
    partial class LightMapInfoControl {
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
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.enabledCheck = new System.Windows.Forms.CheckBox();
            this.fileName = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.fnButton = new System.Windows.Forms.Button();
            this.tpnCombo = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.lmWidth = new System.Windows.Forms.ComboBox();
            this.lmHeight = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.tcIndex = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.atlas = new System.Windows.Forms.CheckBox();
            this.u0 = new System.Windows.Forms.TextBox();
            this.v0 = new System.Windows.Forms.TextBox();
            this.uSize = new System.Windows.Forms.TextBox();
            this.vSize = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.fnDialog = new System.Windows.Forms.SaveFileDialog();
            this.autoFileName = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(113, 26);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(134, 20);
            this.textBox1.TabIndex = 0;
            // 
            // enabledCheck
            // 
            this.enabledCheck.AutoSize = true;
            this.enabledCheck.Location = new System.Drawing.Point(113, 3);
            this.enabledCheck.Name = "enabledCheck";
            this.enabledCheck.Size = new System.Drawing.Size(65, 17);
            this.enabledCheck.TabIndex = 1;
            this.enabledCheck.Text = "Enabled";
            this.enabledCheck.UseVisualStyleBackColor = true;
            this.enabledCheck.CheckedChanged += new System.EventHandler(this.enabledCheck_CheckedChanged);
            // 
            // fileName
            // 
            this.fileName.AutoSize = true;
            this.fileName.Location = new System.Drawing.Point(3, 29);
            this.fileName.Name = "fileName";
            this.fileName.Size = new System.Drawing.Size(54, 13);
            this.fileName.TabIndex = 2;
            this.fileName.Text = "FileName:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 58);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(104, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "TextureParamName:";
            // 
            // fnButton
            // 
            this.fnButton.Location = new System.Drawing.Point(253, 24);
            this.fnButton.Name = "fnButton";
            this.fnButton.Size = new System.Drawing.Size(31, 23);
            this.fnButton.TabIndex = 5;
            this.fnButton.Text = "...";
            this.fnButton.UseVisualStyleBackColor = true;
            this.fnButton.Click += new System.EventHandler(this.fnButton_Click);
            // 
            // tpnCombo
            // 
            this.tpnCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.tpnCombo.FormattingEnabled = true;
            this.tpnCombo.Location = new System.Drawing.Point(113, 55);
            this.tpnCombo.Name = "tpnCombo";
            this.tpnCombo.Size = new System.Drawing.Size(134, 21);
            this.tpnCombo.TabIndex = 6;
            this.tpnCombo.SelectedIndexChanged += new System.EventHandler(this.tpnCombo_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 84);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(82, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "LightMapWidth:";
            // 
            // lmWidth
            // 
            this.lmWidth.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.lmWidth.FormattingEnabled = true;
            this.lmWidth.Items.AddRange(new object[] {
            "1",
            "2",
            "4",
            "8",
            "16",
            "32",
            "64",
            "128",
            "256",
            "512",
            "1024",
            "2048",
            "4096"});
            this.lmWidth.Location = new System.Drawing.Point(113, 81);
            this.lmWidth.Name = "lmWidth";
            this.lmWidth.Size = new System.Drawing.Size(134, 21);
            this.lmWidth.TabIndex = 8;
            this.lmWidth.SelectedIndexChanged += new System.EventHandler(this.lmWidth_SelectedIndexChanged);
            // 
            // lmHeight
            // 
            this.lmHeight.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.lmHeight.FormattingEnabled = true;
            this.lmHeight.Items.AddRange(new object[] {
            "1",
            "2",
            "4",
            "8",
            "16",
            "32",
            "64",
            "128",
            "256",
            "512",
            "1024",
            "2048",
            "4096"});
            this.lmHeight.Location = new System.Drawing.Point(113, 108);
            this.lmHeight.Name = "lmHeight";
            this.lmHeight.Size = new System.Drawing.Size(134, 21);
            this.lmHeight.TabIndex = 10;
            this.lmHeight.SelectedIndexChanged += new System.EventHandler(this.lmHeight_SelectedIndexChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 108);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(85, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "LightMapHeight:";
            // 
            // tcIndex
            // 
            this.tcIndex.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.tcIndex.FormattingEnabled = true;
            this.tcIndex.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8"});
            this.tcIndex.Location = new System.Drawing.Point(113, 135);
            this.tcIndex.Name = "tcIndex";
            this.tcIndex.Size = new System.Drawing.Size(134, 21);
            this.tcIndex.TabIndex = 12;
            this.tcIndex.SelectedIndexChanged += new System.EventHandler(this.tcIndex_SelectedIndexChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 138);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(76, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "TexCoorIndex:";
            // 
            // atlas
            // 
            this.atlas.AutoSize = true;
            this.atlas.Location = new System.Drawing.Point(113, 162);
            this.atlas.Name = "atlas";
            this.atlas.Size = new System.Drawing.Size(99, 17);
            this.atlas.TabIndex = 13;
            this.atlas.Text = "Is Atlas Texture";
            this.atlas.UseVisualStyleBackColor = true;
            this.atlas.CheckedChanged += new System.EventHandler(this.atlas_CheckedChanged);
            // 
            // u0
            // 
            this.u0.Location = new System.Drawing.Point(113, 185);
            this.u0.Name = "u0";
            this.u0.Size = new System.Drawing.Size(134, 20);
            this.u0.TabIndex = 14;
            this.u0.TextChanged += new System.EventHandler(this.u0_TextChanged);
            // 
            // v0
            // 
            this.v0.Location = new System.Drawing.Point(113, 211);
            this.v0.Name = "v0";
            this.v0.Size = new System.Drawing.Size(134, 20);
            this.v0.TabIndex = 15;
            this.v0.TextChanged += new System.EventHandler(this.v0_TextChanged);
            // 
            // uSize
            // 
            this.uSize.Location = new System.Drawing.Point(113, 237);
            this.uSize.Name = "uSize";
            this.uSize.Size = new System.Drawing.Size(134, 20);
            this.uSize.TabIndex = 16;
            this.uSize.TextChanged += new System.EventHandler(this.uSize_TextChanged);
            // 
            // vSize
            // 
            this.vSize.Location = new System.Drawing.Point(113, 263);
            this.vSize.Name = "vSize";
            this.vSize.Size = new System.Drawing.Size(134, 20);
            this.vSize.TabIndex = 17;
            this.vSize.TextChanged += new System.EventHandler(this.vSize_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 188);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(24, 13);
            this.label5.TabIndex = 18;
            this.label5.Text = "U0:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(3, 214);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(23, 13);
            this.label6.TabIndex = 19;
            this.label6.Text = "V0:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(3, 240);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(38, 13);
            this.label7.TabIndex = 20;
            this.label7.Text = "USize:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(3, 266);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(37, 13);
            this.label8.TabIndex = 21;
            this.label8.Text = "VSize:";
            // 
            // autoFileName
            // 
            this.autoFileName.Location = new System.Drawing.Point(6, 291);
            this.autoFileName.Name = "autoFileName";
            this.autoFileName.Size = new System.Drawing.Size(110, 23);
            this.autoFileName.TabIndex = 22;
            this.autoFileName.Text = "Generate FileName";
            this.autoFileName.UseVisualStyleBackColor = true;
            this.autoFileName.Click += new System.EventHandler(this.autoFileName_Click);
            // 
            // LightMapInfoControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.autoFileName);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.vSize);
            this.Controls.Add(this.uSize);
            this.Controls.Add(this.v0);
            this.Controls.Add(this.u0);
            this.Controls.Add(this.atlas);
            this.Controls.Add(this.tcIndex);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.lmHeight);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.lmWidth);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tpnCombo);
            this.Controls.Add(this.fnButton);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.fileName);
            this.Controls.Add(this.enabledCheck);
            this.Controls.Add(this.textBox1);
            this.Name = "LightMapInfoControl";
            this.Size = new System.Drawing.Size(286, 317);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.CheckBox enabledCheck;
        private System.Windows.Forms.Label fileName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button fnButton;
        private System.Windows.Forms.ComboBox tpnCombo;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox lmWidth;
        private System.Windows.Forms.ComboBox lmHeight;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox tcIndex;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox atlas;
        private System.Windows.Forms.TextBox u0;
        private System.Windows.Forms.TextBox v0;
        private System.Windows.Forms.TextBox uSize;
        private System.Windows.Forms.TextBox vSize;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.SaveFileDialog fnDialog;
        private System.Windows.Forms.Button autoFileName;
    }
}
