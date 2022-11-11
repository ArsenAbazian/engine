using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
    public partial class ImageObjectListControl : ControlWithSceneUpdate {
		public ImageObjectListControl() {
			InitializeComponent();
		}

		ImageViewForm imageForm;
		public ImageViewForm ImageViewForm {
			get {
				if(imageForm == null || imageForm.IsDisposed)
					imageForm = new ImageViewForm();
				return imageForm;
			}
		}

		private void picturePreview_Click(object sender, EventArgs e) {
			if(ImageViewForm.Visible) return;
			IItemWithImageFile item = ImageItemList.SelectedItem as IItemWithImageFile;
            if(item == null) return;
            ImageViewForm.ImageFileName = item.ImageFileName;
			ImageViewForm.Show();
		}

		public event EventHandler OkClick {
			add { this.okButton.Click += value; }
			remove { this.okButton.Click -= value; }
		}

        public CheckedListBox ImageItemList { get { return this.imageItemList; } }
        public event EventHandler PropertiesClick {
            add { this.propButton.Click += value; }
            remove { this.propButton.Click -= value; }
        } 

		private void imageItemList_SelectedIndexChanged(object sender, EventArgs e) {
            IItemWithImageFile item = ImageItemList.SelectedItem as IItemWithImageFile;
            if (item == null) return;
            picturePreview.Image = Image.FromFile(item.ImageFileName);
        }
	}

    public interface IItemWithImageFile {
        object Item { get; }
        string ItemName { get; }
        string ImageFileName { get; }
    }
}
