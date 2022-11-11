using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace imageGallery {
	public partial class ImageViewForm : Form {
		public ImageViewForm() {
			InitializeComponent();
		}

		string imageFileName;
		public string ImageFileName { 
			get { return imageFileName; }
			set { 
				imageFileName = value;
				OnImageFileNameChanged();
			} 
		}

		protected virtual void OnImageFileNameChanged() {
			Text = ImageFileName;
			pictureBox1.Image = Image.FromFile(imageFileName);
			if(pictureBox1.Image == null) return;
			ClientSize = pictureBox1.Image.Size;
		}
	}
}