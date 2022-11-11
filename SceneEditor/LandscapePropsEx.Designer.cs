namespace sceneEditor {
    partial class LandscapePropsEx {
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
            this.landscapeProps1 = new sceneEditor.LandscapeProps();
            this.passProps = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // landscapeProps1
            // 
            this.landscapeProps1.Location = new System.Drawing.Point(0, 0);
            this.landscapeProps1.Name = "landscapeProps1";
            this.landscapeProps1.Size = new System.Drawing.Size(469, 261);
            this.landscapeProps1.TabIndex = 0;
            // 
            // passProps
            // 
            this.passProps.Location = new System.Drawing.Point(3, 264);
            this.passProps.Name = "passProps";
            this.passProps.Size = new System.Drawing.Size(234, 23);
            this.passProps.TabIndex = 1;
            this.passProps.Text = "Pass Properties";
            this.passProps.UseVisualStyleBackColor = true;
            this.passProps.Click += new System.EventHandler(this.passProps_Click);
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(391, 264);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 2;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            // 
            // LandscapePropsEx
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.passProps);
            this.Controls.Add(this.landscapeProps1);
            this.Name = "LandscapePropsEx";
            this.Size = new System.Drawing.Size(469, 290);
            this.ResumeLayout(false);

        }

        #endregion

        private LandscapeProps landscapeProps1;
        private System.Windows.Forms.Button passProps;
        private System.Windows.Forms.Button okButton;
    }
}
