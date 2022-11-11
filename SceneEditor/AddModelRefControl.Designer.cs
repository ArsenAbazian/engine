namespace sceneEditor {
    partial class AddModelRefControl {
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
            this.modelsList = new System.Windows.Forms.ListBox();
            this.addButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // modelsList
            // 
            this.modelsList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.modelsList.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.modelsList.FormattingEnabled = true;
            this.modelsList.ItemHeight = 15;
            this.modelsList.Location = new System.Drawing.Point(3, 3);
            this.modelsList.Name = "modelsList";
            this.modelsList.Size = new System.Drawing.Size(219, 304);
            this.modelsList.TabIndex = 0;
            this.modelsList.DoubleClick += new System.EventHandler(this.modelsList_DoubleClick);
            // 
            // addButton
            // 
            this.addButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.addButton.Location = new System.Drawing.Point(3, 308);
            this.addButton.Name = "addButton";
            this.addButton.Size = new System.Drawing.Size(92, 23);
            this.addButton.TabIndex = 1;
            this.addButton.Text = "Add Model Ref";
            this.addButton.UseVisualStyleBackColor = true;
            this.addButton.Click += new System.EventHandler(this.addButton_Click);
            // 
            // okButton
            // 
            this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.Location = new System.Drawing.Point(147, 308);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 2;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            // 
            // AddModelRefControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.addButton);
            this.Controls.Add(this.modelsList);
            this.Name = "AddModelRefControl";
            this.Size = new System.Drawing.Size(226, 334);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox modelsList;
        private System.Windows.Forms.Button addButton;
        private System.Windows.Forms.Button okButton;
    }
}
