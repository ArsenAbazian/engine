namespace sceneEditor.a3dScript {
    partial class ScriptForm {
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.scriptEditControl1 = new sceneEditor.a3dScript.ScriptEditControl();
            this.SuspendLayout();
            // 
            // scriptEditControl1
            // 
            this.scriptEditControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scriptEditControl1.Location = new System.Drawing.Point(0, 0);
            this.scriptEditControl1.Name = "scriptEditControl1";
            this.scriptEditControl1.Size = new System.Drawing.Size(479, 330);
            this.scriptEditControl1.TabIndex = 0;
            // 
            // ScriptForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(479, 330);
            this.Controls.Add(this.scriptEditControl1);
            this.Name = "ScriptForm";
            this.Text = "Lines 1; ALL 0,00014200; DrawRow 0,00010168; MakeRanges 0,00000354; ExpandTabs 0," +
                "00000138; PaintRange 0,00006859; MeasureWidth 0,00000143; DrawText 0,00007755; ";
            this.ResumeLayout(false);

        }

        #endregion

        private ScriptEditControl scriptEditControl1;
    }
}