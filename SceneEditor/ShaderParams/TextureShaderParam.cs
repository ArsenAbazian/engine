using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;
using imageGallery;
using System.IO;
using ddsConvert;

namespace sceneEditor.ShaderParams {
    public partial class TextureShaderParam : sceneEditor.ShaderParams.ShaderParamInfoBase {
        public TextureShaderParam() {
            InitializeComponent();
        }

        public new a3dTextureShaderParam_Mapper Param {
            get { return base.Param as a3dTextureShaderParam_Mapper; }
            set { base.Param = value; }
        }

        protected override void OnParamChanged() {
            base.OnParamChanged();
            UpdateEditors();
        }

        public string GetFullPathName() {
            return Param.FileName[1] == ':' ? Param.FileName : MainForm.Scene.TextureDirectoryFull + Param.FileName;
        }

        protected virtual void UpdateEditors() {
            string fileName = GetFullPathName();
            if (File.Exists(fileName)) {
                bool useTemp = false;
                string tempFile = "";
                if (TmpImageCreator.CreateTmpImage(fileName, out tempFile)) {
                    pictureBox1.ImageLocation = tempFile;
                    useTemp = true;
                }
                else
                    pictureBox1.ImageLocation = fileName;
                try {
                    pictureBox1.Load();
                }
                catch (Exception e) { }
                finally {
                    if (useTemp) 
                        File.Delete(tempFile);
                }
            }
        }

        protected virtual void ShowImageGallery() {
            if (MainForm == null) return;
            ImageGalleryForm form = new ImageGalleryForm();
            form.ImageGallery.SelectedImageChanged += new EventHandler(OnSelectedImageChanged);
            form.Disposed += new EventHandler(OnFormDisposed);
            form.ImageGallery.BeginUpdate();
            foreach(a3dTextureDesc_Mapper textureDesc in MainForm.Scene.Textures) {
                form.ImageGallery.ImageFileNames.Add(textureDesc.FullPathName);
            }
            form.ImageGallery.EndUpdate();
            form.Show();
        }

        protected virtual void OnFormDisposed(object sender, EventArgs e) {
            ImageGalleryForm form = sender as ImageGalleryForm;
            if (form == null) return;
            form.Disposed -= new EventHandler(OnFormDisposed);
            form.ImageGallery.SelectedImageChanged -= new EventHandler(OnSelectedImageChanged);
        }

        protected virtual string GetOffsetTexturePath(a3dTextureDesc_Mapper texDesc) { 
            string res;
            if(texDesc.FullPathName.Contains(MainForm.Scene.TextureDirectoryFull)) 
                res = texDesc.FullPathName.Substring(MainForm.Scene.TextureDirectoryFull.Length, texDesc.FullPathName.Length - MainForm.Scene.TextureDirectoryFull.Length);
            else
                res = texDesc.FullPathName;
            return res;
        }

        protected virtual void OnSelectedImageChanged(object sender, EventArgs e) { 
            ImageGalleryControl imageGallery = sender as ImageGalleryControl;
            if(imageGallery == null) return;
            foreach(a3dTextureDesc_Mapper texDesc in MainForm.Scene.Textures) { 
                if(texDesc.FullPathName == imageGallery.SelectedFileName) {
                    Param.TextureDesc = texDesc;
                    Param.FileName = GetOffsetTexturePath(texDesc);
                    string tmpFile = null;
                    bool tmpUsed = false;
                    try {
                        if (TmpImageCreator.CreateTmpImage(texDesc.FullPathName, out tmpFile)) {
                            pictureBox1.ImageLocation = tmpFile;
                            tmpUsed = true;
                        }
                        else
                            pictureBox1.ImageLocation = texDesc.FullPathName;
                        pictureBox1.Load();
                    }
                    finally {
                        if (tmpUsed)
                            File.Delete(tmpFile);
                    }
                    break;
                }
            }
            UpdateMainForm();
        }

        private void imgButton_Click(object sender, EventArgs e) {
            ShowImageGallery();
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e) {
            if(e.Button == MouseButtons.Right) {
                ImageViewForm form = new ImageViewForm();
                form.ImageFileName = GetFullPathName();
                form.MainForm = MainForm;
                form.Show();
            }
            else if(e.Button == MouseButtons.Left && e.Clicks >= 2) {
                ShowImageGallery();    
            } 
        }
    }
}

