namespace sceneEditor.Lights {
    partial class a3dLightBaseControl {
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
            this.lightName = new sceneEditor.Controls.ValueTextBox();
            this.lightColor = new imageGallery.ColorPicker();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 10);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Name:";
            // 
            // lightName
            // 
            this.lightName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lightName.Location = new System.Drawing.Point(81, 7);
            this.lightName.Name = "lightName";
            this.lightName.Size = new System.Drawing.Size(136, 20);
            this.lightName.TabIndex = 1;
            this.lightName.ValueChanged += new System.EventHandler(this.lightName_ValueChanged);
            // 
            // lightColor
            // 
            this.lightColor.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lightColor.BorderColor = System.Drawing.Color.Empty;
            this.lightColor.Color = System.Drawing.Color.Empty;
            this.lightColor.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.lightColor.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.lightColor.FormattingEnabled = true;
            this.lightColor.Location = new System.Drawing.Point(81, 33);
            this.lightColor.Name = "lightColor";
            this.lightColor.Size = new System.Drawing.Size(136, 21);
            this.lightColor.TabIndex = 2;
            this.lightColor.ColorChanged += new System.EventHandler(this.lightColor_ColorChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 36);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(34, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Color:";
            // 
            // a3dLightBaseControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.label2);
            this.Controls.Add(this.lightColor);
            this.Controls.Add(this.lightName);
            this.Controls.Add(this.label1);
            this.MaximumSize = new System.Drawing.Size(223, 1000);
            this.MinimumSize = new System.Drawing.Size(223, 58);
            this.Name = "a3dLightBaseControl";
            this.Size = new System.Drawing.Size(223, 58);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        protected sceneEditor.Controls.ValueTextBox lightName;
        protected imageGallery.ColorPicker lightColor;
    }
}
