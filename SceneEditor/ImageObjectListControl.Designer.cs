namespace sceneEditor {
	partial class ImageObjectListControl {
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.imageItemList = new System.Windows.Forms.CheckedListBox();
            this.propButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.removeButton = new System.Windows.Forms.Button();
            this.picturePreview = new System.Windows.Forms.PictureBox();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picturePreview)).BeginInit();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.picturePreview);
            this.splitContainer1.Size = new System.Drawing.Size(447, 308);
            this.splitContainer1.SplitterDistance = 218;
            this.splitContainer1.TabIndex = 0;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.imageItemList);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.propButton);
            this.splitContainer2.Panel2.Controls.Add(this.okButton);
            this.splitContainer2.Panel2.Controls.Add(this.removeButton);
            this.splitContainer2.Size = new System.Drawing.Size(218, 308);
            this.splitContainer2.SplitterDistance = 274;
            this.splitContainer2.TabIndex = 0;
            // 
            // imageItemList
            // 
            this.imageItemList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.imageItemList.FormattingEnabled = true;
            this.imageItemList.Location = new System.Drawing.Point(0, 0);
            this.imageItemList.Name = "imageItemList";
            this.imageItemList.Size = new System.Drawing.Size(218, 274);
            this.imageItemList.TabIndex = 0;
            this.imageItemList.SelectedIndexChanged += new System.EventHandler(this.imageItemList_SelectedIndexChanged);
            // 
            // propButton
            // 
            this.propButton.Location = new System.Drawing.Point(84, 4);
            this.propButton.Name = "propButton";
            this.propButton.Size = new System.Drawing.Size(75, 23);
            this.propButton.TabIndex = 2;
            this.propButton.Text = "Properties";
            this.propButton.UseVisualStyleBackColor = true;
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(172, 4);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(43, 23);
            this.okButton.TabIndex = 1;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            // 
            // removeButton
            // 
            this.removeButton.Location = new System.Drawing.Point(3, 4);
            this.removeButton.Name = "removeButton";
            this.removeButton.Size = new System.Drawing.Size(75, 23);
            this.removeButton.TabIndex = 0;
            this.removeButton.Text = "Remove";
            this.removeButton.UseVisualStyleBackColor = true;
            // 
            // picturePreview
            // 
            this.picturePreview.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.picturePreview.Dock = System.Windows.Forms.DockStyle.Fill;
            this.picturePreview.Location = new System.Drawing.Point(0, 0);
            this.picturePreview.Name = "picturePreview";
            this.picturePreview.Size = new System.Drawing.Size(225, 308);
            this.picturePreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.picturePreview.TabIndex = 0;
            this.picturePreview.TabStop = false;
            this.picturePreview.Click += new System.EventHandler(this.picturePreview_Click);
            // 
            // ImageObjectListControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Name = "ImageObjectListControl";
            this.Size = new System.Drawing.Size(447, 308);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.picturePreview)).EndInit();
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.SplitContainer splitContainer2;
		private System.Windows.Forms.CheckedListBox imageItemList;
		private System.Windows.Forms.PictureBox picturePreview;
		private System.Windows.Forms.Button removeButton;
		private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button propButton;
	}
}
