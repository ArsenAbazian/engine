using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.IO;
using ddsConvert;
using sceneEditor;

namespace imageGallery {
	public partial class ImageGalleryControl : UserControl, ICollectionChangedProvider {
        private static readonly object selectedImageChanged = new object();
        
        public ImageGalleryControl() {
			InitializeComponent();
		}

        Color selectedBorderColor = Color.Blue;
        protected virtual bool ShouldSerializeSelectedBorderColor() {
            return SelectedBorderColor == Color.Blue;
        }
        public Color SelectedBorderColor {
            get { return selectedBorderColor; }
            set {
                if (SelectedBorderColor == value) return;
                selectedBorderColor = value;
                Invalidate();
            }
        }

        int selectedBorderIndent = 2;
        [DefaultValue(2)]
        public int SelectedBorderIndent {
            get { return selectedBorderIndent; }
            set {
                if (SelectedBorderIndent == value) return;
                selectedBorderIndent = value;
                Invalidate();
            }
        }

        int selectedBorderThickness = 2;
        [DefaultValue(2)]
        public int SelectedBorderThickness {
            get { return selectedBorderThickness; }
            set {
                if (SelectedBorderThickness == value) return;
                selectedBorderThickness = value;
                Invalidate();
            }
        }

        protected virtual void RaiseSelectedImageChanged() { 
            EventHandler handler = Events[selectedImageChanged] as EventHandler;
            if(handler != null) handler(this, EventArgs.Empty);
        }
        
        public event EventHandler SelectedImageChanged { 
            add { Events.AddHandler(selectedImageChanged, value); }
            remove { Events.RemoveHandler(selectedImageChanged, value); }
        }
        
        string selectedFileName;
        public string SelectedFileName {
            get { return selectedFileName; }
            set {
                if (SelectedFileName == value) return;
                selectedFileName = value;
                Invalidate();
                RaiseSelectedImageChanged();
            }
        }

        StringCollection coll;
		public StringCollection ImageFileNames {
			get {
				if(coll == null) coll = new StringCollection(this);
				return coll;
			}
		}

		PictureBoxCollection picColl;
		public PictureBoxCollection ImageCollection {
			get {
				if(picColl == null) picColl = new PictureBoxCollection();
				return picColl;
			}
		}

		#region ICollectionChangedProvider
		void ICollectionChangedProvider.OnInsertComplete(int index, object value) { 
		}
		void ICollectionChangedProvider.OnRemoveComplete(int index, object value) {
		}
		#endregion

		int indent = 16;
		public int Indent {
			get { return indent; }
			set {
				if(Indent == value) return;
				indent = value;
				OnIndentChanged();
			}
		}

		Size itemSize = new Size(128, 128);
		public Size ItemSize {
			get { return itemSize; }
			set {
				if(ItemSize == value) return;
				itemSize = value;
				OnItemSizeChanged();
			}
		}

		bool layoutOnSize = false;
		public bool LayoutOnSize {
			get { return layoutOnSize; }
			set {
				if(LayoutOnSize == value) return;
				layoutOnSize = value;
				UpdateImagesLayout();
			}
		}

		Orientation orientation = Orientation.Vertical;
		public Orientation Orientation {
			get { return orientation; }
			set {
				if(orientation == value) return;
				orientation = value;
				UpdateImagesLayout();
			}
		}

		int updateCount;
		public bool IsLockUpdate { get { return updateCount > 0; } }
		public void BeginUpdate() {
			updateCount++;
		}
		public void EndUpdate() {
			if(updateCount == 0) return;
			updateCount--;
			if(updateCount == 0)
				UpdateLayout();
		}
		
		protected virtual void OnIndentChanged() {
			UpdateImagesLayout();
		}

		protected virtual void OnItemSizeChanged() {
			UpdateImagesLayout();
		}

		protected virtual void UpdateLayout() {
			CreateImages();
			UpdateImagesLayout();
		}

		protected virtual void ClearImageCollection() {
			ImageCollection.Clear();
		}

		protected virtual void CreateImages() {
			ClearImageCollection();
			foreach(string fileName in ImageFileNames) {
                bool useTemp = false;
                string tempFile = "";
                PictureBox box = new PictureBox();
				box.BorderStyle = BorderStyle.FixedSingle;
				box.SizeMode = PictureBoxSizeMode.Zoom;
				box.MouseDown += new MouseEventHandler(OnImageMouseDown);
                box.Tag = fileName;
                if (TmpImageCreator.CreateTmpImage(fileName, out tempFile)) {
                    box.ImageLocation = tempFile;
                    useTemp = true;
                }
                else
                    box.ImageLocation = fileName;
                try {
                    box.Load();
                }
                catch (Exception e) { }
                finally {
                    if (useTemp) File.Delete(box.ImageLocation);
                }
				imageInfoTip.SetToolTip(box, fileName);
				this.Controls.Add(box);
				ImageCollection.Add(box);
			}
		}
				
		protected override void OnResize(EventArgs e) {
			base.OnResize(e);
			if(LayoutOnSize)
				UpdateImagesLayout();
		}

        protected override void OnPaint(PaintEventArgs e) {
            base.OnPaint(e);
            foreach(PictureBox pic in ImageCollection) {
                if(((string)pic.Tag) == SelectedFileName) {
                    Rectangle rect = pic.Bounds;
                    rect.Inflate(SelectedBorderIndent, SelectedBorderIndent);
                    e.Graphics.DrawRectangle(new Pen(SelectedBorderColor, SelectedBorderThickness), rect);
                    break;
                }   
            }
        }

        ImageViewForm viewForm;
		public ImageViewForm ImageView {
			get {
				if(viewForm == null || viewForm.IsDisposed)
					viewForm = new ImageViewForm();
				return viewForm;
			}
		}

		protected virtual void OnImageMouseDown(object sender, MouseEventArgs e) {
            if (e.Button == MouseButtons.Right) {
                if (ImageView.Visible) ImageView.Close();
                ImageView.ImageFileName = (sender as PictureBox).ImageLocation;
                ImageView.Show();
            }
            if (e.Button == MouseButtons.Left) {
                SelectedFileName = (string)(sender as PictureBox).Tag;
            }
		}

		protected virtual void UpdateImagesLayout() {
			if(Orientation == Orientation.Vertical)
				UpdateImagesLayoutVertical();
			else
				UpdateImageLayoutHorizontal();
		}

		protected virtual void UpdateImageLayoutHorizontal() {
			Rectangle imageRect = new Rectangle(new Point(Indent, Indent), ItemSize);
			foreach(PictureBox box in ImageCollection) {
				box.Bounds = imageRect;
				imageRect.Y = imageRect.Bottom + Indent;
				if(imageRect.Bottom > ClientRectangle.Height) {
					imageRect.Y = Indent;
					imageRect.X = imageRect.Right + Indent;
				}
			}
		}

		protected virtual void UpdateImagesLayoutVertical() { 
			Rectangle imageRect = new Rectangle(new Point(Indent, Indent), ItemSize);
			foreach(PictureBox box in ImageCollection) {
				box.Bounds = imageRect;
				imageRect.X = imageRect.Right + Indent;
				if(imageRect.Right > ClientRectangle.Width) {
					imageRect.X = Indent;
					imageRect.Y = imageRect.Bottom + Indent;
				}
			}
		}
	}

	public interface ICollectionChangedProvider {
		void OnInsertComplete(int index, object value);
		void OnRemoveComplete(int index, object value);
	}

	public class StringCollection : CollectionBase {
		ICollectionChangedProvider owner;
		public StringCollection(ICollectionChangedProvider owner) { this.owner = owner; }
		
		public int Add(string str) { return List.Add(str); }
		public void Insert(int index, string str) { List.Insert(index, str); }
		public void Remove(string str) { List.Remove(str); }
		public bool Contains(string str) { return List.Contains(str); }
		public int IndexOf(string str) { return List.IndexOf(str); }
		
		protected override void OnInsertComplete(int index, object value) {
			base.OnInsertComplete(index, value);
			if(owner != null)
				owner.OnInsertComplete(index, value);
		}
		protected override void OnRemoveComplete(int index, object value) {
			base.OnRemoveComplete(index, value);
			if(owner != null)
				owner.OnRemoveComplete(index, value);
		}

		public string this[int index] { get { return List[index] as string; } set { List[index] = value; } }
	}

	public class PictureBoxCollection : CollectionBase {
		public int Add(PictureBox pic) { return List.Add(pic); }
		public void Insert(int index, PictureBox pic) { List.Insert(index, pic); }
		public void Remove(PictureBox pic) { List.Remove(pic); }
		public bool Contains(PictureBox pic) { return List.Contains(pic); }
		public int IndexOf(PictureBox pic) { return List.IndexOf(pic); }
        public PictureBox this[int index] { get { return InnerList[index] as PictureBox; } }
         protected override void OnClear() {
			foreach(PictureBox box in this) { 
				box.Parent = null;
			}
			base.OnClear();
		}
	}
}
