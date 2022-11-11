namespace sceneEditor
{
    partial class LoadSceneControl
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.sceneFileText = new System.Windows.Forms.TextBox();
            this.sceneFileButton = new System.Windows.Forms.Button();
            this.dataDirButton = new System.Windows.Forms.Button();
            this.dataDirText = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.openSceneDialog = new System.Windows.Forms.OpenFileDialog();
            this.dataDirDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.OkButton = new System.Windows.Forms.Button();
            this.CancelButton = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.dataDirButton);
            this.groupBox1.Controls.Add(this.dataDirText);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.sceneFileButton);
            this.groupBox1.Controls.Add(this.sceneFileText);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(455, 71);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = " Scene Files ";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(60, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Scene File:";
            // 
            // sceneFileText
            // 
            this.sceneFileText.Location = new System.Drawing.Point(90, 19);
            this.sceneFileText.Name = "sceneFileText";
            this.sceneFileText.ReadOnly = true;
            this.sceneFileText.Size = new System.Drawing.Size(324, 20);
            this.sceneFileText.TabIndex = 1;
            // 
            // sceneFileButton
            // 
            this.sceneFileButton.Location = new System.Drawing.Point(420, 17);
            this.sceneFileButton.Name = "sceneFileButton";
            this.sceneFileButton.Size = new System.Drawing.Size(30, 23);
            this.sceneFileButton.TabIndex = 2;
            this.sceneFileButton.Text = "...";
            this.sceneFileButton.UseVisualStyleBackColor = true;
            this.sceneFileButton.Click += new System.EventHandler(this.sceneFileButton_Click);
            // 
            // dataDirButton
            // 
            this.dataDirButton.Location = new System.Drawing.Point(420, 43);
            this.dataDirButton.Name = "dataDirButton";
            this.dataDirButton.Size = new System.Drawing.Size(30, 23);
            this.dataDirButton.TabIndex = 5;
            this.dataDirButton.Text = "...";
            this.dataDirButton.UseVisualStyleBackColor = true;
            this.dataDirButton.Click += new System.EventHandler(this.dataDirButton_Click);
            // 
            // dataDirText
            // 
            this.dataDirText.Location = new System.Drawing.Point(90, 45);
            this.dataDirText.Name = "dataDirText";
            this.dataDirText.ReadOnly = true;
            this.dataDirText.Size = new System.Drawing.Size(324, 20);
            this.dataDirText.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(78, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Data Directory:";
            // 
            // openSceneDialog
            // 
            this.openSceneDialog.Filter = "Scene files (*.asc)|*.asc|All files (*.*)|*.*";
            this.openSceneDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.openSceneDialog_FileOk);
            // 
            // OkButton
            // 
            this.OkButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OkButton.Location = new System.Drawing.Point(297, 80);
            this.OkButton.Name = "OkButton";
            this.OkButton.Size = new System.Drawing.Size(75, 23);
            this.OkButton.TabIndex = 9;
            this.OkButton.Text = "OK";
            this.OkButton.UseVisualStyleBackColor = true;
            // 
            // CancelButton
            // 
            this.CancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelButton.Location = new System.Drawing.Point(378, 80);
            this.CancelButton.Name = "CancelButton";
            this.CancelButton.Size = new System.Drawing.Size(75, 23);
            this.CancelButton.TabIndex = 8;
            this.CancelButton.Text = "Cancel";
            this.CancelButton.UseVisualStyleBackColor = true;
            // 
            // LoadSceneControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.OkButton);
            this.Controls.Add(this.CancelButton);
            this.Controls.Add(this.groupBox1);
            this.Name = "LoadSceneControl";
            this.Size = new System.Drawing.Size(461, 107);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button dataDirButton;
        private System.Windows.Forms.TextBox dataDirText;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button sceneFileButton;
        private System.Windows.Forms.TextBox sceneFileText;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.OpenFileDialog openSceneDialog;
        private System.Windows.Forms.FolderBrowserDialog dataDirDialog;
        private System.Windows.Forms.Button OkButton;
        private System.Windows.Forms.Button CancelButton;
    }
}
