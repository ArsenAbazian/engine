namespace sceneEditor.Dependency {
    partial class GraphDependencyPropertyForm {
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
            this.okButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.graphDependencyPropertyControl1 = new sceneEditor.Dependency.GraphDependencyPropertyControl();
            this.SuspendLayout();
            // 
            // okButton
            // 
            this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.Location = new System.Drawing.Point(12, 361);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(69, 23);
            this.okButton.TabIndex = 1;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            // 
            // cancelButton
            // 
            this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(87, 361);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(69, 23);
            this.cancelButton.TabIndex = 2;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // graphDependencyPropertyControl1
            // 
            this.graphDependencyPropertyControl1.Destination = null;
            this.graphDependencyPropertyControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.graphDependencyPropertyControl1.Location = new System.Drawing.Point(0, 0);
            this.graphDependencyPropertyControl1.Name = "graphDependencyPropertyControl1";
            this.graphDependencyPropertyControl1.Size = new System.Drawing.Size(847, 396);
            this.graphDependencyPropertyControl1.Source1 = null;
            this.graphDependencyPropertyControl1.Source2 = null;
            this.graphDependencyPropertyControl1.TabIndex = 0;
            // 
            // GraphDependencyPropertyForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(847, 396);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.graphDependencyPropertyControl1);
            this.Name = "GraphDependencyPropertyForm";
            this.Text = "GraphDependencyPropertyForm";
            this.ResumeLayout(false);

        }

        #endregion

        private GraphDependencyPropertyControl graphDependencyPropertyControl1;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button cancelButton;
    }
}