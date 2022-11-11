namespace sceneEditor {
    partial class EnterNameForm {
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
            this.enterStringControl1 = new sceneEditor.EnterStringControl();
            this.SuspendLayout();
            // 
            // enterStringControl1
            // 
            this.enterStringControl1.EnterText = "Enter name:";
            this.enterStringControl1.Location = new System.Drawing.Point(0, 2);
            this.enterStringControl1.Name = "enterStringControl1";
            this.enterStringControl1.Size = new System.Drawing.Size(218, 67);
            this.enterStringControl1.TabIndex = 0;
            this.enterStringControl1.Value = "";
            this.enterStringControl1.OkClick += new System.EventHandler(this.enterStringControl1_OkClick);
            // 
            // EnterNameForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(220, 70);
            this.Controls.Add(this.enterStringControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "EnterNameForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Enter Name";
            this.TopMost = true;
            this.ResumeLayout(false);

        }

        #endregion

        private EnterStringControl enterStringControl1;
    }
}