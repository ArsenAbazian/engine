using sceneEditor.Controls;
namespace sceneEditor {
    partial class LandscapeProps {
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
            this.label1 = new System.Windows.Forms.Label();
            this.hmText = new System.Windows.Forms.TextBox();
            this.hmButton = new System.Windows.Forms.Button();
            this.lmButton = new System.Windows.Forms.Button();
            this.lmText = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.pps = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.heightScale = new ValueTextBox();
            this.sideScale = new ValueTextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.lightMapHeight = new ValueTextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.lightMapWidth = new ValueTextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.pngDialog = new System.Windows.Forms.OpenFileDialog();
            this.button1 = new System.Windows.Forms.Button();
            this.effect = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.effectDialog = new System.Windows.Forms.OpenFileDialog();
            this.name = new System.Windows.Forms.TextBox();
            this.landNameLabel = new System.Windows.Forms.Label();
            this.pixelError = new ValueTextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(106, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "HeightMapFileName:";
            // 
            // hmText
            // 
            this.hmText.Location = new System.Drawing.Point(115, 29);
            this.hmText.Name = "hmText";
            this.hmText.ReadOnly = true;
            this.hmText.Size = new System.Drawing.Size(311, 20);
            this.hmText.TabIndex = 1;
            // 
            // hmButton
            // 
            this.hmButton.Location = new System.Drawing.Point(432, 27);
            this.hmButton.Name = "hmButton";
            this.hmButton.Size = new System.Drawing.Size(32, 23);
            this.hmButton.TabIndex = 2;
            this.hmButton.Text = "...";
            this.hmButton.UseVisualStyleBackColor = true;
            this.hmButton.Click += new System.EventHandler(this.hmButton_Click);
            // 
            // lmButton
            // 
            this.lmButton.Location = new System.Drawing.Point(432, 53);
            this.lmButton.Name = "lmButton";
            this.lmButton.Size = new System.Drawing.Size(32, 23);
            this.lmButton.TabIndex = 5;
            this.lmButton.Text = "...";
            this.lmButton.UseVisualStyleBackColor = true;
            this.lmButton.Click += new System.EventHandler(this.lmButton_Click);
            // 
            // lmText
            // 
            this.lmText.Location = new System.Drawing.Point(115, 55);
            this.lmText.Name = "lmText";
            this.lmText.ReadOnly = true;
            this.lmText.Size = new System.Drawing.Size(311, 20);
            this.lmText.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 58);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(98, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "LightMapFileName:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 136);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(75, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "PatchPerSide:";
            // 
            // pps
            // 
            this.pps.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.pps.FormattingEnabled = true;
            this.pps.Items.AddRange(new object[] {
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
            "1024"});
            this.pps.Location = new System.Drawing.Point(115, 133);
            this.pps.Name = "pps";
            this.pps.Size = new System.Drawing.Size(121, 21);
            this.pps.TabIndex = 7;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 162);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(68, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "HeightScale:";
            // 
            // heightScale
            // 
            this.heightScale.Location = new System.Drawing.Point(115, 159);
            this.heightScale.Name = "heightScale";
            this.heightScale.Size = new System.Drawing.Size(121, 20);
            this.heightScale.TabIndex = 9;
            // 
            // sideScale
            // 
            this.sideScale.Location = new System.Drawing.Point(115, 185);
            this.sideScale.Name = "sideScale";
            this.sideScale.Size = new System.Drawing.Size(121, 20);
            this.sideScale.TabIndex = 11;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 188);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(58, 13);
            this.label5.TabIndex = 10;
            this.label5.Text = "SideScale:";
            // 
            // lightMapHeight
            // 
            this.lightMapHeight.Location = new System.Drawing.Point(115, 237);
            this.lightMapHeight.Name = "lightMapHeight";
            this.lightMapHeight.Size = new System.Drawing.Size(121, 20);
            this.lightMapHeight.TabIndex = 15;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(3, 240);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(85, 13);
            this.label6.TabIndex = 14;
            this.label6.Text = "LightMapHeight:";
            // 
            // lightMapWidth
            // 
            this.lightMapWidth.Location = new System.Drawing.Point(115, 211);
            this.lightMapWidth.Name = "lightMapWidth";
            this.lightMapWidth.Size = new System.Drawing.Size(121, 20);
            this.lightMapWidth.TabIndex = 13;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(3, 214);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(82, 13);
            this.label7.TabIndex = 12;
            this.label7.Text = "LightMapWidth:";
            // 
            // pngDialog
            // 
            this.pngDialog.FileName = "Landscape dialog";
            this.pngDialog.Filter = "Png files (*.png)|*.png|All files (*.*)|*.*";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(432, 79);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(32, 23);
            this.button1.TabIndex = 18;
            this.button1.Text = "...";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // effect
            // 
            this.effect.Location = new System.Drawing.Point(115, 81);
            this.effect.Name = "effect";
            this.effect.ReadOnly = true;
            this.effect.Size = new System.Drawing.Size(311, 20);
            this.effect.TabIndex = 17;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(3, 84);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(82, 13);
            this.label8.TabIndex = 16;
            this.label8.Text = "EffectFileName:";
            // 
            // effectDialog
            // 
            this.effectDialog.FileName = "openFileDialog1";
            this.effectDialog.Filter = "FX effect files (*.fx)|*.fx|All files (*.*)|*.*";
            // 
            // name
            // 
            this.name.Location = new System.Drawing.Point(115, 3);
            this.name.Name = "name";
            this.name.Size = new System.Drawing.Size(311, 20);
            this.name.TabIndex = 20;
            // 
            // landNameLabel
            // 
            this.landNameLabel.AutoSize = true;
            this.landNameLabel.Location = new System.Drawing.Point(3, 6);
            this.landNameLabel.Name = "landNameLabel";
            this.landNameLabel.Size = new System.Drawing.Size(62, 13);
            this.landNameLabel.TabIndex = 19;
            this.landNameLabel.Text = "LandName:";
            // 
            // pixelError
            // 
            this.pixelError.Location = new System.Drawing.Point(115, 107);
            this.pixelError.Name = "pixelError";
            this.pixelError.Size = new System.Drawing.Size(121, 20);
            this.pixelError.TabIndex = 22;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(3, 110);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(54, 13);
            this.label9.TabIndex = 21;
            this.label9.Text = "PixelError:";
            // 
            // LandscapeProps
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pixelError);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.name);
            this.Controls.Add(this.landNameLabel);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.effect);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.lightMapHeight);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.lightMapWidth);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.sideScale);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.heightScale);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.pps);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.lmButton);
            this.Controls.Add(this.lmText);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.hmButton);
            this.Controls.Add(this.hmText);
            this.Controls.Add(this.label1);
            this.Name = "LandscapeProps";
            this.Size = new System.Drawing.Size(469, 261);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox hmText;
        private System.Windows.Forms.Button hmButton;
        private System.Windows.Forms.Button lmButton;
        private System.Windows.Forms.TextBox lmText;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox pps;
        private System.Windows.Forms.Label label4;
        private ValueTextBox heightScale;
        private ValueTextBox sideScale;
        private System.Windows.Forms.Label label5;
        private ValueTextBox lightMapHeight;
        private System.Windows.Forms.Label label6;
        private ValueTextBox lightMapWidth;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.OpenFileDialog pngDialog;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox effect;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.OpenFileDialog effectDialog;
        private System.Windows.Forms.TextBox name;
        private System.Windows.Forms.Label landNameLabel;
        private ValueTextBox pixelError;
        private System.Windows.Forms.Label label9;
    }
}
