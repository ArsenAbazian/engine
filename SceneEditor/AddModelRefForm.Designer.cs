namespace sceneEditor {
    partial class AddModelRefForm {
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
            this.addModelRefControl1 = new sceneEditor.AddModelRefControl();
            this.SuspendLayout();
            // 
            // addModelRefControl1
            // 
            this.addModelRefControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.addModelRefControl1.Location = new System.Drawing.Point(0, 0);
            this.addModelRefControl1.Name = "addModelRefControl1";
            this.addModelRefControl1.Size = new System.Drawing.Size(253, 347);
            this.addModelRefControl1.TabIndex = 0;
            this.addModelRefControl1.OkClick += new System.EventHandler(this.addModelRefControl1_OkClick);
            // 
            // AddModelRefForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(253, 347);
            this.Controls.Add(this.addModelRefControl1);
            this.Name = "AddModelRefForm";
            this.Text = "AddModelRefForm";
            this.TopMost = true;
            this.ResumeLayout(false);

        }

        #endregion

        private AddModelRefControl addModelRefControl1;
    }
}