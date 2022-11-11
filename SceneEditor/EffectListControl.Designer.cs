namespace sceneEditor {
    partial class EffectListControl {
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
            this.effectList = new System.Windows.Forms.ListBox();
            this.selButton = new System.Windows.Forms.Button();
            this.addNew = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // effectList
            // 
            this.effectList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.effectList.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.effectList.FormattingEnabled = true;
            this.effectList.ItemHeight = 15;
            this.effectList.Location = new System.Drawing.Point(3, 3);
            this.effectList.Name = "effectList";
            this.effectList.Size = new System.Drawing.Size(214, 409);
            this.effectList.TabIndex = 1;
            this.effectList.DoubleClick += new System.EventHandler(this.effectList_DoubleClick);
            // 
            // selButton
            // 
            this.selButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.selButton.Location = new System.Drawing.Point(142, 418);
            this.selButton.Name = "selButton";
            this.selButton.Size = new System.Drawing.Size(75, 23);
            this.selButton.TabIndex = 2;
            this.selButton.Text = "Select";
            this.selButton.UseVisualStyleBackColor = true;
            this.selButton.Click += new System.EventHandler(this.selButton_Click);
            // 
            // addNew
            // 
            this.addNew.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.addNew.Location = new System.Drawing.Point(3, 418);
            this.addNew.Name = "addNew";
            this.addNew.Size = new System.Drawing.Size(75, 23);
            this.addNew.TabIndex = 3;
            this.addNew.Text = "Add New";
            this.addNew.UseVisualStyleBackColor = true;
            this.addNew.Click += new System.EventHandler(this.addNew_Click);
            // 
            // EffectListControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.addNew);
            this.Controls.Add(this.selButton);
            this.Controls.Add(this.effectList);
            this.Name = "EffectListControl";
            this.Size = new System.Drawing.Size(220, 444);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox effectList;
        private System.Windows.Forms.Button selButton;
        private System.Windows.Forms.Button addNew;
    }
}
