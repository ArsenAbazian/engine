namespace sceneEditor {
    partial class LandscapePropsForm {
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
            this.landscapePropsEx1 = new sceneEditor.LandscapePropsEx();
            this.SuspendLayout();
            // 
            // landscapePropsEx1
            // 
            this.landscapePropsEx1.Landscape = null;
            this.landscapePropsEx1.Location = new System.Drawing.Point(0, 0);
            this.landscapePropsEx1.Name = "landscapePropsEx1";
            this.landscapePropsEx1.Size = new System.Drawing.Size(469, 290);
            this.landscapePropsEx1.TabIndex = 0;
            this.landscapePropsEx1.PixelErrorChanged += new System.EventHandler(this.landscapePropsEx1_PixelErrorChanged);
            this.landscapePropsEx1.OkClick += new System.EventHandler(this.landscapePropsEx1_OkClick);
            this.landscapePropsEx1.LightMapHeightChanged += new System.EventHandler(this.landscapePropsEx1_LightMapHeightChanged);
            this.landscapePropsEx1.LightMapWidthChanged += new System.EventHandler(this.landscapePropsEx1_LightMapWidthChanged);
            this.landscapePropsEx1.NameChanged += new System.EventHandler(this.landscapePropsEx1_NameChanged);
            this.landscapePropsEx1.HeightScaleChanged += new System.EventHandler(this.landscapePropsEx1_HeightScaleChanged);
            this.landscapePropsEx1.SideScaleChanged += new System.EventHandler(this.landscapePropsEx1_SideScaleChanged);
            this.landscapePropsEx1.HeightMapFileChanged += new System.EventHandler(this.landscapePropsEx1_HeightMapFileChanged);
            this.landscapePropsEx1.PatchPerSideChanged += new System.EventHandler(this.landscapePropsEx1_PatchPerSideChanged);
            this.landscapePropsEx1.LightMapFileChanged += new System.EventHandler(this.landscapePropsEx1_LightMapFileChanged);
            this.landscapePropsEx1.EffectChanged += new System.EventHandler(this.landscapePropsEx1_EffectChanged);
            // 
            // LandscapePropsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(469, 291);
            this.Controls.Add(this.landscapePropsEx1);
            this.Name = "LandscapePropsForm";
            this.Text = "LandscapePropsForm";
            this.ResumeLayout(false);

        }

        #endregion

        private LandscapePropsEx landscapePropsEx1;

    }
}