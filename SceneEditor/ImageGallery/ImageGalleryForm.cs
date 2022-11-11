using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace imageGallery {
	public partial class ImageGalleryForm : Form {
		public ImageGalleryForm() {
			InitializeComponent();
		}

		public ImageGalleryControl ImageGallery { get { return imageGalleryControl1; } }
	}
}