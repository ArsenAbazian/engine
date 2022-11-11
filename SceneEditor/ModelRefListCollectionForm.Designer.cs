namespace sceneEditor {
    partial class ModelRefCollectionForm {
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
            this.modelRefCollectionViewControl1 = new sceneEditor.ModelRefCollectionViewControl();
            this.SuspendLayout();
            // 
            // modelRefCollectionViewControl1
            // 
            this.modelRefCollectionViewControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.modelRefCollectionViewControl1.Location = new System.Drawing.Point(0, 0);
            this.modelRefCollectionViewControl1.Name = "modelRefCollectionViewControl1";
            this.modelRefCollectionViewControl1.Size = new System.Drawing.Size(234, 444);
            this.modelRefCollectionViewControl1.TabIndex = 0;
            this.modelRefCollectionViewControl1.OkClick += new System.EventHandler(this.modelRefCollectionViewControl1_OkClick);
            // 
            // ModelRefCollectionForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(234, 444);
            this.Controls.Add(this.modelRefCollectionViewControl1);
            this.Name = "ModelRefCollectionForm";
            this.Text = "ModelRefListCollectionForm";
            this.TopMost = true;
            this.ResumeLayout(false);

        }

        #endregion

        private ModelRefCollectionViewControl modelRefCollectionViewControl1;

    }
}