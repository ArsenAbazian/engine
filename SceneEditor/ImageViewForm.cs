using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace sceneEditor {
    public partial class ImageViewForm : FormWithSceneUpdate {
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
            bool useTemp = false;
            string tempFile = "";
            if (TmpImageCreator.CreateTmpImage(ImageFileName, out tempFile)) {
                pictureBox1.ImageLocation = tempFile;
                useTemp = true;
            }
            else
                pictureBox1.ImageLocation = ImageFileName;
            try {
                pictureBox1.Load();
            }
            catch (Exception) { }
            finally {
                if (useTemp)
                    File.Delete(pictureBox1.ImageLocation);
            }
            if(pictureBox1.Image == null) return;
			ClientSize = pictureBox1.Image.Size;
		}
	}
}