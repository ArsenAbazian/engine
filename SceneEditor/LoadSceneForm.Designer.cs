namespace sceneEditor
{
    partial class LoadSceneForm
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.loadSceneControl1 = new sceneEditor.LoadSceneControl();
            this.SuspendLayout();
            // 
            // loadSceneControl1
            // 
            this.loadSceneControl1.DataDirectoryName = "";
            this.loadSceneControl1.Location = new System.Drawing.Point(0, 0);
            this.loadSceneControl1.Name = "loadSceneControl1";
            this.loadSceneControl1.SceneFileName = "";
            this.loadSceneControl1.Size = new System.Drawing.Size(461, 102);
            this.loadSceneControl1.TabIndex = 0;
            // 
            // LoadSceneForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(461, 106);
            this.Controls.Add(this.loadSceneControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "LoadSceneForm";
            this.Text = "LoadSceneForm";
            this.ResumeLayout(false);

        }

        #endregion

        private LoadSceneControl loadSceneControl1;
    }
}