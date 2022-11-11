namespace imageGallery {
	partial class ImageGalleryForm {
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
            this.imageGalleryControl1 = new imageGallery.ImageGalleryControl();
            this.SuspendLayout();
            // 
            // imageGalleryControl1
            // 
            this.imageGalleryControl1.AutoScroll = true;
            this.imageGalleryControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.imageGalleryControl1.Indent = 16;
            this.imageGalleryControl1.ItemSize = new System.Drawing.Size(128, 128);
            this.imageGalleryControl1.LayoutOnSize = true;
            this.imageGalleryControl1.Location = new System.Drawing.Point(0, 0);
            this.imageGalleryControl1.Name = "imageGalleryControl1";
            this.imageGalleryControl1.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.imageGalleryControl1.SelectedBorderColor = System.Drawing.Color.Blue;
            this.imageGalleryControl1.SelectedFileName = null;
            this.imageGalleryControl1.Size = new System.Drawing.Size(734, 493);
            this.imageGalleryControl1.TabIndex = 0;
            // 
            // ImageGalleryForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(734, 493);
            this.Controls.Add(this.imageGalleryControl1);
            this.Name = "ImageGalleryForm";
            this.Text = "ImageGalleryForm";
            this.TopMost = true;
            this.ResumeLayout(false);

		}

		#endregion

		private ImageGalleryControl imageGalleryControl1;
	}
}