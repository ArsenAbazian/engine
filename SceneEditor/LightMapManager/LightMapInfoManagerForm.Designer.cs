namespace sceneEditor.LightMapManager {
    partial class LightMapInfoManagerForm {
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.lightMapManagerControl1 = new sceneEditor.LightMapManager.LightMapManagerControl();
            this.okButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lightMapManagerControl1
            // 
            this.lightMapManagerControl1.Location = new System.Drawing.Point(-1, 0);
            this.lightMapManagerControl1.Name = "lightMapManagerControl1";
            this.lightMapManagerControl1.Size = new System.Drawing.Size(565, 447);
            this.lightMapManagerControl1.TabIndex = 0;
            // 
            // okButton
            // 
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.Location = new System.Drawing.Point(476, 410);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 1;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // LightMapInfoManagerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(563, 445);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.lightMapManagerControl1);
            this.Name = "LightMapInfoManagerForm";
            this.Text = "LightMapInfoManagerForm";
            this.ResumeLayout(false);

        }

        #endregion

        private LightMapManagerControl lightMapManagerControl1;
        private System.Windows.Forms.Button okButton;
    }
}